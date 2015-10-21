#pragma once

namespace solo
{
	struct RenderContext;

	class Renderer
	{
	public:
		virtual ~Renderer() {}

		virtual void render(RenderContext& context) = 0;

		unsigned getRenderQueue() const
		{
			return renderQueue;
		}

		void setRenderQueue(unsigned queue)
		{
			renderQueue = queue;
		}

	private:
		unsigned renderQueue = 0;
	};
}