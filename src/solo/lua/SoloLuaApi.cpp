/*
	Copyright (c) Aleksey Fedotov
	MIT license
*/

#include "SoloLuaCommon.h"
#include "SoloDevice.h"
#include "SoloFileSystem.h"
#include "SoloPhysics.h"
#include "SoloLogger.h"
#include "SoloVector2.h"
#include "SoloVector3.h"
#include "SoloVector4.h"
#include "SoloQuaternion.h"
#include "SoloDegree.h"
#include "SoloRadian.h"
#include "SoloMatrix.h"
#include "SoloRay.h"
#include "SoloTransform.h"
#include "SoloTexture.h"
#include "SoloMaterial.h"
#include "SoloMesh.h"
#include "SoloLuaScriptComponent.h"
#include "SoloMeshRenderer.h"
#include "SoloCamera.h"
#include "SoloSpectator.h"
#include "SoloSkyboxRenderer.h"
#include "SoloFontRenderer.h"
#include "SoloRigidBody.h"
#include "SoloFrameBuffer.h"

using namespace solo;

#define FIELD(klass, name) #name, &klass::name
#define ENUM_FIELD(e, name) #name, e::name

static void registerEnums(sol::table &module)
{
	module.new_enum(
		"DeviceMode",
		ENUM_FIELD(DeviceMode, Null),
		ENUM_FIELD(DeviceMode, OpenGL),
		ENUM_FIELD(DeviceMode, Vulkan)
	);

	module.new_enum(
		"TransformSpace",
		ENUM_FIELD(TransformSpace, Parent),
		ENUM_FIELD(TransformSpace, Self),
		ENUM_FIELD(TransformSpace, World)
	);

	module.new_enum(
		"TextureWrap",
		ENUM_FIELD(TextureWrap, ClampToBorder),
		ENUM_FIELD(TextureWrap, ClampToEdge),
		ENUM_FIELD(TextureWrap, MirrorRepeat),
		ENUM_FIELD(TextureWrap, Repeat)
	);

	module.new_enum(
		"TextureFormat",
		ENUM_FIELD(TextureFormat, RGB),
		ENUM_FIELD(TextureFormat, RGBA),
		ENUM_FIELD(TextureFormat, Red)
	);

	module.new_enum(
		"TextureFilter",
		ENUM_FIELD(TextureFilter, Linear),
		ENUM_FIELD(TextureFilter, Nearest)
	);

	module.new_enum(
		"TextureMipFilter",
		ENUM_FIELD(TextureMipFilter, Nearest),
		ENUM_FIELD(TextureMipFilter, Linear),
		ENUM_FIELD(TextureMipFilter, None)
	);

	module.new_enum(
		"MaterialPrefab",
		ENUM_FIELD(MaterialPrefab, Font),
		ENUM_FIELD(MaterialPrefab, Skybox)
	);

	module.new_enum(
		"FaceCull",
		ENUM_FIELD(FaceCull, None),
		ENUM_FIELD(FaceCull, Back),
		ENUM_FIELD(FaceCull, Front)
	);

	module.new_enum(
		"PolygonMode",
		ENUM_FIELD(PolygonMode, Fill),
		ENUM_FIELD(PolygonMode, Points),
		ENUM_FIELD(PolygonMode, Wireframe)
	);

	module.new_enum(
		"DepthFunction",
		ENUM_FIELD(DepthFunction, Always),
		ENUM_FIELD(DepthFunction, Equal),
		ENUM_FIELD(DepthFunction, GEqual),
		ENUM_FIELD(DepthFunction, Greater),
		ENUM_FIELD(DepthFunction, LEqual),
		ENUM_FIELD(DepthFunction, Less),
		ENUM_FIELD(DepthFunction, Never),
		ENUM_FIELD(DepthFunction, NotEqual)
	);

	module.new_enum(
		"BlendFactor",
		ENUM_FIELD(BlendFactor, Zero),
		ENUM_FIELD(BlendFactor, One),
		ENUM_FIELD(BlendFactor, SrcColor),
		ENUM_FIELD(BlendFactor, OneMinusSrcColor),
		ENUM_FIELD(BlendFactor, DstColor),
		ENUM_FIELD(BlendFactor, OneMinusDstColor),
		ENUM_FIELD(BlendFactor, SrcAlpha),
		ENUM_FIELD(BlendFactor, OneMinusSrcAlpha),
		ENUM_FIELD(BlendFactor, DstAlpha),
		ENUM_FIELD(BlendFactor, OneMinusDstAlpha),
		ENUM_FIELD(BlendFactor, ConstantAlpha),
		ENUM_FIELD(BlendFactor, OneMinusConstantAlpha),
		ENUM_FIELD(BlendFactor, SrcAlphaSaturate)
	);

	module.new_enum(
		"BindParameterSemantics",
		ENUM_FIELD(BindParameterSemantics, WorldMatrix),
		ENUM_FIELD(BindParameterSemantics, ViewMatrix),
		ENUM_FIELD(BindParameterSemantics, ProjectionMatrix),
		ENUM_FIELD(BindParameterSemantics, WorldViewMatrix),
		ENUM_FIELD(BindParameterSemantics, ViewProjectionMatrix),
		ENUM_FIELD(BindParameterSemantics, WorldViewProjectionMatrix),
		ENUM_FIELD(BindParameterSemantics, InverseTransposedWorldMatrix),
		ENUM_FIELD(BindParameterSemantics, InverseTransposedWorldViewMatrix),
		ENUM_FIELD(BindParameterSemantics, CameraWorldPosition)
	);

	module.new_enum(
		"KeyCode",
		ENUM_FIELD(KeyCode, Digit0),
		ENUM_FIELD(KeyCode, Digit1),
		ENUM_FIELD(KeyCode, Digit2),
		ENUM_FIELD(KeyCode, Digit3),
		ENUM_FIELD(KeyCode, Digit4),
		ENUM_FIELD(KeyCode, Digit5),
		ENUM_FIELD(KeyCode, Digit6),
		ENUM_FIELD(KeyCode, Digit7),
		ENUM_FIELD(KeyCode, Digit8),
		ENUM_FIELD(KeyCode, Digit9),
		ENUM_FIELD(KeyCode, A),
		ENUM_FIELD(KeyCode, B),
		ENUM_FIELD(KeyCode, C),
		ENUM_FIELD(KeyCode, D),
		ENUM_FIELD(KeyCode, E),
		ENUM_FIELD(KeyCode, F),
		ENUM_FIELD(KeyCode, G),
		ENUM_FIELD(KeyCode, H),
		ENUM_FIELD(KeyCode, I),
		ENUM_FIELD(KeyCode, J),
		ENUM_FIELD(KeyCode, K),
		ENUM_FIELD(KeyCode, L),
		ENUM_FIELD(KeyCode, M),
		ENUM_FIELD(KeyCode, N),
		ENUM_FIELD(KeyCode, O),
		ENUM_FIELD(KeyCode, P),
		ENUM_FIELD(KeyCode, Q),
		ENUM_FIELD(KeyCode, R),
		ENUM_FIELD(KeyCode, S),
		ENUM_FIELD(KeyCode, T),
		ENUM_FIELD(KeyCode, U),
		ENUM_FIELD(KeyCode, V),
		ENUM_FIELD(KeyCode, W),
		ENUM_FIELD(KeyCode, X),
		ENUM_FIELD(KeyCode, Y),
		ENUM_FIELD(KeyCode, Z),
		ENUM_FIELD(KeyCode, LeftArrow),
		ENUM_FIELD(KeyCode, RightArrow),
		ENUM_FIELD(KeyCode, UpArrow),
		ENUM_FIELD(KeyCode, DownArrow),
		ENUM_FIELD(KeyCode, Escape),
		ENUM_FIELD(KeyCode, Backspace),
		ENUM_FIELD(KeyCode, Space),
		ENUM_FIELD(KeyCode, Return),
		ENUM_FIELD(KeyCode, LeftShift),
		ENUM_FIELD(KeyCode, LeftCtrl),
		ENUM_FIELD(KeyCode, LeftAlt),
		ENUM_FIELD(KeyCode, RightShift),
		ENUM_FIELD(KeyCode, RightCtrl),
		ENUM_FIELD(KeyCode, RightAlt)
	);

	module.new_enum(
		"MouseButton",
		ENUM_FIELD(MouseButton, Left),
		ENUM_FIELD(MouseButton, Middle),
		ENUM_FIELD(MouseButton, Right)
	);

	module.new_enum(
		"MeshPrefab",
		ENUM_FIELD(MeshPrefab, Cube),
		ENUM_FIELD(MeshPrefab, Quad)
	);

	module.new_enum(
		"PrimitiveType",
		ENUM_FIELD(PrimitiveType, Triangles),
		ENUM_FIELD(PrimitiveType, TriangleStrip),
		ENUM_FIELD(PrimitiveType, Lines),
		ENUM_FIELD(PrimitiveType, LineStrip),
		ENUM_FIELD(PrimitiveType, Points)
	);
}

