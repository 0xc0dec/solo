#ifndef __SOLO_H__
#define __SOLO_H__

#include "SoloCommon.h"
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
#include "SoloMatrix3.h"
#include "SoloMatrix4.h"
#include "SoloPlane.h"
#include "SoloPlaneBoundedVolume.h"
#include "SoloQuaternion.h"
#include "SoloRay.h"
#include "SoloSphere.h"
#include "SoloVector2.h"
#include "SoloVector3.h"
#include "SoloVector4.h"

namespace solo
{
	extern "C" IEngine *getEngine();
}

#endif