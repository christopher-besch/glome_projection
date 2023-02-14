#pragma once
#include "helper.h"

#include <Godot.hpp>
#include <MeshInstance.hpp>

namespace godot {
class Grid: public MeshInstance {
    GODOT_CLASS(Grid, MeshInstance)

public:
    static void _register_methods();

    Grid() {}
    ~Grid() {}

    void _init();
    void _ready();
};

} // namespace godot
