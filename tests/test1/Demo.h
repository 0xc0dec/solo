#pragma once

#include "TestBase.h"
#include "Shaders.h"


class InputWatcher : public ComponentBase<InputWatcher>
{
public:
	explicit InputWatcher(Node* node): ComponentBase<InputWatcher>(node)
	{
		device = Engine::get()->getDevice();
	}

	void update() override
	{
		if (device->isKeyPressed(KeyCode::Escape, true))
			device->requestShutdown();
	}

private:
	Device* device;
};


class SpectatorCamera : public ComponentBase<SpectatorCamera>
{
public:
	SpectatorCamera(Node* node): ComponentBase<SpectatorCamera>(node)
	{
		engine = Engine::get();
		device = engine->getDevice();
		transform = node->getComponent<Transform>();
	}

	virtual void update() override
	{
		auto mouseMotion = device->getMouseMotion();
		auto dt = engine->getTimeDelta();

		if (device->isMouseButtonPressed(MouseButton::Right, true))
			device->setCursorCaptured(true);
		if (device->isMouseButtonReleased(MouseButton::Right))
			device->setCursorCaptured(false);

		if (device->isMouseButtonPressed(MouseButton::Right, false))
		{
			if (mouseMotion.x != 0)
				transform->rotate(Vector3::unitY(), 0.5f * dt * -mouseMotion.x, TransformSpace::World);
			if (mouseMotion.y != 0)
				transform->rotate(Vector3::unitX(), 0.5f * dt * -mouseMotion.y, TransformSpace::Self);
		}

		Vector3 movement;
		if (device->isKeyPressed(KeyCode::W))
			movement += transform->getLocalForward();
		if (device->isKeyPressed(KeyCode::S))
			movement += transform->getLocalBack();
		if (device->isKeyPressed(KeyCode::A))
			movement += transform->getLocalLeft();
		if (device->isKeyPressed(KeyCode::D))
			movement += transform->getLocalRight();
		if (device->isKeyPressed(KeyCode::Q))
			movement += transform->getLocalDown();
		if (device->isKeyPressed(KeyCode::E))
			movement += transform->getLocalUp();

		movement.normalize();
		movement *= dt * 10;
		transform->translateLocal(movement);
	}

private:
	Transform* transform;
	Engine* engine;
	Device* device;
};


class RotatorAroundWorldYAxis : public ComponentBase<RotatorAroundWorldYAxis>
{
public:
	explicit RotatorAroundWorldYAxis(Node* node): ComponentBase<RotatorAroundWorldYAxis>(node)
	{
		engine = Engine::get();
		transform = node->getComponent<Transform>();
	}

	virtual void update() override
	{
		auto angle = engine->getTimeDelta();
		transform->rotate(Vector3::unitY(), angle, TransformSpace::World);
	}

private:
	Engine* engine;
	Transform* transform;
};


class RotatorAroundLocalXAxis : public ComponentBase<RotatorAroundLocalXAxis>
{
public:
	explicit RotatorAroundLocalXAxis(Node* node): ComponentBase<RotatorAroundLocalXAxis>(node)
	{
		engine = Engine::get();
		transform = node->getComponent<Transform>();
	}

	virtual void update() override
	{
		auto angle = engine->getTimeDelta() * 1.3f;
		transform->rotate(Vector3::unitX(), angle, TransformSpace::Self);
	}

private:
	Engine* engine;
	Transform* transform;
};


class RenderTargetUpdater: public ComponentBase<RenderTargetUpdater>
{
public:
	explicit RenderTargetUpdater(Node* node): ComponentBase<RenderTargetUpdater>(node)
	{
		engine = Engine::get();
		sizes.push_back({ 80, 60 });
		sizes.push_back({ 160, 120 });
		sizes.push_back({ 320, 240 });
		sizes.push_back({ 640, 480 });
		sizes.push_back({ 1280, 960 });
		updateSize();
	}

	void update() override
	{
		time += engine->getTimeDelta();
		if (time >= 1)
		{
			time = 0;
			updateSize();
		}
	}

	void updateSize()
	{
		auto camera = node->getComponent<Camera>();
		auto renderTarget = camera->getRenderTarget();
		auto texture = renderTarget->getTextures()[0];
		currentSizeIdx++;
		if (currentSizeIdx >= sizes.size())
			currentSizeIdx = 0;
		auto newSize = sizes[currentSizeIdx];
		texture->setData(ColorFormat::RGB, {}, newSize.x, newSize.y);
		camera->setViewport(0, 0, newSize.x, newSize.y);
		renderTarget->setTextures({ texture });
	}

private:
	Engine *engine;
	float time = 0;
	std::vector<Vector2> sizes;
	int currentSizeIdx = 0;
};


