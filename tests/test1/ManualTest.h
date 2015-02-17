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
				transform->rotate(Vector3::unitY(), dt * -mouseMotion.x, Transform::TransformSpace::World);
			if (mouseMotion.y != 0)
				transform->rotate(Vector3::unitX(), dt * -mouseMotion.y, Transform::TransformSpace::Self);
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


class RotatorAroundWorldAxis : public ComponentBase<RotatorAroundWorldAxis>
{
public:
	explicit RotatorAroundWorldAxis(Node* node) :
		ComponentBase<RotatorAroundWorldAxis>(node)
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


class RotatorAroundLocalAxis : public ComponentBase<RotatorAroundLocalAxis>
{
public:
	explicit RotatorAroundLocalAxis(Node* node) :
		ComponentBase<RotatorAroundLocalAxis>(node)
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
		createAndPlaceQuad1();
		createAndPlaceQuad2();
		createCamera();
	}

	void createAndPlaceQuad2()
	{
		auto canvasSize = device->getCanvasSize();

		auto effect = resManager->getEffect(vsBasic, fsRare);
		auto material = resManager->getMaterial();
		material->addPass(effect);
		material->getParameter("worldViewProj")->bindValue(MaterialParameter::AutoBinding::WorldViewProjectionMatrix);
		material->getParameter("canvasWidth")->setValue(canvasSize.x);
		material->getParameter("canvasHeight")->setValue(canvasSize.y);
		material->getParameter("time")->bindValue<float>([this](const RenderContext& context) -> float { return this->device->getLifetime(); });

		auto quad = createQuad();
		quad->getComponent<ModelRenderer>()->setMaterial(0, material);
		quad->getComponent<Transform>()->setLocalPosition(0, 0, -10);
		quad->getComponent<Transform>()->setLocalScale(5, 5 * canvasSize.y / canvasSize.x, 1);
	}

	void createAndPlaceQuad1()
	{
		auto effect = resManager->getEffect(vsBasic, fsChecker);
		auto material = resManager->getMaterial();
		material->addPass(effect);
		material->getParameter("color")->setValue(Vector4(1, 1, 0, 1));
		material->getParameter("worldViewProj")->bindValue(MaterialParameter::AutoBinding::WorldViewProjectionMatrix);

		auto empty = scene->createNode();
		auto emptyTransform = empty->getComponent<Transform>();
		empty->addComponent<RotatorAroundWorldAxis>();

		auto quad = createQuad();
		quad->addComponent<RotatorAroundLocalAxis>();
		quad->getComponent<Transform>()->setParent(emptyTransform);
		quad->getComponent<Transform>()->setLocalPosition(1, 0, 0);
		quad->getComponent<ModelRenderer>()->setMaterial(0, material);
	}

	void createCamera()
	{
		auto cameraNode = scene->createNode();
		auto cameraTransform = cameraNode->getComponent<Transform>();
		cameraTransform->setLocalPosition(0, 0, 5);
		cameraNode->addComponent<SpectatorCamera>();
		cameraNode->addComponent<InputWatcher>();
		auto camera = cameraNode->addComponent<Camera>();
		camera->setClearColor(0, 0.6f, 0.6f, 1);
	}

	Node* createQuad()
	{
		auto mesh = resManager->getMesh();
		mesh->setVertices(
		{
			{ -1, -1, 0 },
			{ -1, 1, 0 },
			{ 1, 1, 0 },
			{ 1, -1, 0 }
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

		auto quadModel = resManager->getModel();
		quadModel->addMesh(mesh);

		auto node = scene->createNode();
		auto renderer = node->addComponent<ModelRenderer>();
		renderer->setModel(quadModel);
		return node;
	}

private:
	ResourceManager* resManager;
};