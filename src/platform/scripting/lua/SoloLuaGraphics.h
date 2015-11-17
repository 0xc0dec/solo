#pragma once

#include <LuaIntf.h>

namespace solo
{
	class Graphics;
	class Material;

	class LuaGraphics
	{
	public:
		static void renderSurface(Graphics *graphics, Material *material, LuaIntf::LuaRef target);
	};
}