#pragma once

#include <utility>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace pine {

using Vec2 = glm::vec2;
using Vec3 = glm::vec3;
using Vec4 = glm::vec4;

using Mat2 = glm::mat2;
using Mat3 = glm::mat3;
using Mat4 = glm::mat4;

using Quat = glm::quat;

template <typename T>
float radians(const T& t) {
    return glm::radians(t);
}

template <typename... Args>
Mat4 translate(Args... args) {
    return glm::translate(args...);
}

template <typename... Args>
Mat4 rotate(Args... args) {
    return glm::rotate(args...);
}

template <typename... Args>
Mat4 scale(Args... args) {
    return glm::scale(args...);
}

template <typename... Args>
Mat4 ortho(Args... args) {
    return glm::ortho(args...);
}

template <typename... Args>
Mat4 inverse(Args... args) {
    return glm::inverse(args...);
}

template <typename T>
typename T::value_type* value_ptr(T& t) {
    return glm::value_ptr(t);
}

template <typename T>
const typename T::value_type* value_ptr(const T& t) {
    return glm::value_ptr(t);
}

template <typename T>
typename T::value_type* value_ptr(T&& t) {
    return glm::value_ptr(std::forward(t));
}

} // namespace pine