static void registerVector2(sol::table &module)
{
	const sol::constructors<Vector2(), Vector2(float, float)> ctors{};
	sol::usertype<Vector2> vector2{
		ctors,
		FIELD(Vector2, x),
		FIELD(Vector2, y),
		FIELD(Vector2, isUnit),
		FIELD(Vector2, isZero),
		FIELD(Vector2, distance),
		FIELD(Vector2, distanceSquared),
		FIELD(Vector2, length),
		FIELD(Vector2, lengthSquared),
		FIELD(Vector2, normalized),
		FIELD(Vector2, normalize),
		FIELD(Vector2, angle),
		FIELD(Vector2, clamp),
		FIELD(Vector2, dot),
		sol::meta_function::addition, &Vector2::operator+,
		sol::meta_function::subtraction, sol::resolve<Vector2(const Vector2&)const>(&Vector2::operator-),
		sol::meta_function::unary_minus, sol::resolve<Vector2()const>(&Vector2::operator-),
		sol::meta_function::multiplication, &Vector2::operator*,
		sol::meta_function::division, &Vector2::operator/
	};
	module.set_usertype("Vector2", vector2);
}

static void registerVector3(sol::table &module)
{
	const sol::constructors<Vector3(), Vector3(float), Vector3(float, float, float)> ctors{};
	sol::usertype<Vector3> vector3{
		ctors,
		FIELD(Vector3, x),
		FIELD(Vector3, y),
		FIELD(Vector3, z),
		FIELD(Vector3, isUnit),
		FIELD(Vector3, isZero),
		FIELD(Vector3, distance),
		FIELD(Vector3, distanceSquared),
		FIELD(Vector3, length),
		FIELD(Vector3, lengthSquared),
		FIELD(Vector3, normalized),
		FIELD(Vector3, normalize),
		FIELD(Vector3, angle),
		FIELD(Vector3, clamp),
		FIELD(Vector3, dot),
		FIELD(Vector3, cross),
		sol::meta_function::addition, sol::resolve<Vector3(const Vector3&)const>(&Vector3::operator+),
		sol::meta_function::subtraction, sol::resolve<Vector3(const Vector3&)const>(&Vector3::operator-),
		sol::meta_function::unary_minus, sol::resolve<Vector3()const>(&Vector3::operator-),
		sol::meta_function::multiplication, &Vector3::operator*,
		sol::meta_function::division, &Vector3::operator/
	};
	module.set_usertype("Vector3", vector3);
}

