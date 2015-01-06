#pragma once

#include "SoloBase.h"

namespace solo
{
	// Represents a connected set of vertices and their attributes.
	class Mesh
	{
	public:
		~Mesh() {}

		static ptr<Mesh> create();

	private:
		Mesh() {}
	};
}