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

	enum class TextureFiltering
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

		virtual void setWrapMode(TextureWrapMode wrap);

		TextureFiltering getMinFiltering() const;
		void setMinFiltering(TextureFiltering filtering);

		TextureFiltering getMagFiltering() const;
		void setMagFiltering(TextureFiltering filtering);

		void setFiltering(TextureFiltering filtering);

		float getAnisotropyLevel() const;
		void setAnisotropyLevel(float level);

	protected:
		Texture() {}

		TextureWrapMode horizontalWrap = TextureWrapMode::Repeat;
		TextureWrapMode verticalWrap = TextureWrapMode::Repeat;
		TextureFiltering minFiltering = TextureFiltering::Linear;
		TextureFiltering magFiltering = TextureFiltering::Linear;
		float anisotropy = 1.0f;
	};

	inline TextureWrapMode Texture::getVerticalWrapMode() const
	{
		return verticalWrap;
	}

	inline void Texture::setVerticalWrapMode(TextureWrapMode wrap)
	{
		verticalWrap = wrap;
	}

	inline TextureWrapMode Texture::getHorizontalWrapMode() const
	{
		return horizontalWrap;
	}

	inline void Texture::setHorizontalWrapMode(TextureWrapMode wrap)
	{
		horizontalWrap = wrap;
	}

	inline TextureFiltering Texture::getMinFiltering() const
	{
		return minFiltering;
	}

	inline void Texture::setMinFiltering(TextureFiltering filtering)
	{
		minFiltering = filtering;
	}

	inline TextureFiltering Texture::getMagFiltering() const
	{
		return magFiltering;
	}

	inline void Texture::setMagFiltering(TextureFiltering filtering)
	{
		magFiltering = filtering;
	}

	inline void Texture::setFiltering(TextureFiltering filtering)
	{
		minFiltering = filtering;
		magFiltering = filtering;
	}

	inline float Texture::getAnisotropyLevel() const
	{
		return anisotropy;
	}

	inline void Texture::setAnisotropyLevel(float level)
	{
		anisotropy = level;
	}

	class TextureFactory
	{
		friend class ResourceManager;
		static shared<Texture2D> create2D(EngineMode mode);
		static shared<CubeTexture> createCube(EngineMode mode);
	};
}