#include "grid.h"

#include <vector>

#include <ArrayMesh.hpp>
#include <Mesh.hpp>
#include <MeshDataTool.hpp>
#include <Object.hpp>
#include <Vector3.hpp>

using namespace godot;

void Grid::_register_methods()
{
    register_method("_ready", &Grid::_ready);
    register_method("_process", &Grid::_process);
}

void Grid::_init()
{
    m_shader = Object::cast_to<ShaderMaterial>(get_material_override().ptr());

    m_num_longitudes   = 16;
    m_num_latitudes    = 8;
    m_num_subdivisions = 64;
}

void Grid::_ready()
{
    m_camera = get_node<CameraController>("../Camera");

    generate_grid();
}

void Grid::_process(float delta)
{
    m_camera->set_shader_uniforms(m_shader);

    // generate mesh
    PoolVector3Array verticies {};
    for(auto [from, to]: m_lines) {
        if(m_camera->to_cull(gd_vec32glm(from)) || m_camera->to_cull(gd_vec32glm(to)))
            continue;
        verticies.push_back(from);
        verticies.push_back(to);
    }

    ArrayMesh* arr_mesh = ArrayMesh::_new();
    Array      arrays {};
    arrays.resize(ArrayMesh::ARRAY_MAX);
    arrays[ArrayMesh::ARRAY_VERTEX] = verticies;

    arr_mesh->add_surface_from_arrays(Mesh::PRIMITIVE_LINES, arrays);
    set_mesh(arr_mesh);
}

Vector3 Grid::spherical_project(float la_ang, float lo_ang)
{
    float r = m_camera->get_radius();
    return {r * std::sin(la_ang) * std::cos(lo_ang),
            r * std::sin(la_ang) * std::sin(lo_ang),
            r * std::cos(la_ang)};
}

void Grid::generate_grid()
{
    {
        // longitudes
        float step    = 2 * M_PI / (m_num_longitudes);
        float substep = M_PI / (m_num_latitudes * m_num_subdivisions);
        for(int lo {0}; lo < m_num_longitudes; ++lo) {
            float lo_ang = lo * step;
            for(int la {0}; la < m_num_latitudes * m_num_subdivisions; ++la) {
                float la_ang = la * substep;
                m_lines.push_back({spherical_project(la_ang, lo_ang),
                                   spherical_project(la_ang + substep, lo_ang)});
            }
        }
    }

    {
        // latitudes
        float step    = M_PI / (m_num_latitudes);
        float substep = 2 * M_PI / (m_num_longitudes * m_num_subdivisions);
        for(int la {0}; la < m_num_latitudes; ++la) {
            float la_ang = la * step;
            for(int lo {0}; lo < m_num_longitudes * m_num_subdivisions; ++lo) {
                float lo_ang = lo * substep;
                m_lines.push_back({spherical_project(la_ang, lo_ang),
                                   spherical_project(la_ang, lo_ang + substep)});
            }
        }
    }
}
