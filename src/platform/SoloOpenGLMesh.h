#pragma once

#include "SoloMesh.h"

namespace solo
{
	class OpenGLMesh : public Mesh
	{
	public:
		virtual ~OpenGLMesh() override {}

		static ptr<OpenGLMesh> create();

		virtual void draw() override;

	private:
		OpenGLMesh() {}
	};
}