static void registerVector4(sol::table &module)
{
	const sol::constructors<Vector4(), Vector4(float, float, float, float)> ctors{};
	sol::usertype<Vector4> vector4{
		ctors,
		FIELD(Vector4, x),
		FIELD(Vector4, y),
		FIELD(Vector4, z),
		FIELD(Vector4, w),
		FIELD(Vector4, isUnit),
		FIELD(Vector4, isZero),
		FIELD(Vector4, distance),
		FIELD(Vector4, distanceSquared),
		FIELD(Vector4, length),
		FIELD(Vector4, lengthSquared),
		FIELD(Vector4, normalized),
		FIELD(Vector4, normalize),
		FIELD(Vector4, angle),
		FIELD(Vector4, clamp),
		FIELD(Vector4, dot),
		sol::meta_function::addition, sol::resolve<Vector4(const Vector4&)const>(&Vector4::operator+),
		sol::meta_function::subtraction, sol::resolve<Vector4(const Vector4&)const>(&Vector4::operator-),
		sol::meta_function::unary_minus, sol::resolve<Vector4()const>(&Vector4::operator-),
		sol::meta_function::multiplication, &Vector4::operator*,
		sol::meta_function::division, &Vector4::operator/
	};
	module.set_usertype("Vector4", vector4);
}

