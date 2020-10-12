#include "graphic_manager.hpp"
#include "GLFW/glfw3.h"
#include "application.hpp"
#include "imgui.hpp"
#include "platform/glfw/glfw_window.hpp"

namespace Fge
{

void GraphicManager::create_window()
{
  auto glfw_window = std::make_shared<Glfw::GlfwWindow>();
  glfw_window->create_window();
  glfw_window->init_open_gl();

  window = glfw_window;

  init_imgui(glfw_window->get_glfw_window());
}

std::shared_ptr<RenderView>
GraphicManager::create_render_view(const RenderView::Target /*target*/,
                                   const uint32_t /*width*/,
                                   const uint32_t /*height*/,
                                   const uint32_t /*samples*/)
{
  return {};
}

void GraphicManager::flush() { window->flush(); }

void GraphicManager::terminate()
{

  terminate_imgui();
  window->terminate();
}

void GraphicManager::init_imgui(GLFWwindow *window)
{
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

  io.Fonts->AddFontFromFileTTF(
      (file_manager->get_app_res_path() / "fonts/Roboto-Medium.ttf").c_str(),
      16.0f);

  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 460");
}

void GraphicManager::terminate_imgui()
{
  ImGui_ImplGlfw_Shutdown();
  ImGui_ImplOpenGL3_Shutdown();
  ImGui::DestroyContext();
}

void GraphicManager::begin_imgui_render()
{
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
}

void GraphicManager::end_imgui_render()
{
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

} // namespace Fge
