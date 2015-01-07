#include "SoloRenderState.h"
#include "SoloMaterialParameter.h"

using namespace solo;


ptr<MaterialParameter> RenderState::getParameter(const std::string& name)
{
	auto where = _parameters.find(name);
	if (where != _parameters.end())
		return where->second;
	// Create a new one if not found
	auto parameter = NEW<MaterialParameter>(name);
	_parameters[name] = parameter;
	return parameter;
}
