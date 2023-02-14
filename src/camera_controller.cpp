#include "camera_controller.h"

#include <InputEventMouseMotion.hpp>

using namespace godot;

void CameraController::_register_methods()
{
    register_method("_ready", &godot::CameraController::_ready);
    register_method("_notification", &godot::CameraController::_notification);
    register_method("_process", &godot::CameraController::_process);
    register_method("_input", &godot::CameraController::_input);
}

void CameraController::_init()
{
}

void CameraController::_ready()
{
    m_vp = get_viewport();
    m_in = Input::get_singleton();

    Transform glob_trans = get_global_transform();
    // pls don't scale the camera
    m_facing      = -glob_trans.basis.z;
    m_up          = glob_trans.basis.y;
    m_right       = glob_trans.basis.x;
    m_speed       = 10.0;
    m_roll_speed  = 0.8;
    m_yaw_speed   = 0.1;
    m_pitch_speed = 0.1;
}

void CameraController::_notification(const int what)
{
    if(what == MainLoop::NOTIFICATION_WM_FOCUS_IN)
        m_focused = true;
    if(what == MainLoop::NOTIFICATION_WM_FOCUS_OUT)
        m_focused = false;
}

void CameraController::_input(Variant event)
{
    Ref<InputEventMouseMotion> mouse_event = event;
    if(!mouse_event.is_null())
        m_accumulated_mouse_motion += mouse_event->get_relative();
}

void CameraController::_process(float delta)
{
    if(m_focused) {
        m_in->set_mouse_mode(Input::MOUSE_MODE_CAPTURED);

        handle_rotation(delta);
        handle_movement(delta);
    }
}

void CameraController::handle_rotation(float delta)
{
    // pitch/yaw
    rotate(m_right, -m_accumulated_mouse_motion.y * m_pitch_speed * delta);
    rotate(m_up, -m_accumulated_mouse_motion.x * m_yaw_speed * delta);
    m_accumulated_mouse_motion = Vector2::ZERO;

    // roll
    if(m_in->is_action_pressed("roll_left"))
        rotate(m_facing, -m_roll_speed * delta);
    if(m_in->is_action_pressed("roll_right"))
        rotate(m_facing, m_roll_speed * delta);

    Transform trans = get_global_transform();
    m_facing        = -trans.basis.z;
    m_up            = trans.basis.y;
    m_right         = trans.basis.x;
}

void CameraController::handle_movement(float delta)
{
    Transform trans = get_global_transform();
    Vector3   vel   = Vector3::ZERO;
    if(m_in->is_action_pressed("forwards")) {
        vel += m_facing;
    }
    if(m_in->is_action_pressed("backwards")) {
        vel -= m_facing;
    }
    if(m_in->is_action_pressed("right")) {
        vel += m_right;
    }
    if(m_in->is_action_pressed("left")) {
        vel -= m_right;
    }
    if(m_in->is_action_pressed("up")) {
        vel += m_up;
    }
    if(m_in->is_action_pressed("down")) {
        vel -= m_up;
    }
    vel.normalize();
    trans.origin += vel * m_speed * delta;
    set_global_transform(trans);
}
