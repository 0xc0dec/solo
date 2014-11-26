#ifndef __SOLO_H__
#define __SOLO_H__

#include "SoloBase.h"
#include "SoloException.h"
#include "SoloIComponent.h"
#include "SoloIDevice.h"
#include "SoloIEngine.h"
#include "SoloIEngineCallback.h"
#include "SoloIGPUProgram.h"
#include "SoloIMaterial.h"
#include "SoloIScene.h"
#include "SoloISceneNode.h"

namespace solo
{
	extern "C" IEngine *getEngine();
}

#endif