static void registerMatrix(sol::table &module)
{
	const sol::constructors<Matrix(), Matrix(const Matrix&)> ctors{};
	sol::usertype<Matrix> matrix{
		ctors,
		FIELD(Matrix, identity),
		FIELD(Matrix, isIdentity),
		FIELD(Matrix, getDeterminant),
		FIELD(Matrix, invert),
		FIELD(Matrix, transpose),
		FIELD(Matrix, createLookAt),
		FIELD(Matrix, createPerspective),
		FIELD(Matrix, createOrthographic),
		FIELD(Matrix, createScale),
		FIELD(Matrix, createRotationFromQuaternion),
		FIELD(Matrix, createRotationFromAxisAngle),
		FIELD(Matrix, createRotationX),
		FIELD(Matrix, createRotationY),
		FIELD(Matrix, createRotationZ),
		FIELD(Matrix, createTranslation),
		FIELD(Matrix, getScale),
		FIELD(Matrix, getRotation),
		FIELD(Matrix, getTranslation),
		FIELD(Matrix, getUpVector),
		FIELD(Matrix, getDownVector),
		FIELD(Matrix, getLeftVector),
		FIELD(Matrix, getRightVector),
		FIELD(Matrix, getForwardVector),
		FIELD(Matrix, getBackVector),
		FIELD(Matrix, rotateByQuaternion),
		FIELD(Matrix, rotateByAxisAngle),
		FIELD(Matrix, rotateX),
		FIELD(Matrix, rotateY),
		FIELD(Matrix, rotateZ),
		FIELD(Matrix, scaleByScalar),
		FIELD(Matrix, scaleByVector),
		FIELD(Matrix, translate),
		FIELD(Matrix, transformPoint),
		FIELD(Matrix, transformDirection),
		FIELD(Matrix, transformRay),
		FIELD(Matrix, decompose),
		sol::meta_function::multiplication, sol::resolve<Matrix(const Matrix&)const>(&Matrix::operator*),
	};
	module.set_usertype("Matrix", matrix);
}

static void registerRay(sol::table &module)
{
	const sol::constructors<Ray(), Ray(const Vector3&, const Vector3&), Ray(float, float, float, float, float, float)> ctors{};
	sol::usertype<Ray> ray{
		ctors,
		FIELD(Ray, getOrigin),
		FIELD(Ray, setOrigin),
		FIELD(Ray, getDirection),
		FIELD(Ray, setDirection),
	};
	module.set_usertype("Ray", ray);
}

