#include "Demo_Test.h"


auto vsBasic = R"s(
	#version 330 core

	layout (location = 0) in vec4 position;
	layout (location = 1) in vec3 normal;
	layout (location = 2) in vec2 uv;

	uniform mat4 worldViewProjMatrix;
	out vec2 uv0;
	out vec3 n;

	void main()
	{
		gl_Position = worldViewProjMatrix * position;
		uv0 = uv;
	}
)s";

auto fsColor = R"s(
	#version 330 core

	uniform vec4 color;

	in vec2 uv0;
	out vec4 fragColor;

	void main()
	{
		fragColor = color;
	}
)s";

auto fsTexture = R"s(
	#version 330 core

	uniform sampler2D mainTex;

	in vec2 uv0;
	out vec4 fragColor;

	void main()
	{
		fragColor = texture(mainTex, uv0);
	}
)s";

auto fsChecker = R"s(
	#version 330 core

	in vec2 uv0;
	uniform vec4 color;
	out vec4 fragColor;

	void main()
	{
		float xfloor = floor(uv0.x / 0.2);
		float yfloor = floor(uv0.y / 0.2) + 1;
		if (mod(xfloor, 2) == 0 && mod(yfloor, 2) == 0)
			fragColor = vec4(0, 0, 0, 1);
		else if (mod(xfloor, 2) > 0 && mod(yfloor, 2) > 0)
			fragColor = vec4(0, 0, 0, 1);
		else
			fragColor = color;
	}
)s";

auto vsBasicLighting = R"s(
	#version 330 core

	layout (location = 0) in vec4 position;
	layout (location = 1) in vec3 normal;
	layout (location = 2) in vec2 uv;

	uniform mat4 worldViewProjMatrix;
	uniform mat4 normalMatrix;
	out vec2 uv0;
	out vec3 n;

	void main()
	{
		gl_Position = worldViewProjMatrix * position;
		uv0 = uv;
		n = normalize((normalMatrix * vec4(normal, 1)).xyz);
	}
)s";


auto fsTextureWithLighting = R"s(
	#version 330 core

	uniform sampler2D mainTex;

	in vec2 uv0;
	in vec3 n;
	out vec4 fragColor;

	void main()
	{
		vec4 color = texture(mainTex, uv0);
		fragColor = color * dot(vec3(1, 1, 1), n) / (length(vec3(1, 1, 1)) * length(n));
	}
)s";


class EscapeWatcher: public ComponentBase<EscapeWatcher>
{

public:
	explicit EscapeWatcher(Node node, Device *device) :
		ComponentBase(node),
		device(device)
	{
	}

	virtual void update() override
	{
		if (device->isKeyPressed(KeyCode::Escape, true))
			device->requestShutdown();
	}

private:
	Device *device;
};


class Spectator: public ComponentBase<Spectator>
{
public:
	explicit Spectator(Node node, Device *device):
		ComponentBase(node),
		transform(nullptr),
		device(device)
	{
	}

	virtual void init() override
	{
		transform = node.getComponent<Transform>();
	}

	virtual void update() override
	{
		auto mouseMotion = device->getMouseMotion();
		auto dt = device->getTimeDelta();

		if (device->isMouseButtonDown(MouseButton::Right, true))
			device->setCursorCaptured(true);
		if (device->isMouseButtonReleased(MouseButton::Right))
			device->setCursorCaptured(false);

		if (device->isMouseButtonDown(MouseButton::Right, false))
		{
			if (mouseMotion.x != 0)
				transform->rotate(Vector3::unitY(), 0.5f * dt * -mouseMotion.x, TransformSpace::World);
			if (mouseMotion.y != 0)
				transform->rotate(Vector3::unitX(), 0.5f * dt * -mouseMotion.y, TransformSpace::Self);
		}

		Vector3 movement;
		if (device->isKeyPressed(KeyCode::W, false))
			movement += transform->getLocalForward();
		if (device->isKeyPressed(KeyCode::S, false))
			movement += transform->getLocalBack();
		if (device->isKeyPressed(KeyCode::A, false))
			movement += transform->getLocalLeft();
		if (device->isKeyPressed(KeyCode::D, false))
			movement += transform->getLocalRight();
		if (device->isKeyPressed(KeyCode::Q, false))
			movement += transform->getLocalDown();
		if (device->isKeyPressed(KeyCode::E, false))
			movement += transform->getLocalUp();

		movement.normalize();
		movement *= dt * 10;
		transform->translateLocal(movement);
	}

private:
	Transform *transform;
	Device *device;
};


class RotatorAroundLocalXAxis: public ComponentBase<RotatorAroundLocalXAxis>
{
public:
	explicit RotatorAroundLocalXAxis(Node node, Device *device):
		ComponentBase(node),
		transform(nullptr),
		device(device)
	{
	}

