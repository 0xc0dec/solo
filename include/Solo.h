#ifndef __SOLO_H__
#define __SOLO_H__

#include "SoloCommons.h"
#include "SoloException.h"
#include "SoloIComponent.h"
#include "SoloIDevice.h"
#include "SoloIEngine.h"
#include "SoloIEngineCallback.h"
#include "SoloIGPUProgram.h"
#include "SoloIMaterial.h"
#include "SoloIScene.h"
#include "SoloISceneNode.h"
#include "SoloMath.h"
#include "SoloVec3.h"

namespace solo
{
	extern "C" IEngine *getEngine();
}

#endif