static void registerQuaternion(sol::table &module)
{
	const sol::constructors<Quaternion(), Quaternion(float, float, float, float), Quaternion(const Vector3&, const Radian&)> ctors{};
	sol::usertype<Quaternion> quaternion{
		ctors,
		FIELD(Quaternion, x),
		FIELD(Quaternion, y),
		FIELD(Quaternion, z),
		FIELD(Quaternion, w),
		FIELD(Quaternion, createFromAxisAngle),
		FIELD(Quaternion, lerp),
		FIELD(Quaternion, slerp),
		FIELD(Quaternion, squad),
		FIELD(Quaternion, isIdentity),
		FIELD(Quaternion, isZero),
		FIELD(Quaternion, conjugate),
		FIELD(Quaternion, inverse),
		FIELD(Quaternion, normalize),
		FIELD(Quaternion, normalized),
		FIELD(Quaternion, toAxisAngle),
		sol::meta_function::multiplication, &Quaternion::operator*,
	};
	module.set_usertype("Quaternion", quaternion);
}

static void registerRadian(sol::table &module)
{
	const sol::constructors<Radian(float), Radian(const Degree&)> ctors{};
	sol::usertype<Radian> radian{
		ctors,
		FIELD(Radian, toRawDegree),
		FIELD(Radian, toRawRadian),
		sol::meta_function::addition, sol::resolve<Radian(const Radian&)const>(&Radian::operator+),
		sol::meta_function::subtraction, sol::resolve<Radian(const Radian&)const>(&Radian::operator-),
		sol::meta_function::unary_minus, sol::resolve<Radian()const>(&Radian::operator-),
		sol::meta_function::multiplication, &Radian::operator*,
		sol::meta_function::division, &Radian::operator/
	};
	module.set_usertype("Radian", radian);
}

static void registerDegree(sol::table &module)
{
	const sol::constructors<Degree(float), Degree(const Radian&)> ctors{};
	sol::usertype<Degree> degree{
		ctors,
		FIELD(Degree, toRawDegree),
		FIELD(Degree, toRawRadian),
		sol::meta_function::addition, sol::resolve<Degree(const Degree&)const>(&Degree::operator+),
		sol::meta_function::subtraction, sol::resolve<Degree(const Degree&)const>(&Degree::operator-),
		sol::meta_function::unary_minus, sol::resolve<Degree()const>(&Degree::operator-),
		sol::meta_function::multiplication, &Degree::operator*,
		sol::meta_function::division, &Degree::operator/
	};
	module.set_usertype("Degree", degree);
}

static void registerLogger(sol::table &module)
{
	auto logger = module.new_usertype<Logger>(
		"Logger",
		FIELD(Logger, setTargetFile),
		FIELD(Logger, logDebug),
		FIELD(Logger, logInfo),
		FIELD(Logger, logWarning),
		FIELD(Logger, logError),
		FIELD(Logger, logCritical)
	);
}

static void registerDeviceSetup(sol::table &module)
{
	auto deviceSetup = module.new_usertype<DeviceSetup>(
		"DeviceSetup",
		FIELD(DeviceSetup, mode),
		FIELD(DeviceSetup, canvasWidth),
		FIELD(DeviceSetup, canvasHeight),
		FIELD(DeviceSetup, fullScreen),
		FIELD(DeviceSetup, windowTitle),
		FIELD(DeviceSetup, vsync),
		FIELD(DeviceSetup, logFilePath)
	);
}

