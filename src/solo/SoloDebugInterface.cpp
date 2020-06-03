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

void DebugInterface::renderWindow(const WindowConfig &cfg, const std::function<void()> &renderInside)
{
	const ImVec2 pos = ImVec2(cfg.position.x(), cfg.position.y());
	const ImVec2 pivot = ImVec2(cfg.pivot.x(), cfg.pivot.y());
	
	ImGui::SetNextWindowPos(pos, ImGuiCond_Once, pivot);
	
    ImGui::SetNextWindowBgAlpha(cfg.alpha);

	ImGuiWindowFlags flags = ImGuiWindowFlags_NoSavedSettings;
	if (cfg.autoResize)
		flags |= ImGuiWindowFlags_AlwaysAutoResize;
	if (!cfg.decoration)
		flags |= ImGuiWindowFlags_NoDecoration;
	if (!cfg.movable)
        flags |= ImGuiWindowFlags_NoMove;

	ImGui::Begin(cfg.title.c_str(), nullptr, flags);

	renderInside();
	
	ImGui::End();
}

void DebugInterface::renderText(const str &text)
{
	ImGui::Text(text.c_str());
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
