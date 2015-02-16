#pragma once

#include "TestBase.h"

const char *vsBasic =
"#version 330 core\n"

"layout (location = 0) in vec4 position;\n"
"layout (location = 2) in vec2 uv;\n"

"uniform mat4 worldViewProj;\n"

"out vec2 uv0;\n"

"void main()\n"
"{\n"
"	gl_Position = worldViewProj * position;\n"
"	uv0 = uv;\n"
"}";

const char *fsSimleColor =
"#version 330 core\n"

"in vec2 uv0;\n"

"uniform vec4 color;\n"
"out vec4 fragColor;\n"

"void main()\n"
"{\n"
"	float xfloor = floor(uv0.x / 0.2);\n"
"	float yfloor = floor(uv0.y / 0.2) + 1;\n"
"	if (mod(xfloor, 2) == 0 && mod(yfloor, 2) == 0)\n"
"		fragColor = vec4(0, 0, 0, 1);\n"
"	else if (mod(xfloor, 2) > 0 && mod(yfloor, 2) > 0)\n"
"		fragColor = vec4(0, 0, 0, 1);\n"
"	else\n"
"		fragColor = color;\n"
"}\n";


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
		{
			device->requestShutdown();
		}
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
				transform->rotate(Vector3::unitY(), dt * 0.5f * -mouseMotion.x, Transform::TransformSpace::World);
			if (mouseMotion.y != 0)
				transform->rotate(Vector3::unitX(), dt * 0.5f * -mouseMotion.y, Transform::TransformSpace::Self);
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
		scene = engine->getScene();
	}

	virtual void run() override
	{
		createQuad();
	}

	void createQuad()
	{
		auto effect = resManager->getEffect(vsBasic, fsSimleColor);
		auto material = resManager->getMaterial();
		material->addPass(effect);
		material->getParameter("color")->setValue(Vector4(1, 1, 0, 1));
		material->getParameter("worldViewProj")->bindValue(MaterialParameter::AutoBinding::WorldViewProjectionMatrix);

		auto quadModel = resManager->getModel();
		auto quadMesh = createQuadMesh();
		quadModel->addMesh(quadMesh);

		auto empty = scene->createNode();
		auto emptyTransform = empty->getComponent<Transform>();
		empty->addComponent<RotatorAroundWorldAxis>();
		empty->addComponent<InputWatcher>();

		auto quad = scene->createNode();
		auto quadRenderer = quad->addComponent<ModelRenderer>();
		auto quadTransform = quad->getComponent<Transform>();
		quad->addComponent<RotatorAroundLocalAxis>();
		quadTransform->setParent(emptyTransform);
		quadTransform->setLocalPosition(1, 0, 0);
		quadRenderer->setModel(quadModel);
		quadRenderer->setMaterial(0, material);

		auto cameraNode = scene->createNode();
		auto cameraTransform = cameraNode->getComponent<Transform>();
		cameraTransform->setLocalPosition(0, 0, 5);
		cameraNode->addComponent<SpectatorCamera>();
		auto camera = cameraNode->addComponent<Camera>();
		camera->setClearColor(0, 0.6f, 0.6f, 1);
	}

	shared<Mesh> createQuadMesh()
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
		return mesh;
	}

private:
	ResourceManager* resManager;
	Scene* scene;
};