class Demo : public TestBase
{
public:
	Demo(Engine* engine) : TestBase(engine)
	{
		resManager = engine->getResourceManager();
	}

	virtual void run() override
	{
		initTextures();
		initMaterials();
		initOffscreenRendering();

		auto mainCameraNode = scene->createNode();
		auto mainCameraTransform = mainCameraNode->getComponent<Transform>();
		mainCameraTransform->setLocalPosition(0, 0, 10);
		mainCameraNode->addComponent<SpectatorCamera>();
		mainCameraNode->addComponent<InputWatcher>();
		auto mainCamera = mainCameraNode->addComponent<Camera>();
		mainCamera->setClearColor(0, 0.6f, 0.6f, 1);
		mainCamera->setNear(0.05f);

		initStaticQuad({ 0, 7, 0 }, renderTargetMaterial);
		initRotatingQuad({ 5, 0, 0 }, textureMaterial);
		initBox({ -5, 0, 0 }, checkerMaterial);
		initModel("../data/monkey_hires.obj", Vector3(0, 0, 0), texWithLightingMaterial);
	}

	void initOffscreenRendering()
	{
		auto renderTarget = resManager->getOrCreateRenderTarget("test");
		auto renderTexture = DYNAMIC_CAST<Texture2D>(resManager->getOrCreateTexture("RTT"));
		renderTexture->setData(ColorFormat::RGB, {}, 640, 480);
		renderTexture->setFilterMode(Filter::Nearest, Filter::Nearest);
		renderTexture->setWrapMode(WrapMode::Clamp, WrapMode::Clamp);
		renderTarget->setTextures({ renderTexture });

		auto offscreenCameraNode = scene->createNode();
		auto offscreenCameraTransform = offscreenCameraNode->getComponent<Transform>();
		offscreenCameraTransform->setLocalPosition(0, 0, 10);
		auto offscreenCamera = offscreenCameraNode->addComponent<Camera>();
		offscreenCamera->setClearColor(1, 1, 1, 1);
		offscreenCamera->setNear(0.05f);
		offscreenCamera->setRenderTarget(renderTarget);
		offscreenCamera->getNode()->addComponent<RenderTargetUpdater>();

		renderTargetMaterial = resManager->createMaterial(textureEffect);
		renderTargetMaterial->setPolygonFace(PolygonFace::All);
		renderTargetMaterial->getParameter("worldViewProjMatrix")->bindValue(AutoBinding::WorldViewProjectionMatrix);
		renderTargetMaterial->getParameter("mainTex")->setValue(renderTexture);
	}

	void initTextures()
	{
		texture1 = DYNAMIC_CAST<Texture2D>(resManager->getOrLoadTexture("../data/freeman1.png"));
		texture1->generateMipmaps();
		texture1->setFilterMode(Filter::Linear, Filter::Linear);
		texture1->setAnisotropyLevel(8);

		texture2 = DYNAMIC_CAST<Texture2D>(resManager->getOrLoadTexture("../data/freeman2.png"));
		texture2->generateMipmaps();
		texture2->setFilterMode(Filter::Linear, Filter::Linear);
		texture2->setAnisotropyLevel(8);
	}

	void initMaterials()
	{
		textureEffect = resManager->getOrCreateEffect(vsBasic, fsTexture);
		textureMaterial = resManager->createMaterial(textureEffect);
		textureMaterial->setPolygonFace(PolygonFace::All);
		textureMaterial->getParameter("worldViewProjMatrix")->bindValue(AutoBinding::WorldViewProjectionMatrix);
		textureMaterial->getParameter("mainTex")->setValue(texture1);

		auto checkerEffect = resManager->getOrCreateEffect(vsBasic, fsChecker);
		checkerMaterial = resManager->createMaterial(checkerEffect);
		checkerMaterial->setPolygonFace(PolygonFace::All);
		checkerMaterial->getParameter("color")->setValue(Vector4(1, 1, 0, 1));
		checkerMaterial->getParameter("worldViewProjMatrix")->bindValue(AutoBinding::WorldViewProjectionMatrix);

		auto textureWithLightingEffect = resManager->getOrCreateEffect(vsBasicLighting, fsTextureWithLighting);
		texWithLightingMaterial = resManager->createMaterial(textureWithLightingEffect);
		texWithLightingMaterial->setPolygonFace(PolygonFace::All);
		texWithLightingMaterial->getParameter("worldViewProjMatrix")->bindValue(AutoBinding::WorldViewProjectionMatrix);
		texWithLightingMaterial->getParameter("normalMatrix")->bindValue(AutoBinding::InverseTransposedWorldMatrix);
		texWithLightingMaterial->getParameter("mainTex")->setValue(texture2);
	}

