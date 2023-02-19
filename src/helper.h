#pragma once

#include <Basis.hpp>
#include <Vector2.hpp>

#include <glm/mat3x3.hpp>

#include <sstream>

#define prt(x) Godot::print(String((std::stringstream() << x).str().c_str()))

inline glm::mat3x3 gd_basis2glm(godot::Basis basis)
{
    godot::Vector3 x = basis.x;
    godot::Vector3 y = basis.y;
    godot::Vector3 z = basis.z;
    return glm::mat3x3 {x.x, y.x, z.x,
                        x.y, y.y, z.y,
                        x.z, y.z, z.z};
}

inline glm::vec3 gd_vec32glm(godot::Vector3 vec)
{
    return glm::vec3 {vec.x, vec.y, vec.z};
}

inline glm::vec2 gd_vec22glm(godot::Vector2 vec)
{
    return glm::vec2 {vec.x, vec.y};
}

inline godot::Vector3 glm_vec32gd(glm::vec3 vec)
{
    return godot::Vector3 {vec.x, vec.y, vec.z};
}

inline godot::Basis glm_mat3x32gd(glm::mat3x3 mat)
{
    return {glm_vec32gd(mat * glm::vec3 {1, 0, 0}),
            glm_vec32gd(mat * glm::vec3 {0, 1, 0}),
            glm_vec32gd(mat * glm::vec3 {0, 0, 1})};
}
