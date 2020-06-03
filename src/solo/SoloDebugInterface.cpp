/* 
 * Copyright (c) Aleksey Fedotov 
 * MIT license 
 */

#include "SoloDebugInterface.h"
#include "SoloDevice.h"
#include "SoloEnums.h"
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

void DebugInterface::magic()
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
    {
        ImGui::Text("Simple overlay\n" "in the corner of the screen.\n" "(right-click to change position)");
        ImGui::Separator();
        if (ImGui::IsMousePosValid())
            ImGui::Text("Mouse Position: (%.1f,%.1f)", io.MousePos.x, io.MousePos.y);
        else
            ImGui::Text("Mouse Position: <invalid>");
    }
    ImGui::End();
}
