#pragma once

#include "TestBase.h"


const char *vs = R"s(
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


class Materials_Test final: public TestBase
{
public:
	Materials_Test(Device* device): TestBase(device)
	{
	}

	void run() override final
	{
		test_NormalEffectCompilation();
		test_FailedEffectCompilation();
	}

private:
    void test_NormalEffectCompilation()
    {
        Effect::create(vs, fs);
    }

    void test_FailedEffectCompilation()
    {
        testFailedCompilation("sdfsdf", fs, "vertex");
        testFailedCompilation(vs, "sdfsdf", "fragment");
    }

	void testFailedCompilation(const std::string& vertex, const std::string& fragment, const std::string& failedShaderTypeName)
	{
		try
		{
            Effect::create(vertex, fragment);
		}
		catch (AssetException &e)
		{
			assert(
				e.what() == SL_FMT("Failed to compile ", failedShaderTypeName, " shader") ||
				e.what() == SL_FMT("Failed to link program")
			);
			return;
		}
		assert(false);
	}
};
