#ifndef __SOLO_I_GPU_PROGRAM_H__
#define __SOLO_I_GPU_PROGRAM_H__

#include "SoloBase.h"

namespace solo
{
	class IGPUProgram
	{
	public:
		virtual ~IGPUProgram() {}

		virtual size_t getId() const = 0;

		virtual bool isValid() const = 0;
		virtual std::string getLog() const = 0;
	};
}

#endif