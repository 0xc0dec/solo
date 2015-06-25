#include "SoloRenderBuffer.h"

using namespace solo;


void RenderBuffer::setTexture(shared<Texture2D> texture)
{
	this->texture = texture;
}