	virtual void init() override
	{
		transform = node.getComponent<Transform>();
	}

	virtual void update() override
	{
		auto angle = device->getTimeDelta() * 1.3f;
		transform->rotate(Vector3::unitX(), angle, TransformSpace::Self);
	}

private:
	Transform *transform;
	Device *device;
};


class RotatorAroundWorldYAxis: public ComponentBase<RotatorAroundWorldYAxis>
{
public:
	explicit RotatorAroundWorldYAxis(Node node, Device *device):
		ComponentBase(node),
		transform(nullptr),
		device(device)
	{
	}

	virtual void init() override
	{
		transform = node.getComponent<Transform>();
	}

	virtual void update() override
	{
		auto angle = device->getTimeDelta();
		transform->rotate(Vector3::unitY(), angle, TransformSpace::World);
	}

private:
	Transform *transform;
	Device *device;
};


class Targeter: public ComponentBase<Targeter>
{
public:
	explicit Targeter(Node node, Transform *targetTransform):
		ComponentBase(node),
		transform(nullptr),
		targetTransform(targetTransform)
	{
	}

	virtual void init() override
	{
		transform = node.getComponent<Transform>();
	}

	virtual void update() override
	{
		transform->lookAt(targetTransform->getWorldPosition(), Vector3::unitY());
	}

private:
	Transform *transform;
	Transform *targetTransform;
};


shared<Texture> Demo_Test::loadTexture(const std::string &path)
{
	auto texture = DYNAMIC_CAST<Texture2D>(resourceManager->getOrLoadTexture(path));
	texture->generateMipmaps();
	texture->setMinFilter(TextureFilter::Linear);
	texture->setMagFilter(TextureFilter::Linear);
	texture->setAnisotropyLevel(8);
	return texture;
}


void Demo_Test::initMaterials()
{
	auto tex1 = loadTexture("../data/freeman1.png");
	auto tex2 = loadTexture("../data/freeman2.png");

	colorEffect = resourceManager->getOrCreateEffect(vsBasic, fsColor);
	redMaterial = resourceManager->getOrCreateMaterial(colorEffect);
	redMaterial->setPolygonFace(PolygonFace::All);
	redMaterial->getParameter("worldViewProjMatrix")->bindValue(AutoBinding::WorldViewProjectionMatrix);
	redMaterial->getParameter("color")->setVector4(Vector4(1, 0, 0, 1));
	greenMaterial = resourceManager->getOrCreateMaterial(colorEffect);
	greenMaterial->setPolygonFace(PolygonFace::All);
	greenMaterial->getParameter("worldViewProjMatrix")->bindValue(AutoBinding::WorldViewProjectionMatrix);
	greenMaterial->getParameter("color")->setVector4(Vector4(0, 1, 0, 1));
	blueMaterial = resourceManager->getOrCreateMaterial(colorEffect);
	blueMaterial->setPolygonFace(PolygonFace::All);
	blueMaterial->getParameter("worldViewProjMatrix")->bindValue(AutoBinding::WorldViewProjectionMatrix);
	blueMaterial->getParameter("color")->setVector4(Vector4(0, 0, 1, 1));
	whiteMaterial = resourceManager->getOrCreateMaterial(colorEffect);
	whiteMaterial->setPolygonFace(PolygonFace::All);
	whiteMaterial->getParameter("worldViewProjMatrix")->bindValue(AutoBinding::WorldViewProjectionMatrix);
	whiteMaterial->getParameter("color")->setVector4(Vector4(1, 1, 1, 1));

	texEffect = resourceManager->getOrCreateEffect(vsBasic, fsTexture);
	texMaterial = resourceManager->getOrCreateMaterial(texEffect);
	texMaterial->setPolygonFace(PolygonFace::All);
	texMaterial->getParameter("worldViewProjMatrix")->bindValue(AutoBinding::WorldViewProjectionMatrix);
	texMaterial->getParameter("mainTex")->setTexture(tex1);

	auto checkerEffect = resourceManager->getOrCreateEffect(vsBasic, fsChecker);
	checkerMaterial = resourceManager->getOrCreateMaterial(checkerEffect);
	checkerMaterial->setPolygonFace(PolygonFace::All);
	checkerMaterial->getParameter("worldViewProjMatrix")->bindValue(AutoBinding::WorldViewProjectionMatrix);
	checkerMaterial->getParameter("color")->setVector4(Vector4(1, 1, 0, 1));

	auto texWithLightingEffect = resourceManager->getOrCreateEffect(vsBasicLighting, fsTextureWithLighting);
	texWithLightingMaterial = resourceManager->getOrCreateMaterial(texWithLightingEffect);
	texWithLightingMaterial->setPolygonFace(PolygonFace::All);
	texWithLightingMaterial->getParameter("worldViewProjMatrix")->bindValue(AutoBinding::WorldViewProjectionMatrix);
	texWithLightingMaterial->getParameter("normalMatrix")->bindValue(AutoBinding::InverseTransposedWorldMatrix);
	texWithLightingMaterial->getParameter("mainTex")->setTexture(tex2);
}


