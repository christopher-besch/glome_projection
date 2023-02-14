#include "grid.h"
#include "PoolArrays.hpp"

#include <vector>

#include <ArrayMesh.hpp>
#include <Mesh.hpp>
#include <MeshDataTool.hpp>
#include <Vector3.hpp>

using namespace godot;

void Grid::_register_methods()
{
    register_method("_ready", &Grid::_ready);
}

void Grid::_init()
{
    m_radius = 20;

    m_num_longitudes            = 16;
    m_num_longitudinal_steps    = 32;
    m_longitudinal_step_between = 2 * M_PI / m_num_longitudes;
    m_longitudinal_step         = M_PI / m_num_longitudinal_steps;

    m_num_latitudes            = 8;
    m_num_latitudinal_steps    = 16;
    m_latitudinal_step_between = M_PI / m_num_latitudes;
    m_latitudinal_step         = 2 * M_PI / m_num_latitudinal_steps;
}

void Grid::_ready()
{
    PoolVector3Array verticies {};
    // longitudes
    for(int lo {0}; lo < m_num_longitudes; ++lo) {
        float la_ang, lo_ang;
        lo_ang = lo * m_longitudinal_step_between;
        for(int la {0}; la < m_num_longitudinal_steps; ++la) {
            la_ang = la * m_longitudinal_step;
            verticies.push_back(project(la_ang, lo_ang));
            verticies.push_back(project(la_ang + m_longitudinal_step, lo_ang));
        };
    }

    // latitudes
    for(int la {0}; la < m_num_latitudes; ++la) {
        float la_ang, lo_ang;
        la_ang = la * m_latitudinal_step_between;
        for(int lo {0}; lo < m_num_latitudinal_steps; ++lo) {
            lo_ang = lo * m_latitudinal_step;
            verticies.push_back(project(la_ang, lo_ang));
            verticies.push_back(project(la_ang, lo_ang + m_latitudinal_step));
        }
    }

    ArrayMesh* arr_mesh = ArrayMesh::_new();
    Array      arrays {};
    arrays.resize(ArrayMesh::ARRAY_MAX);
    arrays[ArrayMesh::ARRAY_VERTEX] = verticies;

    arr_mesh->add_surface_from_arrays(Mesh::PRIMITIVE_LINES, arrays);

    set_mesh(arr_mesh);
}

Vector3 Grid::project(float la_ang, float lo_ang)
{
    // prt(la_ang << " " << lo_ang);
    return {m_radius * std::sin(la_ang) * std::cos(lo_ang),
            m_radius * std::sin(la_ang) * std::sin(lo_ang),
            m_radius * std::cos(la_ang)};
}