static void registerTransform(sol::table &module)
{
	module.new_usertype<Transform>(
		"Transform",
		sol::base_classes, sol::bases<Component>(),
		FIELD(Transform, getParent),
		FIELD(Transform, setParent),
		FIELD(Transform, getChild),
		FIELD(Transform, getChildrenCount),
		FIELD(Transform, clearChildren),
		FIELD(Transform, getWorldScale),
		FIELD(Transform, getLocalScale),
		FIELD(Transform, getWorldRotation),
		FIELD(Transform, getLocalRotation),
		FIELD(Transform, getWorldPosition),
		FIELD(Transform, getLocalPosition),
		FIELD(Transform, getWorldUp),
		FIELD(Transform, getLocalUp),
		FIELD(Transform, getWorldDown),
		FIELD(Transform, getLocalDown),
		FIELD(Transform, getWorldLeft),
		FIELD(Transform, getLocalLeft),
		FIELD(Transform, getWorldRight),
		FIELD(Transform, getLocalRight),
		FIELD(Transform, getWorldForward),
		FIELD(Transform, getLocalForward),
		FIELD(Transform, getWorldBack),
		FIELD(Transform, getLocalBack),
		FIELD(Transform, translateLocal),
		FIELD(Transform, scaleLocal),
		FIELD(Transform, setLocalPosition),
		FIELD(Transform, setLocalScale),
		FIELD(Transform, rotate),
		FIELD(Transform, rotateByAxisAngle),
		FIELD(Transform, setLocalRotation),
		FIELD(Transform, setLocalAxisAngleRotation),
		FIELD(Transform, lookAt),
		FIELD(Transform, getMatrix),
		FIELD(Transform, getWorldMatrix),
		FIELD(Transform, getWorldViewMatrix),
		FIELD(Transform, getWorldViewProjMatrix),
		FIELD(Transform, getInvTransposedWorldMatrix),
		FIELD(Transform, getInvTransposedWorldViewMatrix),
		FIELD(Transform, transformPoint),
		FIELD(Transform, transformDirection)
	);
}

static void registerFrameBuffer(sol::table &module)
{
	module.new_usertype<FrameBuffer>(
		"FrameBuffer",
		FIELD(FrameBuffer, create),
		FIELD(FrameBuffer, setAttachments),
		FIELD(FrameBuffer, getDimensions)
	);
}

static void setRenderTarget(Camera *camera, sol::optional<sptr<FrameBuffer>> target)
{
	camera->setRenderTarget(target.value_or(nullptr));
}

static void registerCamera(sol::table &module)
{
	module.new_usertype<Camera>(
		"Camera",
		sol::base_classes, sol::bases<Component>(),
		FIELD(Camera, getTransform),
		FIELD(Camera, getRenderTarget),
		"setRenderTarget", &setRenderTarget,
		FIELD(Camera, getClearColor),
		FIELD(Camera, setClearColor),
		FIELD(Camera, hasColorClearing),
		FIELD(Camera, setColorClearing),
		FIELD(Camera, getViewport),
		FIELD(Camera, setViewport),
		FIELD(Camera, isPerspective),
		FIELD(Camera, setPerspective),
		FIELD(Camera, getZNear),
		FIELD(Camera, setZNear),
		FIELD(Camera, getZFar),
		FIELD(Camera, setZFar),
		FIELD(Camera, getFOV),
		FIELD(Camera, setFOV),
		FIELD(Camera, getOrthoSize),
		FIELD(Camera, setOrthoSize),
		FIELD(Camera, windowPointToWorldRay),
		FIELD(Camera, getAspectRatio),
		FIELD(Camera, getTagMask),
		FIELD(Camera, setTagMask),
		FIELD(Camera, getOrder),
		FIELD(Camera, setOrder),
		FIELD(Camera, getViewMatrix),
		FIELD(Camera, getInvViewMatrix),
		FIELD(Camera, getProjectionMatrix),
		FIELD(Camera, getViewProjectionMatrix),
		FIELD(Camera, getInvViewProjectionMatrix)
	);
}

static void registerScene(sol::table &module)
{
	module.new_usertype<Scene>(
		"Scene",
		FIELD(Scene, create),
		FIELD(Scene, getDevice),
		FIELD(Scene, createNode),
		FIELD(Scene, removeNode),
		FIELD(Scene, removeNodeById)
	);
}

