#include "shader.hpp"
#include "util/assert.hpp"

namespace Fge::Gl
{

Shader::Shader(const std::string &             vertex_shader_program,
               const std::string &             fragment_shader_program,
               const std::vector<const char *> output_names)
{
  auto vertex_shader_id =
      compile_shader(vertex_shader_program, GL_VERTEX_SHADER);
  auto fragment_shader_id =
      compile_shader(fragment_shader_program, GL_FRAGMENT_SHADER);

  id = glCreateProgram();
  trace("Shader", "Created shader with id: {}", id);
  glAttachShader(id, vertex_shader_id);
  glAttachShader(id, fragment_shader_id);

  glTransformFeedbackVaryings(id,
                              output_names.size(),
                              output_names.data(),
                              GL_SEPARATE_ATTRIBS);

  glLinkProgram(id);

  check_for_program_compile_errors(id);

  glDeleteShader(vertex_shader_id);
  glDeleteShader(fragment_shader_id);
}

Shader::Shader(const std::string &vertexShaderProgram,
               const std::string &fragmentShaderProgram,
               const std::string &geometryShaderProgram)
{
  unsigned int vertexShaderId = 0, fragmentShaderId = 0, geometryShaderId = 0;
  vertexShaderId            = glCreateShader(GL_VERTEX_SHADER);
  auto vertexShaderCodeCStr = vertexShaderProgram.c_str();
  glShaderSource(vertexShaderId, 1, &vertexShaderCodeCStr, nullptr);
  glCompileShader(vertexShaderId);
  check_for_compile_errors("", vertexShaderId, ShaderType::VERTEX);

  fragmentShaderId            = glCreateShader(GL_FRAGMENT_SHADER);
  auto fragmentShaderCodeCStr = fragmentShaderProgram.c_str();
  glShaderSource(fragmentShaderId, 1, &fragmentShaderCodeCStr, nullptr);
  glCompileShader(fragmentShaderId);
  check_for_compile_errors("", fragmentShaderId, ShaderType::FRAGMENT);

  id = glCreateProgram();
  trace("Shader", "Created shader with id: {}", id);
  glAttachShader(id, vertexShaderId);
  glAttachShader(id, fragmentShaderId);

  if (!geometryShaderProgram.empty())
  {
    geometryShaderId            = glCreateShader(GL_GEOMETRY_SHADER);
    auto geometryShaderCodeCStr = geometryShaderProgram.c_str();
    glShaderSource(geometryShaderId, 1, &geometryShaderCodeCStr, nullptr);
    glCompileShader(geometryShaderId);
    check_for_compile_errors("", geometryShaderId, ShaderType::GEOMETRY);
    glAttachShader(id, geometryShaderId);
  }

  glLinkProgram(id);
  check_for_compile_errors("", id, ShaderType::PROGRAMM);

  glDeleteShader(vertexShaderId);
  glDeleteShader(fragmentShaderId);
}

Shader::~Shader()
{
  trace("Shader", "Delete shader with id: {}", id);
  glDeleteProgram(id);
}

GLuint Shader::compile_shader(const std::string &program, GLenum type)
{
  GLuint id = 0;

  id             = glCreateShader(type);
  auto code_cstr = program.c_str();
  glShaderSource(id, 1, &code_cstr, nullptr);
  glCompileShader(id);

  check_shader_compile_errors(id, type);

  return id;
}

void Shader::check_shader_compile_errors(GLuint id, GLenum shader_type)
{
  GLint     success;
  const int info_log_size = 1024;
  GLchar    info_log[info_log_size];

  std::string shader_type_name;
  switch (shader_type)
  {
  case GL_VERTEX_SHADER:
  {
    shader_type_name = "Vertex";
  }
  break;

  case GL_FRAGMENT_SHADER:
  {
    shader_type_name = "Fragment";
  }
  break;

  default:
    FGE_FAIL("No such shader type");
  }

  glGetShaderiv(id, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(id, info_log_size, nullptr, info_log);

    std::string message = shader_type_name + " Shader Error:\n" + info_log;
    throw std::runtime_error(message);
  }
}

void Shader::check_for_program_compile_errors(GLuint program_id)
{
  GLint     success;
  const int info_log_size = 1024;
  GLchar    info_log[info_log_size];

  glGetProgramiv(program_id, GL_LINK_STATUS, &success);
  if (!success)
  {
    glGetProgramInfoLog(program_id, info_log_size, nullptr, info_log);

    throw std::runtime_error(std::string("Shader Program Error:\n") + info_log);
  }
}

void Shader::check_for_compile_errors(const std::string &name,
                                      const GLuint       shaderId,
                                      const ShaderType   shaderType)
{
  GLint     success;
  const int infoLogSize = 1024;
  GLchar    infoLog[infoLogSize];

  switch (shaderType)
  {
  case ShaderType::VERTEX:
  case ShaderType::FRAGMENT:
  case ShaderType::GEOMETRY:
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
    if (!success)
    {
      glGetShaderInfoLog(shaderId, infoLogSize, nullptr, infoLog);

      std::string message = shader_type_to_string(shaderType) +
                            " Shader Error " + "in " + name + ":\n" + infoLog;
      throw std::runtime_error(message);
    }
    return;

  case ShaderType::PROGRAMM:
    glGetProgramiv(shaderId, GL_LINK_STATUS, &success);
    if (!success)
    {
      glGetProgramInfoLog(shaderId, infoLogSize, nullptr, infoLog);

      std::string message = shader_type_to_string(shaderType) +
                            " Shader Error" + "in " + name + ":\n" + infoLog;
      throw std::runtime_error(message);
    }
    break;

  default:
    FGE_FAIL("No such program type");
  }
}

std::string Shader::shader_type_to_string(ShaderType shaderType) const
{
  switch (shaderType)
  {
  case ShaderType::VERTEX:
    return "Vertex";
  case ShaderType::FRAGMENT:
    return "Fragment";
  case ShaderType::PROGRAMM:
    return "Programm";
  case ShaderType::GEOMETRY:
    return "Geometry";
  default:
    FGE_FAIL("No such shader type");
  }
}

} // namespace Fge::Gl
