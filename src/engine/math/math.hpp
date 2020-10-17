#pragma once

#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/quaternion_trigonometric.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/string_cast.hpp>

const glm::vec3 ZERO_VEC3   = glm::vec3(0.0f);
const glm::vec3 UP_VEC      = glm::vec3(0.0f, 1.0f, 0.0f);
const glm::vec3 FORWARD_VEC = glm::vec3(0.0f, 0.0f, 1.0f);
const glm::vec3 RIGHT_VEC   = glm::vec3(1.0f, 0.0f, 0.0f);

// template <typename GLMType,
//           typename = decltype(glm::to_string(std::declval<GLMType>()))>
// std::ostream &operator<<(std::ostream &out, const GLMType &g)
// {
//   return out << glm::to_string(g);
// }
