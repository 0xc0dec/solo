#pragma once

#include "SoloBase.h"

namespace solo
{
	class Texture2D;
	enum class EngineMode;

	enum class ColorFormat
	{
		RGB,
		RGBA
	};

	class Texture
	{
	public:
		static shared<Texture2D> create2D(EngineMode mode);

		virtual ~Texture() {}

		virtual void apply() = 0;

		virtual void *getNativeHandle() = 0;

	protected:
		Texture() {}

	private:
		Texture(const Texture& other) = delete;
		Texture(Texture&& other) = delete;
		Texture& operator=(const Texture& other) = delete;
		Texture& operator=(Texture&& other) = delete;
	};
}