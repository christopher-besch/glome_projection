#pragma once
#include "helper.h"

#include <Camera.hpp>
#include <Godot.hpp>
#include <Input.hpp>
#include <MainLoop.hpp>
#include <MeshInstance.hpp>
#include <Variant.hpp>
#include <Viewport.hpp>

#include <glm/mat3x3.hpp>

namespace godot {
class CameraController: public Camera {
    GODOT_CLASS(CameraController, Camera)

private:
    Viewport* m_vp {nullptr};
    Input*    m_in;

    bool m_focused {true};
    // per milisecond button pressed
    float m_speed;
    float m_roll_speed;
    // per pixel mouse moved
    float   m_pitch_speed;
    float   m_yaw_speed;
    Vector2 m_accumulated_mouse_motion {Vector2::ZERO};

    MeshInstance* m_debug_point1 {nullptr};
    MeshInstance* m_debug_point2 {nullptr};

    glm::mat3x3 m_globe_rotation;
    glm::mat3x3 m_globe_rotation_inv;

    glm::mat2x2 m_cam_rotation;

public:
    static void _register_methods();

    CameraController() {}
    ~CameraController() {}

    void _init();
    void _ready();
    void _notification(const int what);
    void _input(Variant event);
    void _process(float delta);

private:
    void handle_rotation(float delta);
    void handle_movement(float delta);
};

} // namespace godot
