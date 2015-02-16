#pragma once

#include "TestBase.h"

const char *vsBasic =
" \
#version 330 core\n \
\
layout (location = 0) in vec4 position;\n \
layout (location = 2) in vec2 uv;\n \
\
uniform mat4 worldViewProj;\n \
\
out vec2 uv0;\n \
\
void main()\n \
{\n \
	gl_Position = worldViewProj * position;\n \
	uv0 = uv;\n \
}";

const char *fsChecker =
" \
#version 330 core\n \
\
in vec2 uv0;\n \
\
uniform vec4 color;\n \
out vec4 fragColor;\n \
\
void main()\n \
{\n \
	float xfloor = floor(uv0.x / 0.2);\n \
	float yfloor = floor(uv0.y / 0.2) + 1;\n \
	if (mod(xfloor, 2) == 0 && mod(yfloor, 2) == 0)\n \
		fragColor = vec4(0, 0, 0, 1);\n \
	else if (mod(xfloor, 2) > 0 && mod(yfloor, 2) > 0)\n \
		fragColor = vec4(0, 0, 0, 1);\n \
	else\n \
		fragColor = color;\n \
}";

const char* fsRare =
" \
#version 330 core\n \
#define sphsize .15\n \
const float dist = 2.;\n \
const float perturb = .2;\n \
const float windspeed = .25;\n \
const float steps = 150.;\n \
const float stepsize = .03;\n \
const float brightness = .2;\n \
const float fade = .003;\n \
const float glow = 2.5;\n \
const int iterations = 10;\n \
const float fractparam = .85;\n \
const vec3 offset = vec3(1.5, 2., -0.);\n \
const vec3 planetcolor = vec3(1., 1., .5);\n \
\
out vec4 fragColor;\n \
uniform float time;\n \
uniform float canvasWidth;\n \
uniform float canvasHeight;\n \
\
float rand(vec2 p) {return fract(sin(dot(p,vec2(2132.342,4323.343)))*1325.2158);}\n \
\
float surf(vec2 p)\n \
{\n \
	p = abs(fract((p + vec2(.5, .3))*vec2(.2, .4)) - .5);\n \
	float av = 0.;\n \
	for (int i = 0; i<15; i++) {\n \
		p = abs(p) / dot(p, p) - .65;\n \
		av += min(20., length(p*p));\n \
	}\n \
	return av*.1;\n \
}\n \
\
float wind(vec3 p, float a)\n \
{\n \
	mat2 rota = mat2(cos(a), sin(a), -sin(a), cos(a));\n \
	float d = max(0., dist - max(0., length(p*p*p) - sphsize) / sphsize) / dist;\n \
	p -= d*normalize(p * p * p)*perturb;\n \
	p += vec3(0., -time * windspeed, 0.);\n \
	p.xy *= rota;\n \
	p = abs(fract((p + offset)*.1) - .5);\n \
	for (int i = 0; i<iterations; i++)\n \
		p = abs(p) / dot(p, p) - fractparam;\n \
	return pow(length(p), 1.5)*(1. + d*glow) + d*glow;\n \
}\n \
\
void main()\n \
{\n \
	vec2 uv = gl_FragCoord.xy / vec2(canvasWidth, canvasHeight)-.5;\n \
	vec3 dir = vec3(uv, 1.);\n \
	dir.x *= canvasWidth / canvasHeight;\n \
	vec3 from = vec3(sin(time * .5), 0., -2.);\n \
	float v = 0., l = -0.0001, t = time * windspeed*.2;\n \
	for (float r = 10.; r<steps; r++)\n \
	{\n \
		vec3 p = from + r*dir*stepsize;\n \
		float k = min(200., wind(p, r*0.1));\n \
		v += k*max(0., 1. - r*fade);\n \
		l = pow(max(.2, dot(normalize(p*p), normalize(vec3(-1., 1.5, -0.5)))), 2.);\n \
	}\n \
	v /= steps; v *= brightness;\n \
	vec3 col = vec3(v*v, v*v*v, v*1.5) + l*planetcolor;\n \
	fragColor = vec4(col, 1.0);\n \
}";


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