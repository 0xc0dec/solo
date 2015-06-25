#pragma once

#include "SoloBase.h"

namespace solo
{
	class Texture2D;

	class RenderBuffer
	{
	public:
		virtual ~RenderBuffer() {}

		virtual void bind() = 0;

		void setTexture(shared<Texture2D> texture);

	protected:
		RenderBuffer() {}

		shared<Texture2D> texture;

	private:
		RenderBuffer(const RenderBuffer& other) = delete;
		RenderBuffer(RenderBuffer&& other) = delete;
		RenderBuffer& operator=(const RenderBuffer& other) = delete;
		RenderBuffer& operator=(RenderBuffer&& other) = delete;
	};
}
