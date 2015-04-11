#include "SoloRenderState.h"

using namespace solo;


void RenderState::setPolygonFace(PolygonFace face)
{
	polygonFace = face;
}


RenderState::PolygonFace RenderState::getPolygonFace() const
{
	return polygonFace;
}
