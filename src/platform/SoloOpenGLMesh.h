#pragma once

#include "SoloMesh.h"

namespace solo
{
	class OpenGLMesh : public Mesh
	{
	public:
		virtual ~OpenGLMesh() override {}

		virtual void draw() override;
	};
}