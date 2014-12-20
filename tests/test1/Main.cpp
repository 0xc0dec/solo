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
	DECLARE_CMP_ID(TestComponent)
};


class TestComponent2 : public IComponent
{
public:
	DECLARE_CMP_ID(TestComponent2)
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
		scene->addSystem<TestSystem>(TestComponent::getComponentTypeId());
		auto node = scene->createNode();

		auto cmp = scene->addComponent<TestComponent>(node);
		auto cmp2 = scene->addComponent<TestComponent2>(node);
		LOG("Created component " << cmp->getTypeId());
		LOG("Created component " << cmp2->getTypeId());

		auto program = _engine->getDevice()->createGPUProgram(testVertexShader, testFragmentShader);
		LOG("Program is valid: " << program->isValid());
		LOG("Program compilation log: " << program->getLog());
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

template <typename T>
void f()
{
}

#define CALL(Type) f<Type>()


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
