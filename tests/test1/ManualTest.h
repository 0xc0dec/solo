#pragma once

#include "TestBase.h"


const char *vsBasic =
	"#version 330 core\n"

	"layout (location = 0) in vec4 position;\n"

	"uniform mat4 worldViewProj;\n"

	"void main()\n"
	"{\n"
	"	gl_Position = worldViewProj * position;\n"
	"}";

const char *fsSimleColor =
	"#version 330 core\n"

	"uniform vec4 color;\n"
	"out vec4 fragColor;\n"

	"void main()\n"
	"{\n"
	"	fragColor = color;\n"
	"}\n";


class ManualTest : public TestBase
{
public:
	ManualTest(Engine *engine) : TestBase(engine)
	{
	}

	virtual void run() override
	{
		createQuad();
	}

	void createQuad()
	{
		auto resManager = engine->getResourceManager();
		auto scene = engine->getScene();
		auto effect = resManager->createEffect(vsBasic, fsSimleColor);
		auto material = resManager->createMaterial();
		material->addPass(effect);
		material->getParameter("color")->setValue(Vector4(0, 0, 1, 1));
		material->getParameter("worldViewProj")->bindValue(MaterialParameter::AutoBinding::WorldViewProjectionMatrix);

		auto model = resManager->createModel();
		auto mesh = resManager->createMesh(
		{
			{	-1,	-1,	0 },
			{	1,	1,	0 },
			{	1,	-1,	0 },
			{	-1,	-1,	0 },
			{	-1,	1,	0 },
			{	1,	1,	0 }
		},
		{
			{ 0, 0, -1 },
			{ 0, 0, -1 },
			{ 0, 0, -1 },
			{ 0, 0, -1 },
			{ 0, 0, -1 },
			{ 0, 0, -1 }
		}, {});
		model->addMesh(mesh);

		auto empty = scene->createNode();
		auto emptyTransform = empty->getComponent<Transform>();
		empty->addComponent<RotatorAroundWorldAxis>();

		auto quad = scene->createNode();
		auto quadRenderer = quad->addComponent<ModelRenderer>();
		auto quadTransform = quad->getComponent<Transform>();
		quad->addComponent<RotatorAroundLocalAxis>();
		quadTransform->setParent(emptyTransform);
		quadTransform->setLocalPosition(1, 0, 0);
		quadRenderer->setModel(model);
		quadRenderer->setMaterial(0, material);

		auto cameraNode = scene->createNode();
		auto cameraTransform = cameraNode->getComponent<Transform>();
		cameraTransform->setLocalPosition(0, 0, 5);
		auto camera = cameraNode->addComponent<Camera>();
		camera->setClearColor(0, 0.8f, 0.8f, 1);
	}

	class RotatorAroundWorldAxis : public ComponentBase<RotatorAroundWorldAxis>
	{
	public:
		explicit RotatorAroundWorldAxis(Node* node):
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
};