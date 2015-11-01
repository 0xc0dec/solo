#pragma once

namespace solo
{
	struct RenderContext;

	class Renderer
	{
	public:
		virtual ~Renderer() {}

		virtual void render(RenderContext& context) = 0;

		unsigned getRenderQueue() const;
		void setRenderQueue(unsigned queue);

	private:
		unsigned renderQueue = 0;
	};

	inline unsigned Renderer::getRenderQueue() const
	{
		return renderQueue;
	}

	inline void Renderer::setRenderQueue(unsigned queue)
	{
		renderQueue = queue;
	}
}