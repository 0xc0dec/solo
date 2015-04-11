#include "SoloRenderState.h"
#include "SoloMaterialParameter.h"

using namespace solo;


MaterialParameter* RenderState::getParameter(const std::string& name)
{
	auto where = parameters.find(name);
	if (where != parameters.end())
		return where->second.get();
	auto parameter = NEW2(MaterialParameter, name);
	parameters[name] = parameter;
	return parameter.get();
}


void RenderState::setPolygonFace(PolygonFace face)
{
	polygonFace = face;
}


RenderState::PolygonFace RenderState::getPolygonFace() const
{
	return polygonFace;
}
