#pragma once
#include "helper.h"

#include <Godot.hpp>
#include <MeshInstance.hpp>

namespace godot {
class Grid: public MeshInstance {
    GODOT_CLASS(Grid, MeshInstance)

private:
    float m_radius;
    // all from pole to pole
    int   m_num_longitudes;
    int   m_num_longitudinal_steps;
    float m_longitudinal_step_between;
    float m_longitudinal_step;

    // like equator
    int   m_num_latitudes;
    int   m_num_latitudinal_steps;
    float m_latitudinal_step_between;
    float m_latitudinal_step;

public:
    static void
        _register_methods();

    Grid() {}
    ~Grid() {}

    void _init();
    void _ready();

private:
    Vector3 project(float la_ang, float lo_ang);
};

} // namespace godot
