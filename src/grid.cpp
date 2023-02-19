#include "grid.h"

#include <vector>

#include <ArrayMesh.hpp>
#include <Mesh.hpp>
#include <MeshDataTool.hpp>
#include <Object.hpp>
#include <PoolArrays.hpp>

using namespace godot;

void Grid::_register_methods()
{
    register_method("_ready", &Grid::_ready);
    register_method("_process", &Grid::_process);
}

void Grid::_init()
{
    m_shader = Object::cast_to<ShaderMaterial>(get_material_override().ptr());

    m_num_longitudes   = 32;
    m_num_latitudes    = 32;
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
    PoolColorArray   colors {};
    for(auto [from, to, color]: m_lines) {
        if(m_camera->to_cull(from) || m_camera->to_cull(to))
            continue;
        verticies.push_back(glm_vec32gd(from));
        colors.push_back(color);
        verticies.push_back(glm_vec32gd(to));
        colors.push_back(color);
    }

    ArrayMesh* arr_mesh = ArrayMesh::_new();
    Array      arrays {};
    arrays.resize(ArrayMesh::ARRAY_MAX);
    arrays[ArrayMesh::ARRAY_VERTEX] = verticies;
    arrays[ArrayMesh::ARRAY_COLOR]  = colors;

    arr_mesh->add_surface_from_arrays(Mesh::PRIMITIVE_LINES, arrays);
    set_mesh(arr_mesh);
}

glm::vec3 Grid::spherical_project(float la_ang, float lo_ang)
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
                m_lines.push_back(std::make_tuple(spherical_project(la_ang, lo_ang),
                                                  spherical_project(la_ang + substep, lo_ang),
                                                  Color {0, 0, 1}));
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
                // equator red
                Color color = m_num_latitudes % 2 == 0 && la == m_num_latitudes / 2 ? Color(1, 0, 0) : Color(0, 0, 1);
                m_lines.push_back(std::make_tuple(spherical_project(la_ang, lo_ang),
                                                  spherical_project(la_ang, lo_ang + substep),
                                                  color));
            }
        }
    }
}
