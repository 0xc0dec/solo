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
		auto _scene = engine->getScene();
		auto effect = Effect::create(vsBasic, fsSimleColor);
		auto material = Material::create();
		material->addPass(effect);
		material->getParameter("color")->setValue(Vector4(0, 1, 0, 1));
		material->getParameter("worldViewProj")->bindValue(MaterialParameter::AutoBinding::WorldViewProjectionMatrix);

		auto model = Model::create();
		auto mesh = Mesh::create(
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
		});
		model->addMesh(mesh);

		auto empty = _scene->createNode();
		auto emptyTransform = _scene->getComponent<Transform>(empty);
		_scene->addComponent<RotatorAroundWorldAxis>(empty);

		auto quad = _scene->createNode();
		auto quadRenderer = _scene->addComponent<ModelRenderer>(quad);
		auto quadTransform = _scene->getComponent<Transform>(quad);
		quadTransform->setParent(emptyTransform.get());
		quadTransform->setLocalPosition(1, 0, 0);
		quadRenderer->setModel(model);
		quadRenderer->setMaterial(0, material);
		_scene->addComponent<RotatorAroundLocalAxis>(quad);

		auto cameraNode = _scene->createNode();
		auto cameraTransform = _scene->getComponent<Transform>(cameraNode);
		cameraTransform->setLocalPosition(0, 0, 5);
		auto camera = _scene->addComponent<Camera>(cameraNode);
		camera->setClearColor(0, 0.8f, 0.8f, 1);
	}

	class RotatorAroundWorldAxis : public ComponentBase<RotatorAroundWorldAxis>
	{
	public:
		explicit RotatorAroundWorldAxis(size_t node):
			ComponentBase<RotatorAroundWorldAxis>(node)
		{
			engine = Engine::get();
			transform = engine->getScene()->getComponent<Transform>(node);
		}

		virtual void update() override
		{
			auto angle = engine->getTimeDelta();
			transform->rotate(Vector3::unitY(), angle, Transform::TransformSpace::World);
		}

	private:
		Engine *engine;
		ptr<Transform> transform;
	};

	class RotatorAroundLocalAxis : public ComponentBase<RotatorAroundLocalAxis>
	{
	public:
		explicit RotatorAroundLocalAxis(size_t node) :
			ComponentBase<RotatorAroundLocalAxis>(node)
		{
			engine = Engine::get();
			transform = engine->getScene()->getComponent<Transform>(node);
		}

		virtual void update() override
		{
			auto angle = engine->getTimeDelta() * 0.3f;
			transform->rotate(Vector3::unitX(), angle, Transform::TransformSpace::Self);
		}

	private:
		Engine *engine;
		ptr<Transform> transform;
	};
};