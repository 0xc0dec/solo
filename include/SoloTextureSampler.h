#pragma once

#include "SoloBase.h"

namespace solo
{
	class RenderContext;
	class Texture;

	class TextureSampler
	{
	public:
		virtual ~TextureSampler() {}

		enum class WrapMode
		{
			Clamp,
			Repeat
		};

		enum class Filter
		{
			Nearest,
			Linear,
			NearestMipmapNearest,
			LineapMipmapNearest,
			NearestMipmapLinear,
			LinearMipmapLinear
		};

		void setWrapMode(WrapMode verticalWrap, WrapMode horizontalWrap);
		void setFilterMode(Filter minFilter, Filter magFilter);

		virtual void apply(RenderContext& context) = 0;

	protected:
		TextureSampler(shared<Texture> texture);

	private:
		friend class TextureSamplerFactory;
		
		TextureSampler(const TextureSampler& other) = delete;
		TextureSampler(TextureSampler&& other) = delete;
		TextureSampler& operator=(const TextureSampler& other) = delete;
		TextureSampler& operator=(TextureSampler&& other) = delete;

		shared<Texture> texture;
		WrapMode verticalWrap;
		WrapMode horizontalWrap;
		Filter minFilter;
		Filter magFilter;
	};

	class TextureSamplerFactory
	{
		friend class ResourceManager;
		static shared<TextureSampler> create(shared<Texture> texture);
	};
}