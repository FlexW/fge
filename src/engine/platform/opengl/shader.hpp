#pragma once

#include "gl.hpp"
#include "graphic/shader.hpp"
#include "log/log.hpp"
#include "math/math.hpp"
#include "std.hpp"

namespace Fge::Gl
{

/**
 * @brief General purpose Shader class
 *
 * Handles creation and linking of shaders.
 */
class Shader : public Fge::Shader
{
public:
  /**
   * Creates a new Shader
   *
   * @param vertexShaderProgram Path to vertex shader
   * @param fragmentShaderProgram Path to fragment shader
   * @param geometryShaderProgram Path to geometry shader. This is optional
   */
  Shader(const std::string &vertexShaderProgram,
         const std::string &fragmentShaderProgram,
         const std::string &geometryShaderProgram = "");

  Shader(const std::string &             vertexShaderProgram,
         const std::string &             fragmentShaderProgram,
         const std::vector<const char *> output_names);

  uint32_t get_id() const override { return id; }

  /**
   * Binds the shader for rendering
   */
  void bind() const override { glUseProgram(id); }

  /**
   * Unbinds the shader for rendering
   */
  void unbind() const override { glUseProgram(0); }

  /**
   * Sets a bool variable for the shader
   *
   * @param name Name of the variable
   * @param value Value
   */
  void set_uniform(const std::string &name, bool value) override
  {
    glUniform1i(get_uniform_location(name), (int)value);
  }

  /**
   * Sets a int variable for the shader
   *
   * @param name Name of the variable
   * @param value Value
   */
  void set_uniform(const std::string &name, int32_t value) override
  {
    glUniform1i(get_uniform_location(name), value);
  }

  template <std::size_t SIZE>
  void set_uniform(const std::string &name, std::array<float, SIZE> &values)
  {
    glUniform1fv(get_uniform_location(name),
                 values.size(),
                 reinterpret_cast<GLfloat *>(&values[0]));
  }

  template <std::size_t SIZE>
  void set_uniform(const std::string &name, std::array<int, SIZE> &values)
  {
    glUniform1iv(get_uniform_location(name),
                 values.size(),
                 reinterpret_cast<GLint *>(&values[0]));
  }

  /**
   * Sets a float variable for the shader
   *
   * @param name Name of the variable
   * @param value Value
   */
  void set_uniform(const std::string &name, float value) override
  {
    glUniform1f(get_uniform_location(name), static_cast<GLfloat>(value));
  }

  /**
   * Sets a vec2 variable for the shader
   *
   * @param name Name of the variable
   * @param value Value
   */
  void set_uniform(const std::string &name, const glm::vec2 &value) override
  {
    glUniform2fv(get_uniform_location(name), 1, &value[0]);
  }

  /**
   * Sets a ivec2 variable for the shader
   *
   * @param name Name of the variable
   * @param value Value
   */
  void set_uniform(const std::string &name, const glm::ivec2 &value) override
  {
    glUniform2iv(get_uniform_location(name), 1, &value[0]);
  }

  /**
   * Sets a vec3 variable for the shader
   *
   * @param name Name of the variable
   * @param value Value
   */
  void set_uniform(const std::string &name, const glm::vec3 &value) override
  {
    glUniform3fv(get_uniform_location(name), 1, &value[0]);
  }

  /**
   * Sets a vec4 variable for the shader
   *
   * @param name Name of the variable
   * @param value Value
   */
  void set_uniform(const std::string &name, const glm::vec4 &value) override
  {
    glUniform4fv(get_uniform_location(name), 1, &value[0]);
  }

  /**
   * Sets a mat2 variable for the shader
   *
   * @param name Name of the variable
   * @param value Value
   */
  void set_uniform(const std::string &name, const glm::mat2 &value) override
  {
    glUniformMatrix2fv(get_uniform_location(name), 1, GL_FALSE, &value[0][0]);
  }

  /**
   * Sets a mat3 variable for the shader
   *
   * @param name Name of the variable
   * @param value Value
   */
  void set_uniform(const std::string &name, const glm::mat3 &value) override
  {
    glUniformMatrix3fv(get_uniform_location(name), 1, GL_FALSE, &value[0][0]);
  }

  /**
   * Sets a mat4 variable for the shader
   *
   * @param name Name of the variable
   * @param value Value
   */
  void set_uniform(const std::string &name, const glm::mat4 &value) override
  {
    glUniformMatrix4fv(get_uniform_location(name),
                       1,
                       GL_FALSE,
                       glm::value_ptr(value));
  }

  void set_uniform(const std::string &           name,
                   const std::vector<glm::mat4> &value) override
  {
    glUniformMatrix4fv(get_uniform_location(name),
                       value.size(),
                       GL_FALSE,
                       glm::value_ptr(value[0]));
  }

  /**
   * Get the location of a attribute
   *
   * @param name Name of the attribute
   *
   * @return Attribute location
   */
  GLuint get_attrib_location(const std::string &name) const
  {
    return glGetAttribLocation(id, name.c_str());
  }

  int get_uniform_location(const std::string &name)
  {
    if (uniform_location_cache.find(name) != uniform_location_cache.end())
    {
      return uniform_location_cache[name];
    }
    auto location = glGetUniformLocation(id, name.c_str());

    if (location == -1)
    {
      warning("Shader", "Uniform {} doesn't exist!", name);
    }

    uniform_location_cache[name] = location;
    return location;
  }

private:
  uint32_t                             id = 0;
  std::unordered_map<std::string, int32_t> uniform_location_cache;

  enum class ShaderType
  {
    PROGRAMM,
    FRAGMENT,
    VERTEX,
    GEOMETRY
  };

  std::string shader_type_to_string(ShaderType shader_type) const;

  void check_for_compile_errors(const std::string &name,
                                const GLuint       shader_id,
                                const ShaderType   shader_type);

  GLuint compile_shader(const std::string &program, GLenum type);

  void check_shader_compile_errors(GLuint id, GLenum shader_type);

  void check_for_program_compile_errors(GLuint program_id);
};

} // namespace Fge::Gl