void Demo_Test::initRenderTarget()
{
	auto canvasSize = device->getCanvasSize();
	auto renderTexture = DYNAMIC_CAST<Texture2D>(resourceManager->getOrCreateTexture2D("RTT"));
	renderTexture->setData(ColorFormat::RGB, std::vector<uint8_t>(), canvasSize.x / 8, canvasSize.y / 8);
	renderTexture->setMinFilter(TextureFilter::Nearest);
	renderTexture->setMagFilter(TextureFilter::Nearest);
	renderTexture->setVerticalWrapMode(TextureWrapMode::Clamp);
	renderTexture->setHorizontalWrapMode(TextureWrapMode::Clamp);
	auto texVector = std::vector<shared<Texture2D>>();
	texVector.push_back(renderTexture);

	renderTarget = resourceManager->getOrCreateRenderTarget("test");
	renderTarget->setTextures(texVector);

	renderTargetMaterial = resourceManager->getOrCreateMaterial(texEffect);
	renderTargetMaterial->setPolygonFace(PolygonFace::All);
	renderTargetMaterial->getParameter("worldViewProjMatrix")->bindValue(AutoBinding::WorldViewProjectionMatrix);
	renderTargetMaterial->getParameter("mainTex")->setTexture(renderTexture);
}


void Demo_Test::initCameras()
{
	auto canvasSize = device->getCanvasSize();
	auto offscreenCameraNode = scene->createNode();
	auto offscreenCameraTransform = offscreenCameraNode->getComponent<Transform>();
	offscreenCameraTransform->setLocalPosition(Vector3(0, 0, 10));
	auto offscreenCamera = offscreenCameraNode->addComponent<Camera>();
	offscreenCamera->setClearColor(1, 1, 1, 1);
	offscreenCamera->setNear(0.05f);
	offscreenCamera->setRenderTarget(renderTarget);
	offscreenCamera->setViewport(0, 0, canvasSize.x / 8, canvasSize.y / 8);

	auto mainCameraNode = scene->createNode();
	auto mainCameraTransform = mainCameraNode->getComponent<Transform>();
	mainCameraTransform->setLocalPosition(Vector3(0, 2, 15));
	mainCameraNode->addComponent<Spectator>(device);
	mainCameraNode->addComponent<EscapeWatcher>(device);
	auto mainCamera = mainCameraNode->addComponent<Camera>();
	mainCamera->setClearColor(0, 0.6f, 0.6f, 1);
	mainCamera->setNear(0.05f);
}


void Demo_Test::initObjects()
{
	auto canvasSize = device->getCanvasSize();

	// Monkey
	auto node = scene->createNode();
	auto renderer = node->addComponent<ModelRenderer>();
	renderer->setModel(monkeyModel);
	renderer->setMaterialForMesh(0, texWithLightingMaterial);
	node->getComponent<Transform>()->setLocalPosition(Vector3::zero());
	node->addComponent<RotatorAroundLocalXAxis>(device);

	// RTT quad
	auto parent = scene->createNode();
	parent->getComponent<Transform>()->setLocalPosition(Vector3(-2, 2, -2));
	parent->addComponent<RotatorAroundWorldYAxis>(device);
	initAxesModel(parent);

	auto quad = createQuad();
	quad->getComponent<ModelRenderer>()->setMaterialForMesh(0, renderTargetMaterial);
	auto quadTransform = quad->getComponent<Transform>();
	quadTransform->setParent(parent->getComponent<Transform>());
	quadTransform->setLocalPosition(Vector3(5, 2, -5));
	quadTransform->setLocalScale(Vector3(5, 5 * canvasSize.y / canvasSize.x, 1));
	quad->addComponent<Targeter>(node->getComponent<Transform>()); // monkey

	// Textured quad
	parent = scene->createNode();
	parent->getComponent<Transform>()->setLocalPosition(Vector3(5, 0, 0));
	parent->addComponent<RotatorAroundWorldYAxis>(device);
	initAxesModel(parent);

	quad = createQuad();
	quad->addComponent<RotatorAroundLocalXAxis>(device);
	quad->getComponent<Transform>()->setParent(parent->getComponent<Transform>());
	quad->getComponent<Transform>()->setLocalPosition(Vector3(2, 0, 0));
	quad->getComponent<ModelRenderer>()->setMaterialForMesh(0, texMaterial);

	// Box
	node = createQuad();
	rebuildToBoxMesh(node);
	node->getComponent<ModelRenderer>()->setMaterialForMesh(0, checkerMaterial);
	node->getComponent<Transform>()->setLocalPosition(Vector3(-5, 0, 0));
	node->addComponent<RotatorAroundWorldYAxis>(device);
}


