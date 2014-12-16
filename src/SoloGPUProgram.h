#ifndef __SOLO_GPU_PROGRAM_H__
#define __SOLO_GPU_PROGRAM_H__

#include "SoloIGPUProgram.h"
#include "SoloBaseInternal.h"

namespace solo
{
	class GPUProgram: public IGPUProgram
	{
	public:
		GPUProgram(const std::string &vsShaderSrc, const std::string &fsShaderSrc);
		virtual ~GPUProgram();

		virtual bool valid() const override
		{
			return _valid;
		}

		virtual std::string log() const override
		{
			return _log;
		}

		virtual size_t id() const override
		{
			return _id;
		}

	protected:
		bool _valid;
		std::string _log;
		size_t _id;

		void _appendToLog(const std::string &newLog);
	};
}

#endif