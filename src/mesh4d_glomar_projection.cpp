#include "mesh4d_glomar_projection.h"

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
    Ref<Mesh>    mesh = get_mesh();
    MeshDataTool mdt  = MeshDataTool();
    mdt.create_from_surface(mesh, 0);
    // for(int i = 0; i < mdt.get_vertex_count(); ++i) {
    //     Vector3 vertex = mdt.get_vertex(i);
    //     vertex += Vector3(1.0, 0.0, 0.0);
    //     mdt.set_vertex(i, vertex);
    // }
    mdt.commit_to_surface(mesh);
}
