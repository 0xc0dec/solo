#pragma once

#include "SoloBaseInternal.h"
#include "SoloIMesh.h"

namespace solo
{
	// Represents a connected set of vertices and their attributes.
	class Mesh : public IMesh
	{
	public:
		static ptr<Mesh> create();

	private:
		Mesh() {}
		virtual ~Mesh() {}
	};
}