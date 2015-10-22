#include "SoloRenderState.h"

using namespace solo;


void RenderState::setPolygonFace(PolygonFace face)
{
	polygonFace = face;
}


PolygonFace RenderState::getPolygonFace() const
{
	return polygonFace;
}


bool RenderState::getZWriteEnabled() const
{
	return zwrite;
}


void RenderState::setZWriteEnabled(bool enabled)
{
	zwrite = enabled;
}