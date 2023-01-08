#pragma once

#include <Godot.hpp>
#include <MeshInstance.hpp>

namespace godot {
class Mesh4DGlomarProjection: public MeshInstance {
    GODOT_CLASS(Mesh4DGlomarProjection, MeshInstance)

public:
    static void _register_methods();

    Mesh4DGlomarProjection();
    ~Mesh4DGlomarProjection();

    void _init();
    void _ready();
};

} // namespace godot
