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
		createQuadInScreenSpace();
	}

	void createQuadInScreenSpace()
	{
		auto _scene = engine->getScene();
		auto effect = Effect::create(vsBasic, fsSimleColor);
		auto material = Material::create();
		material->addPass(effect);

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

		auto node = _scene->createNode();
		auto renderer = _scene->addComponent<ModelRenderer>(node);
		auto nodeTransform = _scene->getComponent<Transform>(node);
		nodeTransform->rotate(Vector3::unitX(), 0.5f, Transform::TransformSpace::Self);
		renderer->setModel(model);
		renderer->setMaterial(0, material);
		material->getParameter("color")->setValue(Vector4(0, 1, 0, 1));
		material->getParameter("worldViewProj")->bindValue(MaterialParameter::AutoBinding::WorldViewProjectionMatrix);
		_scene->addComponent<Rotator>(node);

		auto cameraNode = _scene->createNode();
		auto cameraTransform = _scene->getComponent<Transform>(cameraNode);
		cameraTransform->setLocalPosition(0, 0, 5);
		auto camera = _scene->addComponent<Camera>(cameraNode);
		camera->setPerspective(true);
		camera->setFOV(60);
		camera->setNear(1);
		camera->setFar(100);
		camera->setAspectRatio(1.7f);
		camera->setClearColor(0, 0.8f, 0.8f, 1);
	}

	class Rotator : public ComponentBase<Rotator>
	{
	public:
		explicit Rotator(size_t node):
			ComponentBase<Rotator>(node)
		{
			engine = Engine::get();
			transform = engine->getScene()->getComponent<Transform>(node);
		}

		virtual void update() override
		{
			auto angle1 = engine->getTimeDelta() * 5;
			auto angle2 = engine->getTimeDelta();
			transform->rotate(Vector3::unitZ(), angle1, Transform::TransformSpace::Self);
			transform->rotate(Vector3::unitY(), angle2, Transform::TransformSpace::World);
		}

	private:
		Engine *engine;
		ptr<Transform> transform;
	};
};