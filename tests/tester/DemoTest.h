#pragma once

#include "TestBase.h"

using namespace solo;

class DemoTest: public TestBase
{
public:
	DemoTest(Engine* engine): TestBase(engine)
	{
	}

	virtual void run() override;

private:
	shared<Effect> texEffect;
	shared<Material> texMaterial;
	shared<Material> checkerMaterial;
	shared<Material> texWithLightingMaterial;
	shared<Material> renderTargetMaterial;

	shared<Texture> loadTexture(const std::string &path);
	void initMaterials();
	shared<RenderTarget> initRenderTarget();
	void initCameras(shared<RenderTarget> renderTarget);
	Transform *initModel();
	void initRTTQuad(Transform *targetTransform);
	void initTexturedQuad();
	void initBox();
	void rebuildToBoxMesh(shared<Node> node);
	shared<Node> createQuad();
};