#include "camera_controller.h"

#include <InputEventMouseMotion.hpp>
#include <SphereMesh.hpp>
#include <Transform.hpp>

#include <glm/mat3x3.hpp>

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
    m_speed             = 0.1 * M_PI;
    m_roll_speed        = 0.8;
    m_yaw_speed         = 0.1;
    m_pitch_speed       = 0.1;
    m_radius            = 5.0;
    m_cull_horizon_angl = 0.4 * M_PI;

    m_globe_rotation = glm::mat4 {1, 0, 0, 0,
                                  0, 1, 0, 0,
                                  0, 0, 1, 0,
                                  0, 0, 0, 1};
}

void CameraController::_ready()
{
    set_process_priority(0);
    m_vp = get_viewport();
    m_in = Input::get_singleton();
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
        m_accumulated_mouse_motion += gd_vec22glm(mouse_event->get_relative());
}

void CameraController::_process(float delta)
{
    if(m_focused) {
        m_in->set_mouse_mode(Input::MOUSE_MODE_CAPTURED);

        handle_rotation(delta);
        handle_movement(delta);
        calc_culling_plain();
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
    m_accumulated_mouse_motion = {0, 0};

    // roll
    if(m_in->is_action_pressed("roll_left"))
        rotate(facing, -m_roll_speed * delta);
    if(m_in->is_action_pressed("roll_right"))
        rotate(facing, m_roll_speed * delta);
}

void CameraController::handle_movement(float delta)
{
    glm::vec3 vel {0, 0, 0};
    if(m_in->is_action_pressed("forwards")) {
        vel -= glm::vec3 {0, 0, 1};
    }
    if(m_in->is_action_pressed("backwards")) {
        vel += glm::vec3 {0, 0, 1};
    }
    if(m_in->is_action_pressed("right")) {
        vel += glm::vec3 {0, 1, 0};
    }
    if(m_in->is_action_pressed("left")) {
        vel -= glm::vec3 {0, 1, 0};
    }
    if(m_in->is_action_pressed("up")) {
        vel += glm::vec3 {1, 0, 0};
    }
    if(m_in->is_action_pressed("down")) {
        vel -= glm::vec3 {1, 0, 0};
    }

    if(vel != glm::vec3 {0, 0, 0})
        vel = glm::normalize(vel);
    vel *= m_speed * delta;

    Transform trans  = get_global_transform();
    glm::vec3 right  = gd_vec32glm(trans.basis.x);
    glm::vec3 up     = gd_vec32glm(trans.basis.y);
    glm::vec3 facing = gd_vec32glm(trans.basis.z);
    glm::mat3 cam_rotation {up, right, facing};

    // TODO: maybe to be transposed
    glm::mat4 cam_mat_rotated = m_globe_rotation *
                                glm::mat4 {cam_rotation[0][0], cam_rotation[1][0], cam_rotation[2][0], 0,
                                           cam_rotation[0][1], cam_rotation[1][1], cam_rotation[2][1], 0,
                                           cam_rotation[0][2], cam_rotation[1][2], cam_rotation[2][2], 0,
                                           0, 0, 0, 1};
    // glm::mat4 cam_mat_rotated = m_globe_rotation *
    //                             glm::mat4 {cam_rotation[0][0], cam_rotation[0][1], cam_rotation[0][2], 0,
    //                                        cam_rotation[1][0], cam_rotation[1][1], cam_rotation[1][2], 0,
    //                                        cam_rotation[2][0], cam_rotation[2][1], cam_rotation[2][2], 0,
    //                                        0, 0, 0, 1};

    // rotate on xw plane
    glm::mat4 rotation_mat = {std::cos(vel.x), 0, 0, -std::sin(vel.x),
                              0, 1, 0, 0,
                              0, 0, 1, 0,
                              std::sin(vel.x), 0, 0, std::cos(vel.x)};
    // rotate on yw plane
    rotation_mat = glm::mat4 {1, 0, 0, 0,
                              0, std::cos(vel.y), 0, -std::sin(vel.y),
                              0, 0, 1, 0,
                              0, std::sin(vel.y), 0, std::cos(vel.y)} *
                   rotation_mat;
    // rotate on zw plane
    rotation_mat = glm::mat4 {1, 0, 0, 0,
                              0, 1, 0, 0,
                              0, 0, std::cos(vel.z), -std::sin(vel.z),
                              0, 0, std::sin(vel.z), std::cos(vel.z)} *
                   rotation_mat;

    m_globe_rotation = cam_mat_rotated * rotation_mat * glm::inverse(cam_mat_rotated) * m_globe_rotation;

    // TODO: remove

    glm::vec4 vert {0, 0, 0, m_radius};
    glm::mat4 globe_rotation_inv = glm::inverse(m_globe_rotation);
    // remove rotation
    vert = globe_rotation_inv * vert;
    prt(glm::length(vert));

    glm::vec4 cam_pos = glm::vec4(0, 0, 0, m_radius);

    // project on tangential plane
    float d = dot(cam_pos, cam_pos);
    float r = d / (dot(vert, cam_pos));
    vert    = r * vert;
    prt(vert.x << " " << vert.y << " " << vert.z << " " << vert.w);

    // translate back to origin -> w = 0
}

void CameraController::calc_culling_plain()
{
    m_cull_plain_point = m_globe_rotation * glm::vec4 {0, 0, 0, m_radius};
    // scale vector to pos down
    glm::vec4 other_cull_plain_point = std::cos(m_cull_horizon_angl) * m_cull_plain_point;
    // not required to normalize because done in both functions
    m_cull_plain_param = glm::dot(m_cull_plain_point, other_cull_plain_point);
}

bool CameraController::to_cull(glm::vec4 point)
{
    return glm::dot(m_cull_plain_point, point) - m_cull_plain_param < 0;
}

void CameraController::set_shader_uniforms(ShaderMaterial* shader)
{
    shader->set_shader_param("u_radius", m_radius);
    glm::mat4 globe_rotation_inv = glm::inverse(m_globe_rotation);
    shader->set_shader_param("u_globe_rotation_inv_0", glm_vec42gd(globe_rotation_inv[0]));
    shader->set_shader_param("u_globe_rotation_inv_1", glm_vec42gd(globe_rotation_inv[1]));
    shader->set_shader_param("u_globe_rotation_inv_2", glm_vec42gd(globe_rotation_inv[2]));
    shader->set_shader_param("u_globe_rotation_inv_3", glm_vec42gd(globe_rotation_inv[3]));
}
