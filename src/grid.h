#pragma once
#include "camera_controller.h"
#include "helper.h"

#include <Color.hpp>
#include <Godot.hpp>
#include <MeshInstance.hpp>
#include <ShaderMaterial.hpp>

#include <tuple>
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

    std::vector<std::tuple<glm::vec4, glm::vec4, Color>> m_lines;

public:
    static void
        _register_methods();

    Grid() {}
    ~Grid() {}

    void _init();
    void _ready();
    void _process(float delta);

private:
    glm::vec4 glomar_project_orig(float la_ang, float lo1_ang, float lo2_ang);
    glm::vec4 glomar_project(float la_ang, float lo1_ang, float lo2_ang)
    {
        auto x = glomar_project_orig(la_ang, lo1_ang, lo2_ang);
        return x;
    }

    void generate_grid();
};

} // namespace godot