void Demo_Test::initModels()
{
	axesModel = resourceManager->getOrLoadModel("../data/axes.obj");
	monkeyModel = resourceManager->getOrLoadModel("../data/monkey_hires.obj");
}


shared<Node> Demo_Test::createQuad()
{
	auto mesh = resourceManager->getOrCreateMesh();

	std::vector<Vector3> vertices
	{
		Vector3(-1, -1, 0),
		Vector3(-1, 1, 0),
		Vector3(1, 1, 0),
		Vector3(1, -1, 0)
	};

	std::vector<Vector3> normals
	{
		Vector3(0, 0, -1),
		Vector3(0, 0, -1),
		Vector3(0, 0, -1),
		Vector3(0, 0, -1)
	};

	std::vector<Vector2> uvs
	{
		Vector2(0, 0),
		Vector2(0, 1),
		Vector2(1, 1),
		Vector2(1, 0)
	};

	std::vector<unsigned short> indices
	{
		0, 1, 2,
		0, 2, 3
	};

	mesh->setVertices(vertices);
	mesh->setNormals(normals);
	mesh->setUVs(uvs);
	mesh->setIndices(indices);

	auto model = resourceManager->getOrCreateModel();
	model->addMesh(mesh);

	auto node = scene->createNode();
	auto renderer = node->addComponent<ModelRenderer>();
	renderer->setModel(model);

	return node;
}


void Demo_Test::rebuildToBoxMesh(shared<Node> node)
{
	auto model = node->getComponent<ModelRenderer>()->getModel();
	auto mesh = model->getMesh(0);

	std::vector<Vector3> newVertices
	{
		Vector3(-1, -1, 1),
		Vector3(-1, 1, 1),
		Vector3(1, 1, 1),
		Vector3(1, -1, 1),

		Vector3(-1, -1, -1),
		Vector3(-1, 1, -1),
		Vector3(-1, 1, 1),
		Vector3(-1, -1, 1),

		Vector3(1, -1, -1),
		Vector3(1, 1, -1),
		Vector3(-1, 1, -1),
		Vector3(-1, -1, -1),

		Vector3(1, -1, 1),
		Vector3(1, 1, 1),
		Vector3(1, 1, -1),
		Vector3(1, -1, -1),

		Vector3(-1, 1, 1),
		Vector3(-1, 1, -1),
		Vector3(1, 1, -1),
		Vector3(1, 1, 1),

		Vector3(-1, -1, -1),
		Vector3(-1, -1, 1),
		Vector3(1, -1, 1),
		Vector3(1, -1, -1)
	};

	std::vector<Vector2> newUVs
	{
		Vector2(0, 0),
		Vector2(0, 1),
		Vector2(1, 1),
		Vector2(1, 0),
		Vector2(0, 0),
		Vector2(0, 1),
		Vector2(1, 1),
		Vector2(1, 0),
		Vector2(0, 0),
		Vector2(0, 1),
		Vector2(1, 1),
		Vector2(1, 0),
		Vector2(0, 0),
		Vector2(0, 1),
		Vector2(1, 1),
		Vector2(1, 0),
		Vector2(0, 0),
		Vector2(0, 1),
		Vector2(1, 1),
		Vector2(1, 0),
		Vector2(0, 0),
		Vector2(0, 1),
		Vector2(1, 1),
		Vector2(1, 0)
	};

	std::vector<unsigned short> newIndices
	{
		0u, 1u, 2u,
		0u, 2u, 3u,
		4u, 5u, 6u,
		4u, 6u, 7u,
		8u, 9u, 10u,
		8u, 10u, 11u,
		12u, 13u, 14u,
		12u, 14u, 15u,
		16u, 17u, 18u,
		16u, 18u, 19u,
		20u, 21u, 22u,
		20u, 22u, 23u
	};

	mesh->setVertices(newVertices);
	mesh->setUVs(newUVs);
	mesh->setIndices(newIndices);
}


void Demo_Test::initAxesModel(shared<Node> node)
{
	auto renderer = node->addComponent<ModelRenderer>();
	renderer->setModel(axesModel);
	renderer->setMaterialForMesh(0, blueMaterial);
	renderer->setMaterialForMesh(1, greenMaterial);
	renderer->setMaterialForMesh(2, whiteMaterial);
	renderer->setMaterialForMesh(3, redMaterial);
}


void Demo_Test::run()
{
	initMaterials();
	initModels();
	initRenderTarget();
	initObjects();
	initCameras();
}