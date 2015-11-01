#pragma once

#include "SoloBase.h"

namespace solo
{
	class Texture2D;
	class CubeTexture;
	enum class EngineMode;

	enum class TextureWrapMode
	{
		Clamp,
		Repeat
	};

	enum class TextureFilter
	{
		Nearest,
		Linear,
		NearestMipmapNearest,
		LinearMipmapNearest,
		NearestMipmapLinear,
		LinearMipmapLinear
	};

	class Texture
	{
	public:
		Texture(const Texture& other) = delete;
		Texture(Texture&& other) = delete;
		Texture& operator=(const Texture& other) = delete;
		Texture& operator=(Texture&& other) = delete;
		virtual ~Texture() {}

		virtual void apply() = 0;

		TextureWrapMode getHorizontalWrapMode() const;
		void setHorizontalWrapMode(TextureWrapMode horizontalWrap);
		
		TextureWrapMode getVerticalWrapMode() const;
		void setVerticalWrapMode(TextureWrapMode verticalWrap);

		TextureFilter getMinFilter() const;
		void setMinFilter(TextureFilter minFilter);

		TextureFilter getMagFilter() const;
		void setMagFilter(TextureFilter magFilter);

		float getAnisotropyLevel() const;
		void setAnisotropyLevel(float level);

	protected:
		Texture() {}

		TextureWrapMode horizontalWrap = TextureWrapMode::Repeat;
		TextureWrapMode verticalWrap = TextureWrapMode::Repeat;
		TextureFilter minFilter = TextureFilter::Linear;
		TextureFilter magFilter = TextureFilter::Linear;
		float anisotropy = 1.0f;
	};

	class TextureFactory
	{
		friend class ResourceManager;
		static shared<Texture2D> create2D(EngineMode mode);
		static shared<CubeTexture> createCube(EngineMode mode);
	};
}