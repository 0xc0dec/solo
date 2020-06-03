/* 
 * Copyright (c) Aleksey Fedotov 
 * MIT license 
 */

#pragma once

#include "SoloCommon.h"

#ifdef SL_OPENGL_RENDERER

#include "SoloDebugInterface.h"

namespace solo
{
	class Device;
	class OpenGLSDLDevice;

	class OpenGLDebugInterface final: public DebugInterface
	{
	public:
		explicit OpenGLDebugInterface(Device *device);
		~OpenGLDebugInterface();

		void beginFrame() override;
		
		void render() const;
		
	private:
		OpenGLSDLDevice *device_;
	};
}

#endif
