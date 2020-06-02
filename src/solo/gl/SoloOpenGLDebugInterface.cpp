/* 
 * Copyright (c) Aleksey Fedotov 
 * MIT license 
 */

#include "SoloOpenGLDebugInterface.h"

#ifdef SL_OPENGL_RENDERER

#include "SoloOpenGLSDLDevice.h"
#include <imgui.h>
#include <examples/imgui_impl_sdl.h>
#include <examples/imgui_impl_opengl3.h>

using namespace solo;

OpenGLDebugInterface::OpenGLDebugInterface(Device *device):
	device_(dynamic_cast<OpenGLSDLDevice*>(device))
{
	IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImGui_ImplSDL2_InitForOpenGL(device_->window(), device_->window());
	ImGui_ImplOpenGL3_Init("#version 130"); // GL 3.0 + GLSL 130

	device_->onEvent([](auto &evt) { ImGui_ImplSDL2_ProcessEvent(&evt); });
}

OpenGLDebugInterface::~OpenGLDebugInterface()
{
	ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
}

void OpenGLDebugInterface::render() const
{
	ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(device_->window());
    ImGui::NewFrame();

	auto open = true; // never close
	ImGui::ShowDemoWindow(&open);

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

#endif