static void registerDevice(sol::table &module)
{
	module.new_usertype<Device>(
		"Device",
		FIELD(Device, getWindowTitle),
		FIELD(Device, setWindowTitle),
		FIELD(Device, getCanvasSize),
		FIELD(Device, getDpiIndependentCanvasSize),
		FIELD(Device, isVsync),
		FIELD(Device, getMode),
		FIELD(Device, saveScreenshot),
		FIELD(Device, setCursorCaptured),
		FIELD(Device, getLifetime),
		FIELD(Device, getTimeDelta),
		FIELD(Device, isWindowCloseRequested),
		FIELD(Device, isQuitRequested),
		FIELD(Device, isKeyPressed),
		FIELD(Device, isKeyReleased),
		FIELD(Device, getMouseMotion),
		FIELD(Device, getMousePosition),
		FIELD(Device, isMouseButtonDown),
		FIELD(Device, isMouseButtonReleased),
		FIELD(Device, update),
		FIELD(Device, hasActiveBackgroundJobs),
		FIELD(Device, getFileSystem),
		FIELD(Device, getPhysics),
		FIELD(Device, getLogger)
	);
}

static umap<str, u32> builtInComponents = {
	{ "Transform", Transform::getId() },
	{ "MeshRenderer", MeshRenderer::getId() },
	{ "Camera", Camera::getId() },
	{ "Spectator", Spectator::getId() },
	{ "SkyboxRenderer", SkyboxRenderer::getId() },
	{ "FontRenderer", FontRenderer::getId() },
	{ "RigidBody", RigidBody::getId() }
};

static auto findComponent(Node *node, const str &name) -> Component*
{
	SL_PANIC_IF(!builtInComponents.count(name), SL_FMT("Not found built-in component ", name));
	return node->getScene()->findComponent(node->getId(), builtInComponents.at(name));
}

static auto findTransform(Node *node) -> Transform*
{
	return node->findComponent<Transform>();
}

static auto addTransform(Node *node) -> Transform*
{
	return node->addComponent<Transform>();
}

static void removeTransform(Node *node)
{
	node->removeComponent<Transform>();
}

static auto addMeshRenderer(Node *node) -> MeshRenderer*
{
	return node->addComponent<MeshRenderer>();
}

static void removeMeshRenderer(Node *node)
{
	node->addComponent<MeshRenderer>();
}

static auto addCamera(Node *node) -> Camera*
{
	return node->addComponent<Camera>();
}

static void removeCamera(Node *node)
{
	node->removeComponent<Camera>();
}

static auto addSpectator(Node *node) -> Spectator*
{
	return node->addComponent<Spectator>();
}

static void removeSpectator(Node *node)
{
	node->removeComponent<Spectator>();
}

static auto addSkyboxRenderer(Node *node) -> SkyboxRenderer*
{
	return node->addComponent<SkyboxRenderer>();
}

static void removeSkyboxRenderer(Node *node)
{
	node->removeComponent<SkyboxRenderer>();
}

static auto addFontRenderer(Node *node) -> FontRenderer*
{
	return node->addComponent<FontRenderer>();
}

static void removeFontRenderer(Node *node)
{
	node->removeComponent<FontRenderer>();
}

static auto addRigidBody(Node *node, sol::object &arg) -> RigidBody*
{
	return node->addComponent<RigidBody>(arg.as<RigidBodyConstructionParameters>());
}

static void removeRigidBody(Node *node)
{
	node->removeComponent<RigidBody>();
}

static auto addComponent(Node *node, const str &name, sol::object &arg) -> Component*
{
	if (name == "Transform")
		return node->addComponent<Transform>();
	if (name == "MeshRenderer")
		return node->addComponent<MeshRenderer>();
	if (name == "Camera")
		return node->addComponent<Camera>();
	if (name == "Spectator")
		return node->addComponent<Spectator>();
	if (name == "SkyboxRenderer")
		return node->addComponent<SkyboxRenderer>();
	if (name == "FontRenderer")
		return node->addComponent<FontRenderer>();
	if (name == "RigidBody")
		return node->addComponent<RigidBody>(arg.as<RigidBodyConstructionParameters>());

	SL_PANIC(SL_FMT("Unknown built-in component ", name))
	return nullptr;
}

