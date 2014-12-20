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
	virtual size_t getTypeId() override
	{
		return solo::getTypeId<TestComponent>();
	}
};


class TestComponent2 : public IComponent
{
public:
	virtual size_t getTypeId() override
	{
		return solo::getTypeId<TestComponent2>();
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
	explicit Callback(IEngine* engine)
		: _engine(engine)
	{
	}

	virtual void onEngineStarted() override
	{
		_engine->getDevice()->setWindowTitle("Test title");

		auto scene = _engine->getScene();
		scene->addSystem<TestSystem>(getTypeId<TestComponent>());
		auto node = scene->createNode();

		auto cmp = scene->addComponent<TestComponent>(node);
		auto cmp2 = scene->addComponent<TestComponent2>(node);
		LOG("Created component " << cmp->getTypeId());
		LOG("Created component " << cmp2->getTypeId());

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
	std::cin.get();
	return 0;
}
