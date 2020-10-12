#include "util.hpp"
#include "std.hpp"

namespace Fge::Gl
{

void dump_info()
{
  const GLubyte *renderer     = glGetString(GL_RENDERER);
  const GLubyte *vendor       = glGetString(GL_VENDOR);
  const GLubyte *version      = glGetString(GL_VERSION);
  const GLubyte *glsl_version = glGetString(GL_SHADING_LANGUAGE_VERSION);

  GLint major, minor, samples, sampleBuffers;
  glGetIntegerv(GL_MAJOR_VERSION, &major);
  glGetIntegerv(GL_MINOR_VERSION, &minor);
  glGetIntegerv(GL_SAMPLES, &samples);
  glGetIntegerv(GL_SAMPLE_BUFFERS, &sampleBuffers);

  std::printf(
      "-------------------------------------------------------------\n");
  std::printf("GL Vendor    : %s\n", vendor);
  std::printf("GL Renderer  : %s\n", renderer);
  std::printf("GL Version   : %s\n", version);
  std::printf("GL Version   : %d.%d\n", major, minor);
  std::printf("GLSL Version : %s\n", glsl_version);
  std::printf("MSAA samples : %d\n", samples);
  std::printf("MSAA buffers : %d\n", sampleBuffers);
  std::printf(
      "-------------------------------------------------------------\n");
}

void APIENTRY debug_callback(GLenum source,
                             GLenum type,
                             GLuint id,
                             GLenum severity,
                             GLsizei /*length*/,
                             const GLchar *msg,
                             const void * /*param*/)
{
  std::string sourceStr;
  switch (source)
  {
  case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
    sourceStr = "WindowSys";
    break;
  case GL_DEBUG_SOURCE_APPLICATION:
    sourceStr = "App";
    break;
  case GL_DEBUG_SOURCE_API:
    sourceStr = "OpenGL";
    break;
  case GL_DEBUG_SOURCE_SHADER_COMPILER:
    sourceStr = "ShaderCompiler";
    break;
  case GL_DEBUG_SOURCE_THIRD_PARTY:
    sourceStr = "3rdParty";
    break;
  case GL_DEBUG_SOURCE_OTHER:
    sourceStr = "Other";
    break;
  default:
    sourceStr = "Unknown";
  }

  std::string typeStr;
  switch (type)
  {
  case GL_DEBUG_TYPE_ERROR:
    typeStr = "Error";
    break;
  case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
    typeStr = "Deprecated";
    break;
  case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
    typeStr = "Undefined";
    break;
  case GL_DEBUG_TYPE_PORTABILITY:
    typeStr = "Portability";
    break;
  case GL_DEBUG_TYPE_PERFORMANCE:
    typeStr = "Performance";
    break;
  case GL_DEBUG_TYPE_MARKER:
    typeStr = "Marker";
    break;
  case GL_DEBUG_TYPE_PUSH_GROUP:
    typeStr = "PushGrp";
    break;
  case GL_DEBUG_TYPE_POP_GROUP:
    typeStr = "PopGrp";
    break;
  case GL_DEBUG_TYPE_OTHER:
    typeStr = "Other";
    break;
  default:
    typeStr = "Unknown";
  }

  std::string sevStr;
  switch (severity)
  {
  case GL_DEBUG_SEVERITY_HIGH:
    sevStr = "HIGH";
    break;
  case GL_DEBUG_SEVERITY_MEDIUM:
    sevStr = "MED";
    break;
  case GL_DEBUG_SEVERITY_LOW:
    sevStr = "LOW";
    break;
  case GL_DEBUG_SEVERITY_NOTIFICATION:
    sevStr = "NOTIFY";
    break;
  default:
    sevStr = "UNK";
  }

  std::cerr << sourceStr << ":" << typeStr << "[" << sevStr << "]"
            << "(" << id << "): " << msg << std::endl;
}

} // namespace Fge::Gl
