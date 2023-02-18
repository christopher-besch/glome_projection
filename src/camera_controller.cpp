#include "camera_controller.h"

#include <InputEventMouseMotion.hpp>
#include <SphereMesh.hpp>

#include <glm/mat2x2.hpp>
#include <glm/mat3x2.hpp>
#include <glm/mat3x3.hpp>
#include <glm/vec3.hpp>

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
    m_speed       = 10.0;
    m_roll_speed  = 0.8;
    m_yaw_speed   = 0.1;
    m_pitch_speed = 0.1;
    m_radius      = 5.0;

    m_globe_rotation = glm::mat3x3 {1, 0, 0,
                                    0, 1, 0,
                                    0, 0, 1};
}

void CameraController::_ready()
{
    m_vp = get_viewport();
    m_in = Input::get_singleton();

    Transform glob_trans = get_global_transform();

    // TODO: remove
    m_debug_point1     = MeshInstance::_new();
    SphereMesh* sphere = SphereMesh::_new();
    m_debug_point1->set_mesh(sphere);
    m_debug_point1->set_transform(m_debug_point1->get_transform().scaled(Vector3(0.1, 0.1, 0.1)));
    get_node("../Grid")->add_child(m_debug_point1, true);

    m_debug_point2 = MeshInstance::_new();
    m_debug_point2->set_mesh(sphere);
    m_debug_point2->set_transform(m_debug_point2->get_transform().scaled(Vector3(0.1, 0.1, 0.1)));
    get_node("../Grid")->add_child(m_debug_point2, true);
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
    Transform trans  = get_global_transform();
    Vector3   right  = trans.basis.x;
    Vector3   up     = trans.basis.y;
    Vector3   facing = -trans.basis.z;

    // pitch/yaw
    rotate(right, -m_accumulated_mouse_motion.y * m_pitch_speed * delta);
    rotate(up, -m_accumulated_mouse_motion.x * m_yaw_speed * delta);
    m_accumulated_mouse_motion = Vector2::ZERO;

    // roll
    if(m_in->is_action_pressed("roll_left"))
        rotate(facing, -m_roll_speed * delta);
    if(m_in->is_action_pressed("roll_right"))
        rotate(facing, m_roll_speed * delta);
}

void CameraController::handle_movement(float delta)
{
    Transform trans = get_global_transform();
    Vector3   vel   = Vector3::ZERO;
    if(m_in->is_action_pressed("forwards")) {
        vel -= trans.basis.z;
    }
    if(m_in->is_action_pressed("backwards")) {
        vel += trans.basis.z;
    }
    if(m_in->is_action_pressed("right")) {
        vel += trans.basis.x;
    }
    if(m_in->is_action_pressed("left")) {
        vel -= trans.basis.x;
    }
    if(m_in->is_action_pressed("up")) {
        vel += trans.basis.y;
    }
    if(m_in->is_action_pressed("down")) {
        vel -= trans.basis.y;
    }
    vel.normalize();
    trans.origin += vel * m_speed * delta;
    set_global_transform(trans);

    // TODO: replace part above
    trans           = get_transform();
    glm::vec3 right = gd_vec32glm(trans.basis.x);
    glm::vec3 up    = gd_vec32glm(trans.basis.y);
    glm::mat2 cam_rotation {right[0], up[0],
                            right[1], up[1]};

    glm::mat3 cam_mat_rotated = m_globe_rotation *
                                glm::mat3 {cam_rotation[0][0], cam_rotation[0][1], 0,
                                           cam_rotation[1][0], cam_rotation[1][1], 0,
                                           0, 0, 1};

    float phi1 = 0;
    if(m_in->is_action_pressed("debug_forwards"))
        phi1 = 0.2 * M_PI * delta;
    if(m_in->is_action_pressed("debug_backwards"))
        phi1 = -0.2 * M_PI * delta;
    // rotate on xz plane
    glm::mat3x3 rotation_mat = {std::cos(phi1), 0, std::sin(phi1),
                                0, 1, 0,
                                -std::sin(phi1), 0, std::cos(phi1)};

    float phi2 = 0;
    // rotate on yz plane
    rotation_mat = glm::mat3 {1, 0, 0,
                              0, std::cos(phi2), -std::sin(phi2),
                              0, std::sin(phi2), std::cos(phi2)} *
                   rotation_mat;

    m_globe_rotation = cam_mat_rotated * rotation_mat * glm::inverse(cam_mat_rotated) * m_globe_rotation;

    // TODO: remove
    glm::vec3 new_pos = m_globe_rotation * glm::vec3 {0.0, 0.0, m_radius};

    Transform point1_trans = m_debug_point1->get_global_transform();
    point1_trans.set_origin(glm_vec32gd(new_pos));
    m_debug_point1->set_global_transform(point1_trans);

    glm::vec3 pointing_to  = new_pos + cam_mat_rotated * glm::vec3 {-3, 0, 0};
    Transform point2_trans = m_debug_point1->get_global_transform();
    point2_trans.set_origin(glm_vec32gd(pointing_to));
    m_debug_point2->set_global_transform(point2_trans);
}
