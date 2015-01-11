#pragma once

#include "TestBase.h"

const char * vs =
	"#version 330 core\n"

	"uniform float testFloat;\n"
	"uniform sampler2D testSampler;\n"
	"uniform float testArray[3];\n"
	"uniform vec3 testVector;\n"

	"void main()\n"
	"{\n"
	"	float nonsense = testFloat * texture(testSampler, testVector.xy).x;\n"
	"	gl_Position = vec4(nonsense, testArray[0], testArray[1], testArray[2]);\n"
	"}";

const char *fs =
	"#version 330 core\n"

	"layout (location = 0) out vec4 color;\n"

	"void main()\n"
	"{\n"
	"	color = vec4(1, 1, 1, 1);\n"
	"}";


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


class MaterialsTest : public TestBase
{
public:
	MaterialsTest(Engine *engine) : TestBase(engine)
	{
	}

	virtual void run() override
	{
		testEffectCompilation();
		testEffectVariablesDetection();
		testRenderQuadInScreenCoords();
	}

	void testRenderQuadInScreenCoords()
	{
		auto _scene = _engine->getScene();
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

	void testEffectVariablesDetection()
	{
		auto effect = Effect::create(vs, fs);
		auto var1 = effect->findVariable("testFloat");
		auto var2 = effect->findVariable("testSampler");
		auto var3 = effect->findVariable("testArray");
		assert(var1);
		assert(var2);
		assert(var3);
		assert(var1->getName() == "testFloat");
		assert(var2->getName() == "testSampler");
		assert(var3->getName() == "testArray");
	}

	void testEffectCompilation()
	{
		auto effect = Effect::create(vs, fs);
		assert(effect->isValid());
		assert(effect->getLog().empty());
	}
};
