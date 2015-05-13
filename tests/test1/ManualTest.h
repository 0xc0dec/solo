#pragma once

#include "TestBase.h"
#include "Shaders.h"


class InputWatcher : public ComponentBase<InputWatcher>
{
public:
	explicit InputWatcher(Node* node) : ComponentBase<InputWatcher>(node)
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
	SpectatorCamera(Node* node) : ComponentBase<SpectatorCamera>(node)
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
				transform->rotate(Vector3::unitY(), 0.5f * dt * -mouseMotion.x, Transform::TransformSpace::World);
			if (mouseMotion.y != 0)
				transform->rotate(Vector3::unitX(), 0.5f * dt * -mouseMotion.y, Transform::TransformSpace::Self);
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
	Device *device;
};


class RotatorAroundWorldYAxis : public ComponentBase<RotatorAroundWorldYAxis>
{
public:
	explicit RotatorAroundWorldYAxis(Node* node) :
		ComponentBase<RotatorAroundWorldYAxis>(node)
	{
		engine = Engine::get();
		transform = node->getComponent<Transform>();
	}

	virtual void update() override
	{
		auto angle = engine->getTimeDelta();
		transform->rotate(Vector3::unitY(), angle, Transform::TransformSpace::World);
	}

private:
	Engine *engine;
	Transform* transform;
};


class RotatorAroundLocalXAxis : public ComponentBase<RotatorAroundLocalXAxis>
{
public:
	explicit RotatorAroundLocalXAxis(Node* node) :
		ComponentBase<RotatorAroundLocalXAxis>(node)
	{
		engine = Engine::get();
		transform = node->getComponent<Transform>();
	}

	virtual void update() override
	{
		auto angle = engine->getTimeDelta() * 1.3f;
		transform->rotate(Vector3::unitX(), angle, Transform::TransformSpace::Self);
	}

private:
	Engine *engine;
	Transform* transform;
};


class ManualTest : public TestBase
{
public:
	ManualTest(Engine *engine) : TestBase(engine)
	{
		resManager = engine->getResourceManager();
	}

	virtual void run() override
	{
		createMaterials();
		createAndPlaceQuad1();
		createAndPlaceQuad2();
		createAndPlaceBox();
		loadAndPlaceModel();
		createCamera();
	}

	void createMaterials()
	{
		auto canvasSize = device->getCanvasSize();

		auto effRare = resManager->getOrCreateEffect(vsBasic, fsRare);
		matRare = resManager->getOrCreateMaterial(effRare);
		matRare->getParameter("worldViewProj")->bindValue(MaterialParameter::AutoBinding::WorldViewProjectionMatrix);
		matRare->getParameter("canvasWidth")->setValue(canvasSize.x);
		matRare->getParameter("canvasHeight")->setValue(canvasSize.y);
		matRare->getParameter("time")->bindValue<float>([this](const RenderContext& context) -> float { return this->device->getLifetime(); });

		auto effTexture = resManager->getOrCreateEffect(vsBasic, fsTexture);
		auto texture = DYNAMIC_CAST<Texture2D>(resManager->getOrLoadTexture("../data/Freeman.png"));
		texture->generateMipmaps();
		texture->setFilterMode(Texture2D::Filter::Linear, Texture2D::Filter::Linear);
		texture->setAnisotropyLevel(8);
		matTexture = resManager->getOrCreateMaterial(effTexture);
		matTexture->setPolygonFace(RenderState::PolygonFace::All);
		matTexture->getParameter("worldViewProj")->bindValue(MaterialParameter::AutoBinding::WorldViewProjectionMatrix);
		matTexture->getParameter("mainTex")->setValue(texture);

		auto effChecker = resManager->getOrCreateEffect(vsBasic, fsChecker);
		matChecker = resManager->getOrCreateMaterial(effChecker);
		matChecker->setPolygonFace(RenderState::PolygonFace::All);
		matChecker->getParameter("color")->setValue(Vector4(1, 1, 0, 1));
		matChecker->getParameter("worldViewProj")->bindValue(MaterialParameter::AutoBinding::WorldViewProjectionMatrix);
	}

	void createAndPlaceBox()
	{
		auto node = createQuad();
		rebuildQuadToBox(node);
		node->getComponent<ModelRenderer>()->setMaterial(0, matChecker);
		node->getComponent<Transform>()->setLocalPosition(-3, 0, 0);
		node->addComponent<RotatorAroundWorldYAxis>();
	}

