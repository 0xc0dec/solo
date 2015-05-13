#pragma once

#include "SoloBase.h"

namespace solo
{
	class Texture2D;

	class Texture
	{
	public:
		enum class ColorFormat
		{
			RGB,
			RGBA
		};

		virtual ~Texture() {}

		virtual void apply() = 0;

	protected:
		Texture() {}

	private:
		Texture(const Texture& other) = delete;
		Texture(Texture&& other) = delete;
		Texture& operator=(const Texture& other) = delete;
		Texture& operator=(Texture&& other) = delete;
	};

	class TextureFactory
	{
		friend class PngTextureLoader;
		static shared<Texture2D> create2D();
	};
}