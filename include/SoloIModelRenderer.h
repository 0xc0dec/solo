#pragma once

#include "SoloIComponent.h"

namespace solo
{
	class IModel;

	class IModelRenderer: public Component<IModelRenderer>
	{
	public:
		virtual void setModel(const ptr<IModel> model) = 0;
		virtual ptr<IModel> getModel() = 0;
	};
}