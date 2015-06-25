#pragma once

#include "SoloBase.h"

namespace solo
{
	class RenderContext;

	enum class PolygonFace
	{
		CW,
		CCW,
		All
	};

	class RenderState
	{
	public:
		virtual ~RenderState() {}

		virtual void bind(RenderContext& context) = 0;
		virtual void unbind(RenderContext& context) = 0;

		void setPolygonFace(PolygonFace face);
		PolygonFace getPolygonFace() const;

	protected:
		RenderState() {}

		virtual void applyFaceCull() = 0;

		PolygonFace polygonFace = PolygonFace::CW;

	private:
		RenderState(const RenderState& other) = delete;
		RenderState(RenderState&& other) = delete;
		RenderState& operator=(const RenderState& other) = delete;
		RenderState& operator=(RenderState&& other) = delete;
	};
}