#ifndef __SOLO_H__
#define __SOLO_H__

#include "SoloBase.h"
#include "SoloBoundingBox.h"
#include "SoloBoundingSphere.h"
#include "SoloException.h"
#include "SoloFormatter.h"
#include "SoloFrustum.h"
#include "SoloIComponent.h"
#include "SoloIDevice.h"
#include "SoloIEngine.h"
#include "SoloIEngineCallback.h"
#include "SoloIGPUProgram.h"
#include "SoloIMaterial.h"
#include "SoloIScene.h"
#include "SoloITransform.h"
#include "SoloMath.h"
#include "SoloMatrix.h"
#include "SoloPlane.h"
#include "SoloQuaternion.h"
#include "SoloRay.h"
#include "SoloTypeId.h"
#include "SoloVector2.h"
#include "SoloVector3.h"
#include "SoloVector4.h"

namespace solo
{
	extern "C" IEngine *getEngine();
}

#endif