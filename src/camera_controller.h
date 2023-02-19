#pragma once
#include "helper.h"

#include <Camera.hpp>
#include <Godot.hpp>
#include <Input.hpp>
#include <MainLoop.hpp>
#include <MeshInstance.hpp>
#include <ShaderMaterial.hpp>
#include <Variant.hpp>
#include <Viewport.hpp>

#include <glm/mat3x3.hpp>

namespace godot {
class CameraController: public Camera {
    GODOT_CLASS(CameraController, Camera)

private:
    Viewport* m_vp {nullptr};
    Input*    m_in {nullptr};

    bool m_focused {true};
    // per milisecond button pressed
    float m_speed;
    float m_roll_speed;
    // per pixel mouse moved
    float   m_pitch_speed;
    float   m_yaw_speed;
    Vector2 m_accumulated_mouse_motion {Vector2::ZERO};

    // radius of glome
    float m_radius;

    float     m_cull_horizon_angl;
    glm::vec3 m_cull_plain_point;
    float     m_cull_plain_param;

    // how to rotate (0, 0, r) to cur cam position
    glm::mat3 m_globe_rotation;

public:
    static void _register_methods();

    CameraController() {}
    ~CameraController() {}

    void _init();
    void _ready();
    void _notification(const int what);
    void _input(Variant event);
    void _process(float delta);

    float     get_radius() const { return m_radius; }
    glm::mat3 get_globe_rotation() const { return m_globe_rotation; }
    glm::mat3 get_globe_rotation_inv() const { return glm::inverse(m_globe_rotation); }

    bool to_cull(glm::vec3 point);
    void set_shader_uniforms(ShaderMaterial* shader);

private:
    void handle_rotation(float delta);
    void handle_movement(float delta);
    void calc_culling_plain();
};

} // namespace godot
