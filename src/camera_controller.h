#pragma once
#include "helper.h"

#include <Camera.hpp>
#include <Godot.hpp>
#include <Input.hpp>
#include <MainLoop.hpp>
#include <Variant.hpp>
#include <Viewport.hpp>

namespace godot {
class CameraController: public Camera {
    GODOT_CLASS(CameraController, Camera)

private:
    Viewport* m_vp {nullptr};
    Input*    m_in;

    bool    m_focused {true};
    Vector3 m_facing;
    Vector3 m_up;
    Vector3 m_right;
    // per milisecond button pressed
    float m_speed;
    float m_roll_speed;
    // per pixel mouse moved
    float   m_pitch_speed;
    float   m_yaw_speed;
    Vector2 m_accumulated_mouse_motion {Vector2::ZERO};

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
