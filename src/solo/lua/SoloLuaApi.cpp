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
}
