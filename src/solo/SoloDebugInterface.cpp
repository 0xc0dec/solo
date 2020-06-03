/* 
 * Copyright (c) Aleksey Fedotov 
 * MIT license 
 */

#include "SoloDebugInterface.h"
#include "SoloDevice.h"
#include "SoloEnums.h"
#include "SoloRenderer.h"
#include "vk/SoloVulkanDebugInterface.h"
#include "gl/SoloOpenGLDebugInterface.h"
#include <imgui.h>

using namespace solo;

auto DebugInterface::fromDevice(Device *device) -> sptr<DebugInterface>
{
	switch (device->mode())
    {
#ifdef SL_OPENGL_RENDERER
		case DeviceMode::OpenGL:
			return std::make_shared<OpenGLDebugInterface>(device);
#endif
		
#ifdef SL_VULKAN_RENDERER
        case DeviceMode::Vulkan:
            return std::make_shared<VulkanDebugInterface>(device);
#endif
    	default:
    		panic("Unknown device mode");
    		return nullptr;
    }
}

void DebugInterface::renderFrame(const std::function<void()> &render)
{
	beginFrame();
	render();
	endFrame();
}

void DebugInterface::overlay(const str &text)
{
	const float DISTANCE = 10.0f;
    static int corner = 0;
    ImGuiIO& io = ImGui::GetIO();
    if (corner != -1)
    {
        ImVec2 pos = ImVec2((corner & 1) ? io.DisplaySize.x - DISTANCE : DISTANCE, (corner & 2) ? io.DisplaySize.y - DISTANCE : DISTANCE);
        ImVec2 pivot = ImVec2((corner & 1) ? 1.0f : 0.0f, (corner & 2) ? 1.0f : 0.0f);
        ImGui::SetNextWindowPos(pos, ImGuiCond_Always, pivot);
    }
    ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
    ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings
		| ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
    if (corner != -1)
        flags |= ImGuiWindowFlags_NoMove;
    if (ImGui::Begin("Example: Simple overlay", nullptr, flags))
        ImGui::Text(text.c_str());
    ImGui::End();
}

DebugInterface::DebugInterface(Device *device):
	renderer_(device->renderer())
{
}

void DebugInterface::endFrame()
{
	ImGui::Render();
	renderer_->renderDebugInterface(this);
}
