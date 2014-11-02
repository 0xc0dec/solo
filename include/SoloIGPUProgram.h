#ifndef __SOLO_I_GPU_PROGRAM_H__
#define __SOLO_I_GPU_PROGRAM_H__

#include "SoloCommons.h"

namespace solo
{
	class IGPUProgram
	{
	public:
		virtual ~IGPUProgram() {}

		virtual size_t id() const = 0;

		virtual bool valid() const = 0;
		virtual str log() const = 0;
	};
}

#endif