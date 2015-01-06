#pragma once

#include "SoloBase.h"
#include "SoloBoundingBox.h"
#include "SoloBoundingSphere.h"
#include "SoloException.h"
#include "SoloFormatter.h"
#include "SoloFrustum.h"
#include "SoloCamera.h"
#include "SoloComponent.h"
#include "SoloDevice.h"
#include "SoloEngine.h"
#include "SoloEngineCallback.h"
#include "SoloEffect.h"
#include "SoloMaterial.h"
#include "SoloScene.h"
#include "SoloTransform.h"
#include "SoloVideoDriver.h"
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
	extern "C" Engine *getEngine();
}
