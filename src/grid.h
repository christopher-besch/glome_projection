#pragma once
#include "camera_controller.h"
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
    CameraController* m_camera {nullptr};
    ShaderMaterial*   m_shader {nullptr};

    // all from pole to pole
    int m_num_longitudes;
    // like equator
    int m_num_latitudes;
    int m_num_subdivisions;

    std::vector<std::pair<glm::vec3, glm::vec3>> m_lines;

public:
    static void
        _register_methods();

    Grid() {}
    ~Grid() {}

    void _init();
    void _ready();
    void _process(float delta);

private:
    glm::vec3 spherical_project(float la_ang, float lo_ang);

    void generate_grid();
};

} // namespace godot
