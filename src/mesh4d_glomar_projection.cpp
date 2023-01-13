#include "mesh4d_glomar_projection.h"
#include "PoolArrays.hpp"

#include <ArrayMesh.hpp>
#include <Mesh.hpp>
#include <MeshDataTool.hpp>
#include <Vector3.hpp>

using namespace godot;

void Mesh4DGlomarProjection::_register_methods()
{
    register_method("_ready", &Mesh4DGlomarProjection::_ready);
}

Mesh4DGlomarProjection::Mesh4DGlomarProjection() {}
Mesh4DGlomarProjection::~Mesh4DGlomarProjection() {}

void Mesh4DGlomarProjection::_init() {}

void Mesh4DGlomarProjection::_ready()
{
    Godot::print("hi");
    PoolVector3Array verticies {};
    verticies.push_back(Vector3 {3, 0, 0});
    verticies.push_back(Vector3 {0, 0, 3});
    verticies.push_back(Vector3 {0, 0, 0});

    ArrayMesh* arr_mesh = ArrayMesh::_new();
    Array      arrays {};
    arrays.resize(ArrayMesh::ARRAY_MAX);
    arrays[ArrayMesh::ARRAY_VERTEX] = verticies;

    arr_mesh->add_surface_from_arrays(Mesh::PRIMITIVE_TRIANGLES, arrays);
    set_mesh(arr_mesh);
}
