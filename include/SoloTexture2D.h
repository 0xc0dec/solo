#pragma once

#include "SoloTexture.h"

namespace solo
{
	class Texture2D: public Texture
	{
	public:
		virtual void generateMipmaps() = 0;

	protected:
		Texture2D() {}

	private:
		Texture2D(const Texture2D& other) = delete;
		Texture2D(Texture2D&& other) = delete;
		Texture2D& operator=(const Texture2D& other) = delete;
		Texture2D& operator=(Texture2D&& other) = delete;
	};
}