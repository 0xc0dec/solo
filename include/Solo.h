#pragma once

#include "SoloBase.h"
#include "SoloBoundingBox.h"
#include "SoloBoundingSphere.h"
#include "SoloCamera.h"
#include "SoloComponent.h"
#include "SoloDevice.h"
#include "SoloDirty.h"
#include "SoloEffect.h"
#include "SoloEffectVariable.h"
#include "SoloEngine.h"
#include "SoloEngineCallback.h"
#include "SoloEngineCreationArgs.h"
#include "SoloException.h"
#include "SoloFileSystem.h"
#include "SoloFormatter.h"
#include "SoloFrustum.h"
#include "SoloLog.h"
#include "SoloMaterial.h"
#include "SoloMaterialParameter.h"
#include "SoloMath.h"
#include "SoloMatrix.h"
#include "SoloMesh.h"
#include "SoloModel.h"
#include "SoloModelRenderer.h"
#include "SoloNode.h"
#include "SoloPlane.h"
#include "SoloQuaternion.h"
#include "SoloRay.h"
#include "SoloRenderTarget.h"
#include "SoloRenderContext.h"
#include "SoloRenderState.h"
#include "SoloResourceManager.h"
#include "SoloScene.h"
#include "SoloScripter.h"
#include "SoloTexture.h"
#include "SoloTexture2D.h"
#include "SoloTransform.h"
#include "SoloTypeId.h"
#include "SoloVector2.h"
#include "SoloVector3.h"
#include "SoloVector4.h"

namespace solo
{
	extern "C" Engine *getEngine();
}
