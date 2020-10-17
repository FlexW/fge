#include "graphic_manager.hpp"
#include "application.hpp"
#include "forward_render_path.hpp"
#include "graphic/render_view.hpp"
#include "imgui.hpp"
#include "platform/glfw/glfw_window.hpp"
#include "platform/opengl/gl_renderer.hpp"

namespace Fge
{

void GraphicManager::create_window()
{
  auto glfw_window = std::make_shared<Glfw::GlfwWindow>();
  glfw_window->create_window();
  glfw_window->init_open_gl();

  renderer = std::make_shared<Gl::Renderer>();

  render_path = std::make_shared<ForwardRenderPath>();

  window = glfw_window;

  init_imgui(glfw_window->get_glfw_window());
}

std::shared_ptr<RenderView>
GraphicManager::create_render_view(const RenderView::Target target,
                                   const uint32_t           width,
                                   const uint32_t           height,
                                   const uint32_t           samples)
{
  return std::make_shared<RenderView>(target, width, height, samples);
}

void GraphicManager::flush() { window->flush(); }

void GraphicManager::terminate()
{
  renderer->terminate();
  terminate_imgui();
  window->terminate();
}

void GraphicManager::init_imgui(GLFWwindow *window)
{
  // TODO: Maybe this should go into Renderer

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  (void)io;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();
  ImGuiStyle &style                 = ImGui::GetStyle();
  style.WindowRounding              = 0.0f;
  style.Colors[ImGuiCol_WindowBg].w = 1.0f;

  // Set font
  auto app          = Application::get_instance();
  auto file_manager = app->get_file_manager();

#ifndef WIN32
  io.Fonts->AddFontFromFileTTF(
      (file_manager->get_app_res_path() / "fonts/Roboto-Medium.ttf")
          .string()
          .c_str(),
      16.0f);
#endif // WIN32

  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 460");
}

void GraphicManager::terminate_imgui()
{
  // TODO: Maybe this should go into Renderer

  ImGui_ImplGlfw_Shutdown();
  ImGui_ImplOpenGL3_Shutdown();
  ImGui::DestroyContext();
}

void GraphicManager::begin_imgui_render()
{
  // TODO: Maybe this should go into Renderer

  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
}

void GraphicManager::end_imgui_render()
{
  // TODO: Maybe this should go into Renderer

  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void GraphicManager::begin_render() {}

void GraphicManager::end_render() {}

} // namespace Fge
