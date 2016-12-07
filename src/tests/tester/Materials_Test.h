/*
    Copyright (c) Aleksey Fedotov

    This software is provided 'as-is', without any express or implied
    warranty. In no event will the authors be held liable for any damages
    arising from the use of this software.

    Permission is granted to anyone to use this software for any purpose,
    including commercial applications, and to alter it and redistribute it
    freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not
        claim that you wrote the original software. If you use this software
        in a product, an acknowledgment in the product documentation would be
        appreciated but is not required.
    2. Altered source versions must be plainly marked as such, and must not be
        misrepresented as being the original software.
    3. This notice may not be removed or altered from any source distribution.
*/

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
        Effect::create(device, vs, fs);
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
            Effect::create(device, vertex, fragment);
        }
        catch (EngineException &e)
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
