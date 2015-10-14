#pragma once

#include "TestBase.h"


class Demo_Test: public TestBase
{
public:
	Demo_Test(Engine* engine): TestBase(engine)
	{
	}

	virtual void run() override;

private:
	shared<RenderTarget> renderTarget;
	shared<Effect> texEffect;
	shared<Effect> colorEffect;
	shared<Material> redMaterial;
	shared<Material> greenMaterial;
	shared<Material> blueMaterial;
	shared<Material> whiteMaterial;
	shared<Material> texMaterial;
	shared<Material> checkerMaterial;
	shared<Material> texWithLightingMaterial;
	shared<Material> renderTargetMaterial;

	shared<Model> axesModel;
	shared<Model> monkeyModel;

	shared<Texture> loadTexture(const std::string &path);
	void initMaterials();
	void initModels();
	void initRenderTarget();
	void initCameras();
	void initObjects();
	void rebuildToBoxMesh(shared<Node> node);
	void initAxesModel(shared<Node> node);
	shared<Node> createQuad();
};