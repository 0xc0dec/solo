#ifndef __SOLO_H__
#define __SOLO_H__

#include "SoloCommons.h"
#include "SoloIEngine.h"
#include "SoloIEngineCallback.h"
#include "SoloIDevice.h"

namespace solo
{
	extern "C" IEngine *getEngine();
}

#endif