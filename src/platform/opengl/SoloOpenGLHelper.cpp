#include "SoloOpenGLHelper.h"

using namespace solo;


GLenum OpenGLHelper::convertToGLColorFormat(ColorFormat format)
{
	switch (format)
	{
		case ColorFormat::RGB:
			return GL_RGB;
		case ColorFormat::RGBA:
			return GL_RGBA;
		default:
			SL_THROW_FMT(EngineException, "Unexpected texture format ", static_cast<int>(format));
	}
}


GLenum OpenGLHelper::convertToGLWrapMode(TextureWrapMode mode)
{
	switch (mode)
	{
		case TextureWrapMode::Clamp:
			return GL_CLAMP_TO_EDGE;
		case TextureWrapMode::Repeat:
			return GL_REPEAT;
		default:
			SL_THROW_FMT(EngineException, "Unexpected wrap mode ", static_cast<int>(mode));
	}
}


GLenum OpenGLHelper::convertToGLFilter(TextureFilter filter)
{
	switch (filter)
	{
		case TextureFilter::Linear:
			return GL_LINEAR;
		case TextureFilter::Nearest:
			return GL_NEAREST;
		case TextureFilter::LinearMipmapNearest:
			return GL_LINEAR_MIPMAP_NEAREST;
		case TextureFilter::LinearMipmapLinear:
			return GL_LINEAR_MIPMAP_LINEAR;
		case TextureFilter::NearestMipmapLinear:
			return GL_NEAREST_MIPMAP_LINEAR;
		case TextureFilter::NearestMipmapNearest:
			return GL_NEAREST_MIPMAP_NEAREST;
		default:
			SL_THROW_FMT(EngineException, "Unexpected texture filter ", static_cast<int>(filter));
	}
}