	void initBox(const Vector3& position, shared<Material> material)
	{
		auto node = createQuad();
		rebuildToBoxMesh(node);
		node->getComponent<ModelRenderer>()->setMaterial(0, material);
		node->getComponent<Transform>()->setLocalPosition(position);
		node->addComponent<RotatorAroundWorldYAxis>();
	}

	void initModel(const char* url, const Vector3& position, shared<Material> material)
	{
		auto mesh = engine->getResourceManager()->getOrLoadMesh(url);
		auto model = resManager->getOrCreateModel();
		model->addMesh(mesh);
		auto node = scene->createNode();
		auto renderer = node->addComponent<ModelRenderer>();
		renderer->setModel(model);
		renderer->setMaterial(0, material);
		node->getComponent<Transform>()->setLocalPosition(position);
		node->addComponent<RotatorAroundLocalXAxis>();
	}

	void initStaticQuad(const Vector3 &position, shared<Material> material)
	{
		auto quad = createQuad();
		quad->getComponent<ModelRenderer>()->setMaterial(0, material);
		quad->getComponent<Transform>()->setLocalPosition(position);

		auto canvasSize = device->getCanvasSize();
		quad->getComponent<Transform>()->setLocalScale(5, 5 * canvasSize.y / canvasSize.x, 1);
	}

	void initRotatingQuad(const Vector3& position, shared<Material> material)
	{
		auto empty = scene->createNode();
		auto emptyTransform = empty->getComponent<Transform>();
		emptyTransform->setLocalPosition(position);
		empty->addComponent<RotatorAroundWorldYAxis>();

		auto quad = createQuad();
		quad->addComponent<RotatorAroundLocalXAxis>();
		quad->getComponent<Transform>()->setParent(emptyTransform);
		quad->getComponent<Transform>()->setLocalPosition({1, 0, 0});
		quad->getComponent<ModelRenderer>()->setMaterial(0, material);
	}

	Node* createQuad()
	{
		auto mesh = resManager->getOrCreateMesh();
		mesh->setVertices(
		{
			{-1, -1, 0},
			{-1, 1, 0},
			{1, 1, 0},
			{1, -1, 0}
		});
		mesh->setNormals(
		{
			{0, 0, -1},
			{0, 0, -1},
			{0, 0, -1},
			{0, 0, -1}
		});
		mesh->setUVs(
		{
			{0, 0},
			{0, 1},
			{1, 1},
			{1, 0}
		});
		mesh->setIndices(
		{
			0, 1, 2,
			0, 2, 3
		});

		auto model = resManager->getOrCreateModel();
		model->addMesh(mesh);

		auto node = scene->createNode();
		auto renderer = node->addComponent<ModelRenderer>();
		renderer->setModel(model);
		return node;
	}

	// Check if the engine is capable of rebuilding meshes
	void rebuildToBoxMesh(Node* node)
	{
		auto model = node->getComponent<ModelRenderer>()->getModel();
		auto mesh = model->getMesh(0);
		mesh->setVertices(
		{
			{-1, -1, 1},	{-1, 1, 1},		{1, 1, 1},		{1, -1, 1}, // front face
			{-1, -1, -1},	{-1, 1, -1},	{-1, 1, 1},		{-1, -1, 1}, // left face
			{1, -1, -1},	{1, 1, -1},		{-1, 1, -1},	{-1, -1, -1}, // back face
			{1, -1, 1},		{1, 1, 1},		{1, 1, -1},		{1, -1, -1}, // right face
			{-1, 1, 1},		{-1, 1, -1},	{1, 1, -1},		{1, 1, 1}, // top face
			{-1, -1, -1},	{-1, -1, 1},	{1, -1, 1},		{1, -1, -1}, // bottom face

		});
		mesh->setUVs(
		{
			{0, 0}, {0, 1}, {1, 1}, {1, 0},
			{0, 0}, {0, 1}, {1, 1}, {1, 0},
			{0, 0}, {0, 1}, {1, 1}, {1, 0},
			{0, 0}, {0, 1}, {1, 1}, {1, 0},
			{0, 0}, {0, 1}, {1, 1}, {1, 0},
			{0, 0}, {0, 1}, {1, 1}, {1, 0},
		});
		mesh->setIndices(
		{
			0,	1,	2,	0,	2,	3,
			4,	5,	6,	4,	6,	7,
			8,	9,	10,	8,	10,	11,
			12,	13,	14,	12,	14,	15,
			16,	17,	18,	16,	18,	19,
			20,	21,	22,	20,	22,	23,
		});
	}

private:
	shared<Texture2D> texture1;
	shared<Texture2D> texture2;
	shared<Effect> textureEffect;
	shared<Material> textureMaterial;
	shared<Material> texWithLightingMaterial;
	shared<Material> checkerMaterial;
	shared<Material> renderTargetMaterial;
	ResourceManager* resManager;
};
