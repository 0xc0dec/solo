#include "SoloTexture2D.h"

using namespace solo;


void Texture2D::setData(ColorFormat format, const std::vector<uint8_t>& data, unsigned width, unsigned height)
{
	applyData(format, data, width, height);
	size = { static_cast<float>(width), static_cast<float>(height) };
}


Vector2 Texture2D::getSize() const
{
	return size;
}
