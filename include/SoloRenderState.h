#pragma once

#include "SoloBase.h"

namespace solo
{
	class MaterialParameter;
	class RenderContext;

	class RenderState
	{
	public:
		enum class PolygonFace
		{
			CW,
			CCW,
			All
		};

		virtual ~RenderState() {}

		virtual void bind(RenderContext& context) = 0;
		virtual void unbind(RenderContext& context) = 0;

		MaterialParameter* getParameter(const std::string &name);

		void setPolygonFace(PolygonFace face);
		PolygonFace getPolygonFace() const;

	protected:
		RenderState();

		virtual void applyFaceCull() = 0;

		PolygonFace polygonFace;
		std::map<std::string, shared<MaterialParameter>> parameters;

	private:
		RenderState(const RenderState& other) = delete;
		RenderState(RenderState&& other) = delete;
		RenderState& operator=(const RenderState& other) = delete;
		RenderState& operator=(RenderState&& other) = delete;
	};
}