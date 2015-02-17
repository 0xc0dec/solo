#pragma once

#include "SoloBase.h"

namespace solo
{
	class MaterialParameter;

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

		MaterialParameter* getParameter(const std::string &name);

		void setPolygonFace(PolygonFace face);
		PolygonFace getPolygonFace() const;

	protected:
		RenderState();
		RenderState(const RenderState& other) = delete;
		RenderState(RenderState&& other) = delete;
		RenderState& operator=(const RenderState& other) = delete;
		RenderState& operator=(RenderState&& other) = delete;

		std::map<std::string, shared<MaterialParameter>> parameters;
		PolygonFace polygonFace;
	};
}