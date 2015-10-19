#pragma once

#include "SoloBase.h"

namespace solo
{
	enum class ColorFormat
	{
		RGB,
		RGBA
	};

	struct Image
	{
		unsigned width;
		unsigned height;
		ColorFormat colorFormat;
		std::vector<uint8_t> data;
	};
}