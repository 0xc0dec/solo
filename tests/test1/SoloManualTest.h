#pragma once

#include "TestBase.h"


const char *vsBasic =
	"#version 330 core\n"

	"layout (location = 0) in vec4 position;\n"

	"void main()\n"
	"{\n"
	"	gl_Position = position;\n"
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
			{ -0.95f, -0.95f, 0 },
			{ 0.95f, 0.95f, 0 },
			{ 0.95f, -0.95f, 0 },
			{ -0.95f, -0.95f, 0 },
			{ -0.95f, 0.95f, 0 },
			{ 0.95f, 0.95f, 0 }
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
		renderer->setModel(model);
		renderer->setMaterial(0, material);
		material->getParameter("color")->setValue(Vector4(0, 1, 0, 1));

		auto cameraNode = _scene->createNode();
		auto camera = _scene->addComponent<Camera>(cameraNode);
		camera->setClearColor(0, 0, 1, 1);
	}
};