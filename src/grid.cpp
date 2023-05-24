#include "grid.h"

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

    m_num_longitudes   = 16;
    m_num_latitudes    = 16;
    m_num_subdivisions = 8;
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
    PoolVector3Array posxyz {};
    PoolVector2Array posw {};
    PoolColorArray   colors {};
    for(auto [from, to, color]: m_lines) {
        if(m_camera->to_cull(from) || m_camera->to_cull(to))
            continue;
        posxyz.push_back({from.x, from.y, from.z});
        posw.push_back({from.w, 0});
        colors.push_back(color);

        posxyz.push_back({to.x, to.y, to.z});
        posw.push_back({to.w, 0});
        colors.push_back(color);
    }

    ArrayMesh* arr_mesh = ArrayMesh::_new();
    Array      arrays {};
    arrays.resize(ArrayMesh::ARRAY_MAX);
    arrays[ArrayMesh::ARRAY_VERTEX] = posxyz;
    arrays[ArrayMesh::ARRAY_TEX_UV] = posw;
    arrays[ArrayMesh::ARRAY_COLOR]  = colors;

    arr_mesh->add_surface_from_arrays(Mesh::PRIMITIVE_LINES, arrays);
    set_mesh(arr_mesh);
}

glm::vec4 Grid::glomar_project_orig(float la_ang, float lo1_ang, float lo2_ang)
{
    float r = m_camera->get_radius();
    // return {r * std::cos(la_ang) * std::cos(lo1_ang) * std::cos(lo2_ang),
    //         r * std::cos(la_ang) * std::cos(lo1_ang) * std::sin(lo2_ang),
    //         r * std::sin(la_ang) * std::sin(lo1_ang),
    //         r * std::sin(la_ang)};
    return {-r * std::sin(la_ang) * std::cos(lo1_ang) * std::cos(lo2_ang),
            -r * std::sin(la_ang) * std::cos(lo1_ang) * std::sin(lo2_ang),
            -r * std::sin(la_ang) * std::sin(lo1_ang),
            r * std::cos(la_ang)};
}

void Grid::generate_grid()
{
    {
        // longitudes
        float step    = 2 * M_PI / (m_num_longitudes);
        float substep = M_PI / (m_num_latitudes * m_num_subdivisions);
        for(int lo1 {0}; lo1 < m_num_longitudes; ++lo1) {
            float lo1_ang = lo1 * step;
            for(int lo2 {0}; lo2 < m_num_longitudes; ++lo2) {
                float lo2_ang = lo2 * step;
                for(int la {0}; la < m_num_latitudes * m_num_subdivisions; ++la) {
                    float la_ang = la * substep;
                    m_lines.push_back(std::make_tuple(glomar_project(la_ang, lo1_ang, lo2_ang),
                                                      glomar_project(la_ang + substep, lo1_ang, lo2_ang),
                                                      Color {0, 1, 1}));
                }
            }
        }
    }

    {
        // latitudes
        float step    = M_PI / (m_num_latitudes);
        float substep = 2 * M_PI / (m_num_longitudes * m_num_subdivisions);
        for(int la {0}; la < m_num_latitudes; ++la) {
            float la_ang = la * step;
            // equator red
            Color color = m_num_latitudes % 2 == 0 && la == m_num_latitudes / 2 ? Color(1, 0, 0) : Color(0, 0, 1);
            for(int lo1 {0}; lo1 < 2 * m_num_longitudes; ++lo1) {
                float lo1_ang = lo1 * step;
                for(int lo2 {0}; lo2 < m_num_longitudes * m_num_subdivisions; ++lo2) {
                    float lo2_ang = lo2 * substep;
                    m_lines.push_back(std::make_tuple(glomar_project(la_ang, lo1_ang, lo2_ang),
                                                      glomar_project(la_ang, lo1_ang, lo2_ang + substep),
                                                      color));
                }
            }
            for(int lo2 {0}; lo2 < m_num_longitudes; ++lo2) {
                float lo2_ang = lo2 * step;
                for(int lo1 {0}; lo1 < m_num_longitudes * m_num_subdivisions; ++lo1) {
                    float lo1_ang = lo1 * substep;
                    m_lines.push_back(std::make_tuple(glomar_project(la_ang, lo1_ang, lo2_ang),
                                                      glomar_project(la_ang, lo1_ang + substep, lo2_ang),
                                                      color));
                }
            }
        }
    }
}
