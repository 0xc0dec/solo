#pragma once

namespace solo
{
	struct RenderContext;

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
		RenderState(const RenderState& other) = delete;
		RenderState(RenderState&& other) = delete;
		RenderState& operator=(const RenderState& other) = delete;
		RenderState& operator=(RenderState&& other) = delete;

		virtual void bind(RenderContext& context) = 0;
		virtual void unbind(RenderContext& context) = 0;

		PolygonFace getPolygonFace() const;
		void setPolygonFace(PolygonFace face);

		bool getZWriteEnabled() const;
		void setZWriteEnabled(bool enabled);

	protected:
		RenderState() {}

		virtual void applyFaceCull() = 0;
		virtual void applyZWrite() = 0;

		PolygonFace polygonFace = PolygonFace::CW;
		bool zwrite = true;
	};
}