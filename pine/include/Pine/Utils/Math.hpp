#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Pine
{

typedef glm::vec2 Vec2;
typedef glm::vec3 Vec3;
typedef glm::vec4 Vec4;

typedef glm::mat2 Mat2;
typedef glm::mat3 Mat3;
typedef glm::mat4 Mat4;

typedef glm::quat Quat;

template <typename T>
float Radians(const T& t)
{
    return glm::radians(t);
}

template <typename... Args>
Mat4 Translate(Args... args)
{
    return glm::translate(args...);
}

template <typename... Args>
Mat4 Rotate(Args... args)
{
    return glm::rotate(args...);
}

template <typename... Args>
Mat4 Scale(Args... args)
{
    return glm::scale(args...);
}

template <typename... Args>
Mat4 Ortho(Args... args)
{
    return glm::ortho(args...);
}

template <typename... Args>
Mat4 Inverse(Args... args)
{
    return glm::inverse(args...);
}

template <typename T>
typename T::value_type* ValuePtr(T& t)
{
    return glm::value_ptr(t);
}

template <typename T>
const typename T::value_type* ValuePtr(const T& t)
{
    return glm::value_ptr(t);
}

} // namespace Pine
