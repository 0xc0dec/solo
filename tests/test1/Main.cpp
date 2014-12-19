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


class TestComponent : public IComponent
{
public:
	static size_t getComponentTypeId()
	{
		static auto hash = computeHash("TestComponent");
		return hash;
	}

	size_t getTypeId() override
	{
		return getComponentTypeId();
	}
};


class TestSystem : public ISystem
{
public:
	void update(size_t node, ptr<IComponent> component) override
	{
		LOG("Updating test system, node: " << node << ", component: " << component->getTypeId());
	}
};


class Callback : public IEngineCallback
{
public:
	explicit Callback(IEngine* i_engine)
		: _engine(i_engine)
	{
	}

	virtual void onEngineStarted() override
	{
		_engine->getDevice()->setWindowTitle("Test title");

		_engine->getScene()->addSystem<TestSystem>(TestComponent::getComponentTypeId());
		auto node = _engine->getScene()->createNode();
		auto cmp = _engine->getScene()->addComponent<TestComponent>(node);
		LOG("Created component " << cmp->getTypeId());

		auto program = _engine->getDevice()->createGPUProgram(testVertexShader, testFragmentShader);
		LOG("Program is valid: " << program->valid());
		LOG("Program compilation log: " << program->log());
	}

	virtual void onEngineStopped() override
	{
	}

	virtual void onBeforeFrame() override
	{
	}

	virtual bool onDeviceCloseRequested() override
	{
		return true;
	}

private:
	IEngine *_engine;
};


int main()
{
	auto engine = getEngine();
	EngineCreationArgs engineArgs = { 1366, 768, 32, 24, false };
	Callback callback(engine);
	engine->setCallback(&callback);
	engine->run(engineArgs);
	return 0;
}
