#pragma once

#include "math/math.hpp"
#include "std.hpp"

namespace Fge
{

class Shader
{
public:
  virtual ~Shader() = default;

  virtual uint32_t get_id() const = 0;

  virtual void bind() const = 0;

  virtual void unbind() const = 0;

  virtual void set_uniform(const std::string &name, bool value) = 0;

  virtual void set_uniform(const std::string &name, int32_t value) = 0;

  virtual void set_uniform(const std::string &name, const glm::mat4 &value) = 0;

  virtual void set_uniform(const std::string &name, float value) = 0;

  virtual void set_uniform(const std::string &name, const glm::vec2 &value) = 0;

  virtual void set_uniform(const std::string &name,
                           const glm::ivec2 & value) = 0;

  virtual void set_uniform(const std::string &name, const glm::vec3 &value) = 0;

  virtual void set_uniform(const std::string &name, const glm::vec4 &value) = 0;

  virtual void set_uniform(const std::string &name, const glm::mat2 &value) = 0;

  virtual void set_uniform(const std::string &name, const glm::mat3 &value) = 0;

  virtual void set_uniform(const std::string &           name,
                           const std::vector<glm::mat4> &value) = 0;
};

} // namespace Fge
