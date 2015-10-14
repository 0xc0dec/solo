#pragma once

#include "TestBase.h"

const char * vs = R"s(
	#version 330 core

	uniform float testFloat;
	uniform sampler2D testSampler;
	uniform float testArray[3];
	uniform vec3 testVector;

	void main()
	{
		float nonsense = testFloat * texture(testSampler, testVector.xy).x;
		gl_Position = vec4(nonsense, testArray[0], testArray[1], testArray[2]);
	}
)s";

const char *fs = R"s(
	#version 330 core

	layout (location = 0) out vec4 color;

	void main()
	{
		color = vec4(1, 1, 1, 1);
	}
)s";


class MaterialsTest : public TestBase
{
public:
	MaterialsTest(Engine *engine) : TestBase(engine)
	{
	}

	virtual void run() override
	{
		test_CreateEffect_FindVariables();
		test_CompileEffect_OK();
		test_CompileEffect_Fail();
	}

	void test_CreateEffect_FindVariables()
	{
		auto effect = resourceManager->getOrCreateEffect(vs, fs);
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

	void test_CompileEffect_OK()
	{
		resourceManager->getOrCreateEffect(vs, fs);
	}

	void test_CompileEffect_Fail()
	{
		testFailedCompilation("sdfsdf", fs, "vertex");
		testFailedCompilation(vs, "sdfsdf", "fragment");
	}

private:
	void testFailedCompilation(const std::string& vertex, const std::string& fragment, const std::string& failedShaderTypeName)
	{
		try
		{
			resourceManager->getOrCreateEffect(vertex, fragment);
		}
		catch (EffectCompilationException &e)
		{
			assert(
				e.what() == FORMAT("Failed to compile ", failedShaderTypeName, " shader") ||
				e.what() == FORMAT("Failed to link effect program")
			);
			return;
		}
		assert(false);
	}
};