	void loadAndPlaceModel()
	{
		auto mesh = engine->getResourceManager()->getOrLoadMesh("../data/monkey_nouv.obj");
		auto model = resManager->getOrCreateModel();
		model->addMesh(mesh);
		auto node = scene->createNode();
		auto renderer = node->addComponent<ModelRenderer>();
		renderer->setModel(model);
		renderer->setMaterial(0, matChecker);
	}

	void createAndPlaceQuad2()
	{
		auto quad = createQuad();
		quad->getComponent<ModelRenderer>()->setMaterial(0, matRare);
		quad->getComponent<Transform>()->setLocalPosition(0, 0, -10);

		auto canvasSize = device->getCanvasSize();
		quad->getComponent<Transform>()->setLocalScale(5, 5 * canvasSize.y / canvasSize.x, 1);
	}

	void createAndPlaceQuad1()
	{
		auto empty = scene->createNode();
		auto emptyTransform = empty->getComponent<Transform>();
		emptyTransform->setLocalPosition(3, 0, 0);
		empty->addComponent<RotatorAroundWorldYAxis>();

		auto quad = createQuad();
		quad->addComponent<RotatorAroundLocalXAxis>();
		quad->getComponent<Transform>()->setParent(emptyTransform);
		quad->getComponent<Transform>()->setLocalPosition(1, 0, 0);
		quad->getComponent<ModelRenderer>()->setMaterial(0, matTexture);
	}

	void createCamera()
	{
		auto cameraNode = scene->createNode();
		auto cameraTransform = cameraNode->getComponent<Transform>();
		cameraTransform->setLocalPosition(0, 0, 10);
		cameraNode->addComponent<SpectatorCamera>();
		cameraNode->addComponent<InputWatcher>();
		auto camera = cameraNode->addComponent<Camera>();
		camera->setClearColor(0, 0.6f, 0.6f, 1);
	}

	Node* createQuad()
	{
		auto mesh = resManager->getOrCreateMesh();
		mesh->setVertices(
		{
			{ -1, -1, 0 },
			{ -1,  1, 0 },
			{  1,  1, 0 },
			{  1, -1, 0 }
		});
		mesh->setNormals(
		{
			{ 0, 0, -1 },
			{ 0, 0, -1 },
			{ 0, 0, -1 },
			{ 0, 0, -1 }
		});
		mesh->setUVs(
		{
			{ 0, 0 },
			{ 0, 1 },
			{ 1, 1 },
			{ 1, 0 }
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
	void rebuildQuadToBox(Node *quadNode)
	{
		auto model = quadNode->getComponent<ModelRenderer>()->getModel();
		auto mesh = model->getMesh(0);
		mesh->setVertices(
		{
			{ -1, -1,  1 }, { -1,  1,  1 }, {  1,  1,  1 }, {  1, -1,  1 }, // front face
			{ -1, -1, -1 }, { -1,  1, -1 }, { -1,  1,  1 }, { -1, -1,  1 }, // left face
			{  1, -1, -1 }, {  1,  1, -1 }, { -1,  1, -1 }, { -1, -1, -1 }, // back face
			{  1, -1,  1 }, {  1,  1,  1 }, {  1,  1, -1 }, {  1, -1, -1 }, // right face
			{ -1,  1,  1 }, { -1,  1, -1 }, {  1,  1, -1 }, {  1,  1,  1 }, // top face
			{ -1, -1, -1 }, { -1, -1,  1 }, {  1, -1,  1 }, {  1, -1, -1 }, // bottom face

		});
		mesh->setUVs(
		{
			{ 0, 0 }, { 0, 1 }, { 1, 1 }, { 1, 0 },
			{ 0, 0 }, { 0, 1 }, { 1, 1 }, { 1, 0 },
			{ 0, 0 }, { 0, 1 }, { 1, 1 }, { 1, 0 },
			{ 0, 0 }, { 0, 1 }, { 1, 1 }, { 1, 0 },
			{ 0, 0 }, { 0, 1 }, { 1, 1 }, { 1, 0 },
			{ 0, 0 }, { 0, 1 }, { 1, 1 }, { 1, 0 },
		});
		mesh->setIndices(
		{
			 0,  1,  2,		 0,  2,  3,
			 4,  5,  6,		 4,  6,  7,
			 8,  9, 10,		 8, 10, 11,
			12, 13, 14,		12, 14, 15,
			16, 17, 18,		16, 18, 19,
			20, 21, 22,		20, 22, 23,
		});
	}

private:
	shared<Material> matChecker;
	shared<Material> matRare;
	shared<Material> matTexture;
	ResourceManager* resManager;
};