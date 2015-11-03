#pragma once

#include "SoloBase.h"

namespace solo
{
	class Texture2D;
	class CubeTexture;
	enum class DeviceMode;

	enum class TextureWrapping
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

		TextureWrapping getHorizontalWrapping() const;
		void setHorizontalWrapping(TextureWrapping horizontalWrap);
		
		TextureWrapping getVerticalWrapping() const;
		void setVerticalWrapping(TextureWrapping verticalWrap);

		virtual void setWrapping(TextureWrapping wrap);

		TextureFiltering getMinFiltering() const;
		void setMinFiltering(TextureFiltering filtering);

		TextureFiltering getMagFiltering() const;
		void setMagFiltering(TextureFiltering filtering);

		void setFiltering(TextureFiltering filtering);

		float getAnisotropyLevel() const;
		void setAnisotropyLevel(float level);

	protected:
		Texture() {}

		TextureWrapping horizontalWrapping = TextureWrapping::Repeat;
		TextureWrapping verticalWrapping = TextureWrapping::Repeat;
		TextureFiltering minFiltering = TextureFiltering::Linear;
		TextureFiltering magFiltering = TextureFiltering::Linear;
		float anisotropy = 1.0f;
	};

	inline TextureWrapping Texture::getVerticalWrapping() const
	{
		return verticalWrapping;
	}

	inline void Texture::setVerticalWrapping(TextureWrapping wrap)
	{
		verticalWrapping = wrap;
	}

	inline TextureWrapping Texture::getHorizontalWrapping() const
	{
		return horizontalWrapping;
	}

	inline void Texture::setHorizontalWrapping(TextureWrapping wrap)
	{
		horizontalWrapping = wrap;
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
		static shared<Texture2D> create2D(DeviceMode mode);
		static shared<CubeTexture> createCube(DeviceMode mode);
	};
}