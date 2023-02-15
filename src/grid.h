#pragma once
#include "helper.h"

#include <Godot.hpp>
#include <MeshInstance.hpp>
#include <ShaderMaterial.hpp>

#include <utility>
#include <vector>

namespace godot {
class Grid: public MeshInstance {
    GODOT_CLASS(Grid, MeshInstance)

private:
    ShaderMaterial* m_shader {nullptr};

    float m_radius;
    // all from pole to pole
    int m_num_longitudes;
    // like equator
    int m_num_latitudes;
    int m_num_subdivisions;

    float m_cull_horizon_angl;
    float m_cull_plain_param;

    std::vector<std::pair<Vector3, Vector3>> m_lines;
    Vector3                                  m_cam_pos;
    Vector3                                  m_cam_up;
    Vector3                                  m_cam_right;

public:
    static void
        _register_methods();

    Grid() {}
    ~Grid() {}

    void _init();
    void _ready();
    void _process(float delta);

    void set_cam_pos(Vector3 new_pos)
    {
        m_cam_pos = new_pos;
    }

private:
    Vector3 spherical_project(float la_ang, float lo_ang);

    void generate_grid();

    void calc_culling_plain();
    bool to_cull(Vector3 point);
};

} // namespace godot
