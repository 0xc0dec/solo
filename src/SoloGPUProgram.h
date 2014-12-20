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

		virtual bool isValid() const override;

		virtual std::string getLog() const override;

		virtual size_t getId() const override;

	protected:
		bool _valid;
		std::string _log;
		size_t _id;

		void _appendToLog(const std::string &newLog);
	};
}

#endif