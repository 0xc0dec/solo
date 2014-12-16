#include <iostream>
#include "../../include/Solo.h"

using namespace solo;

#define LOG(msg) std::cout << msg << std::endl;

const char * testVertexShader = 
	"#version 330 core\n"

	"layout(location = 0) in vec3 vertexPositionModelSpace;"
	"layout(location = 1) in vec2 vertexUV;"

	"void main()\n"
	"{\n"
	"	gl_Position = vec4(vertexPositionModelSpace, 1);\n"
	"}";

const char *testFragmentShader = 
	"#version 330 core\n"

	"out vec3 color;\n"

	"void main()\n"
	"{\n"
	"	color = vec3(1);\n"
	"}";


class Callback : public IEngineCallback
{
public:
	explicit Callback(IEngine* i_engine)
		: _engine(i_engine)
	{
	}

	void processEngineStartedEvent() override
	{
		_engine->getDevice()->setWindowTitle("Test title");
		
		auto program = _engine->getDevice()->createGPUProgram(testVertexShader, testFragmentShader);
		LOG("Program is valid: " << program->valid());
		LOG("Program compilation log: " << program->log());
	}

	void processEngineStoppedEvent() override
	{
	}

	void processBeforeFrameEvent() override
	{
	}

	bool processDeviceCloseRequestedEvent() override
	{
		return true;
	}

private:
	IEngine *_engine;
};


int main(int argc, char *argv[])
{
	auto engine = getEngine();
	EngineCreationArgs engineArgs = { 800, 600, 32, 24, false };
	Callback callback(engine);
	engine->setCallback(&callback);
	engine->run(engineArgs);
	return 0;
}
