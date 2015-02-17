#pragma once

#include "SoloBase.h"

namespace solo
{
	class Texture2D;

	class Texture
	{
	public:
		virtual ~Texture() {}

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
		static shared<Texture2D> create2D(std::vector<char> data);
	};
}