static void removeComponent(Node *node, const str &name)
{
	SL_PANIC_IF(!builtInComponents.count(name), SL_FMT("Not found built-in component ", name));
	node->getScene()->removeComponent(node->getId(), builtInComponents.at(name));
}

static void addScriptComponent(Node *node, sol::table &cmp)
{
	const auto c = std::make_shared<LuaScriptComponent>(*node, cmp);
	node->getScene()->addComponent(node->getId(), c);
}

static void removeScriptComponent(Node *node, sol::table &cmp) // TODO also version with cmp id param
{
	const auto typeId = cmp.get<u32>("typeId") + LuaScriptComponent::minComponentTypeId;
	node->getScene()->removeComponent(node->getId(), typeId);
}

static auto findScriptComponent(Node *node, u32 typeId) -> sol::object
{
	const auto cmp = node->getScene()->findComponent(node->getId(), typeId + LuaScriptComponent::minComponentTypeId);
	if (cmp)
	{
	    const auto scriptComponent = dynamic_cast<LuaScriptComponent*>(cmp);
		return scriptComponent->getUnderlyingCmp();
	}

	return sol::nil;
}

static void registerNode(sol::table &module)
{
	module.new_usertype<Node>(
		"Node",
		FIELD(Node, getId),
		FIELD(Node, getScene),
		"addScriptComponent", &addScriptComponent,
		"removeScriptComponent", &removeScriptComponent,
		"findScriptComponent", &findScriptComponent,
		"addComponent", &addComponent,
		"addCamera", &addCamera,
		"removeCamera", &removeCamera,
		"addTransform", &addTransform,
		"removeTransform", &removeTransform,
		"addSkyboxRenderer", &addSkyboxRenderer,
		"removeSkyboxRenderer", &removeSkyboxRenderer,
		"addFontRenderer", &addFontRenderer,
		"removeFontRenderer", &removeFontRenderer,
		"addRigidBody", &addRigidBody,
		"removeRigidBody", &removeRigidBody,
		"addSpectator", &addSpectator,
		"removeSpectator", &removeSpectator,
		"addMeshRenderer", &addMeshRenderer,
		"removeMeshRenderer", &removeMeshRenderer,
		"findComponent", &findComponent, // TODO remove
		"findTransform", &findTransform,
		"removeComponent", &removeComponent
	);
}

static void registerComponent(sol::table &module)
{
	module.new_usertype<Component>(
		"Component",
		FIELD(Component, getTypeId),
		FIELD(Component, getTag),
		FIELD(Component, setTag),
		FIELD(Component, getLayer),
		FIELD(Component, setLayer),
		FIELD(Component, getNode)
	);
}

static void registerFileSystem(sol::table &module)
{
	module.new_usertype<FileSystem>(
		"FileSystem",
		FIELD(FileSystem, readBytes),
		FIELD(FileSystem, writeBytes),
		FIELD(FileSystem, readText),
		FIELD(FileSystem, readLines),
		FIELD(FileSystem, writeLines),
		FIELD(FileSystem, iterateLines)
	);
}

void registerApi(sol::table &module)
{
	registerEnums(module);
	registerVector2(module);
	registerVector3(module);
	registerVector4(module);
	registerDegree(module);
	registerRadian(module);
	registerQuaternion(module);
	registerRay(module);
	registerMatrix(module);
	registerLogger(module);
	registerFileSystem(module);
	registerDeviceSetup(module);
	registerDevice(module);
	registerScene(module);
	registerComponent(module);
	registerTransform(module);
	registerCamera(module);
	registerFrameBuffer(module);
	registerNode(module);
}
