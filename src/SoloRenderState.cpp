#include "SoloRenderState.h"
#include "SoloMaterialParameter.h"

using namespace solo;


ptr<MaterialParameter> RenderState::getParameter(const std::string& name)
{
	auto where = parameters.find(name);
	if (where != parameters.end())
		return where->second;
	// Create a new one if not found
	auto parameter = MaterialParameter::create(name);
	parameters[name] = parameter;
	return parameter;
}
