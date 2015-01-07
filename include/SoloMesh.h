#pragma once

#include "SoloBase.h"

namespace solo
{
	// Represents a connected set of vertices and their attributes.
	class Mesh
	{
	public:
		virtual ~Mesh() {}

		static ptr<Mesh> create();

		virtual void draw() = 0;

	protected:
		Mesh() {}
	};
}