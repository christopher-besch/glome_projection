#pragma once

#include <Basis.hpp>
#include <Color.hpp>
#include <Godot.hpp>
#include <Vector2.hpp>

#include <glm/mat3x3.hpp>

#include <sstream>

#define prt(x) godot::Godot::print(godot::String((std::stringstream() << x).str().c_str()))

template<typename T>
inline void pretty_prt(T x);
template<>
inline void pretty_prt(glm::vec3 vec)
{
    prt(vec.x << " " << vec.y << " " << vec.z);
}
template<>
inline void pretty_prt(glm::vec4 vec)
{
    prt(vec.x << " " << vec.y << " " << vec.z << " " << vec.w);
}
template<>
inline void pretty_prt(glm::mat3 mat)
{
    pretty_prt(glm::vec3 {mat[0][0], mat[1][0], mat[2][0]});
    pretty_prt(glm::vec3 {mat[0][1], mat[1][1], mat[2][1]});
    pretty_prt(glm::vec3 {mat[0][2], mat[1][2], mat[2][2]});
}
template<>
inline void pretty_prt(glm::mat4 mat)
{
    pretty_prt(glm::vec4 {mat[0][0], mat[1][0], mat[2][0], mat[3][0]});
    pretty_prt(glm::vec4 {mat[0][1], mat[1][1], mat[2][1], mat[3][1]});
    pretty_prt(glm::vec4 {mat[0][2], mat[1][2], mat[2][2], mat[3][2]});
    pretty_prt(glm::vec4 {mat[0][3], mat[1][3], mat[2][3], mat[3][3]});
}

template<typename GodotType, typename GlmType>
inline GlmType gd2glm(GodotType);
template<>
inline glm::vec3 gd2glm(godot::Vector3 vec)
{
    return {vec.x, vec.y, vec.z};
}
template<>
inline glm::vec2 gd2glm(godot::Vector2 vec)
{
    return {vec.x, vec.y};
}
template<>
inline glm::mat3x3 gd2glm(godot::Basis basis)
{
    godot::Vector3 x = basis.x;
    godot::Vector3 y = basis.y;
    godot::Vector3 z = basis.z;
    return {x.x, y.x, z.x,
            x.y, y.y, z.y,
            x.z, y.z, z.z};
}

template<typename GlmType, typename GodotType>
inline GodotType glm2gd(GlmType);
template<>
inline godot::Vector3 glm2gd(glm::vec3 vec)
{
    return {vec.x, vec.y, vec.z};
}
template<>
inline godot::Color glm2gd(glm::vec4 vec)
{
    return {vec.x, vec.y, vec.z, vec.w};
}
template<>
inline godot::Basis glm2gd(glm::mat3x3 mat)
{
    return {glm2gd<glm::vec3, godot::Vector3>(mat[0]),
            glm2gd<glm::vec3, godot::Vector3>(mat[1]),
            glm2gd<glm::vec3, godot::Vector3>(mat[2])};
}

inline void test()
{
    auto t = godot::Vector3 {1, 2, 3};
    auto i = gd2glm<godot::Vector3, glm::vec3>(t);
}
