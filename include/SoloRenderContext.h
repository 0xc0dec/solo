#pragma once

#include "SoloBase.h"

namespace solo
{
	class Camera;

	struct RenderContext
	{
		size_t renderedNode;
		ptr<Camera> renderingCamera;

		RenderContext(size_t renderedNode, ptr<Camera> renderingCamera) :
			renderedNode(renderedNode),
			renderingCamera(renderingCamera)
		{
		}
	};
}