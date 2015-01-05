#pragma once

#include "SoloBaseInternal.h"
#include "SoloIMeshRenderer.h"

namespace solo
{
	class MeshRenderer: public IMeshRenderer
	{
	public:
		static ptr<MeshRenderer> create();

	private:
		MeshRenderer() {}
		virtual ~MeshRenderer() {}
	};
}