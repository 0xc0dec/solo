#include "SoloMaterialParameter.h"
#include "SoloTransform.h"
#include "SoloVector2.h"
#include "SoloVector3.h"
#include "SoloVector4.h"
#include "SoloMatrix.h"
#include "SoloRenderContext.h"
#include "SoloMaterial.h"
#include "SoloEffect.h"
#include "SoloCamera.h"

using namespace solo;


MaterialParameter::MaterialParameter(const std::string& name, Material *material):
	material(material),
	name(name)
{
}


void MaterialParameter::setFloat(float value)
{
	tryClearOldValue(ValueType::Float);
	floatValue = value;
	type = ValueType::Float;
}


void MaterialParameter::setFloatArray(const std::vector<float>& value)
{
	tryClearOldValue(ValueType::FloatArray);
	floatArrayValue = value;
	type = ValueType::FloatArray;
}


void MaterialParameter::setInt(int value)
{
	tryClearOldValue(ValueType::Int);
	intValue = value;
	type = ValueType::Int;
}


void MaterialParameter::setIntArray(const std::vector<int>& value)
{
	tryClearOldValue(ValueType::IntArray);
	intArrayValue = value;
	type = ValueType::IntArray;
}


void MaterialParameter::setVector2(const Vector2 &value)
{
	tryClearOldValue(ValueType::Vector2);
	vector2Value = value;
	type = ValueType::Vector2;
}


void MaterialParameter::setVector2Array(const std::vector<Vector2>& value)
{
	tryClearOldValue(ValueType::Vector2Array);
	vector2ArrayValue = value;
	type = ValueType::Vector2Array;
}


void MaterialParameter::setVector3(const Vector3 &value)
{
	tryClearOldValue(ValueType::Vector3);
	vector3Value = value;
	type = ValueType::Vector3;
}


void MaterialParameter::setVector3Array(const std::vector<Vector3>& value)
{
	tryClearOldValue(ValueType::Vector3Array);
	vector3ArrayValue = value;
	type = ValueType::Vector3Array;
}


void MaterialParameter::setVector4(const Vector4& value)
{
	tryClearOldValue(ValueType::Vector4);
	vector4Value = value;
	type = ValueType::Vector4;
}


void MaterialParameter::setVector4Array(const std::vector<Vector4>& value)
{
	tryClearOldValue(ValueType::Vector4Array);
	vector4ArrayValue = value;
	type = ValueType::Vector4Array;
}


void MaterialParameter::setMatrix(const Matrix& value)
{
	tryClearOldValue(ValueType::Matrix);
	matrixValue = value;
	type = ValueType::Matrix;
}


void MaterialParameter::setMatrixArray(const std::vector<Matrix>& value)
{
	tryClearOldValue(ValueType::MatrixArray);
	matrixArrayValue = value;
	type = ValueType::MatrixArray;
}


void MaterialParameter::setTexture(const shared<Texture> texture)
{
	tryClearOldValue(ValueType::Texture);
	textureValue = texture;
	type = ValueType::Texture;
}


void MaterialParameter::setTextureArray(const std::vector<shared<Texture>>& textures)
{
	tryClearOldValue(ValueType::TextureArray);
	textureArrayValue = textures;
	type = ValueType::TextureArray;
}


void MaterialParameter::setFunction(std::function<void(EffectVariable* variable, const RenderContext& context)> func)
{
	tryClearOldValue(ValueType::Func);
	this->funcValue = func;
	type = ValueType::Func;
}


void MaterialParameter::apply(const RenderContext& context)
{
	auto variable = material->getEffect()->findVariable(name);
	if (!variable)
		return;
	switch (type)
	{
		case ValueType::Float:
			variable->setFloat(floatValue);
			break;
		case ValueType::FloatArray:
			variable->setFloatArray(floatArrayValue);
			break;
		case ValueType::Int:
			variable->setFloat(intValue);
			break;
		case ValueType::IntArray:
			variable->setIntArray(intArrayValue);
			break;
		case ValueType::Vector2:
			variable->setVector2(vector2Value);
			break;
		case ValueType::Vector2Array:
			variable->setVector2Array(vector2ArrayValue);
			break;
		case ValueType::Vector3:
			variable->setVector3(vector3Value);
			break;
		case ValueType::Vector3Array:
			variable->setVector3Array(vector3ArrayValue);
			break;
		case ValueType::Vector4:
			variable->setVector4(vector4Value);
			break;
		case ValueType::Vector4Array:
			variable->setVector4Array(vector4ArrayValue);
			break;
		case ValueType::Matrix:
			variable->setMatrix(matrixValue);
			break;
		case ValueType::MatrixArray:
			variable->setMatrixArray(matrixArrayValue);
			break;
		case ValueType::Texture:
			variable->setTexture(textureValue);
			break;
		case ValueType::TextureArray:
			variable->setTextureArray(textureArrayValue);
			break;
		case ValueType::Func:
			funcValue(variable, context);
			break;
		case ValueType::None:
		default:
			break;
	}
}


void MaterialParameter::tryClearOldValue(ValueType newExpectedValue)
{
	if (newExpectedValue == type)
		return;
	switch (type)
	{
		case ValueType::FloatArray:
			floatArrayValue.clear();
			break;
		case ValueType::IntArray:
			intArrayValue.clear();
			break;
		case ValueType::Vector2Array:
			vector2ArrayValue.clear();
			break;
		case ValueType::Vector3Array:
			vector3ArrayValue.clear();
			break;
		case ValueType::Vector4Array:
			vector4ArrayValue.clear();
			break;
		case ValueType::MatrixArray:
			matrixArrayValue.clear();
			break;
		case ValueType::Texture:
			textureValue = nullptr;
			break;
		case ValueType::TextureArray:
			textureArrayValue.clear();
			break;
		case ValueType::Func:
			funcValue = nullptr;
			break;
		case ValueType::None:
		default:
			break;
	}
	type = ValueType::None;
}


void MaterialParameter::setWorldMatrix(EffectVariable *variable, const RenderContext& context)
{
	variable->setMatrix(context.nodeTransform->getWorldMatrix());
}


void MaterialParameter::setViewMatrix(EffectVariable *variable, const RenderContext& context)
{
	variable->setMatrix(context.camera->getViewMatrix());
}


void MaterialParameter::setProjectionMatrix(EffectVariable *variable, const RenderContext& context)
{
	variable->setMatrix(context.camera->getProjectionMatrix());
}


void MaterialParameter::setWorldViewMatrix(EffectVariable *variable, const RenderContext& context)
{
	variable->setMatrix(context.nodeTransform->getWorldViewMatrix(context.camera));
}


void MaterialParameter::setViewProjectionMatrix(EffectVariable *variable, const RenderContext& context)
{
	variable->setMatrix(context.camera->getViewProjectionMatrix());
}


void MaterialParameter::setWorldViewProjectionMatrix(EffectVariable *variable, const RenderContext& context)
{
	variable->setMatrix(context.nodeTransform->getWorldViewProjectionMatrix(context.camera));
}


void MaterialParameter::setInverseTransposedWorldViewMatrix(EffectVariable *variable, const RenderContext& context)
{
	variable->setMatrix(context.nodeTransform->getInverseTransposedWorldViewMatrix(context.camera));
}


void MaterialParameter::setInverseTransposedWorldMatrix(EffectVariable *variable, const RenderContext& context)
{
	variable->setMatrix(context.nodeTransform->getInverseTransposedWorldMatrix());
}


void MaterialParameter::setCameraWorldPosition(EffectVariable *variable, const RenderContext& context)
{
	variable->setVector3(context.cameraTransform->getWorldPosition());
}


void MaterialParameter::bindValue(AutoBinding autoBinding)
{
	switch (autoBinding)
	{
		case AutoBinding::WorldMatrix:
			setFunction(&MaterialParameter::setWorldMatrix);
			break;
		case AutoBinding::ViewMatrix:
			setFunction(&MaterialParameter::setViewMatrix);
			break;
		case AutoBinding::ProjectionMatrix:
			setFunction(&MaterialParameter::setProjectionMatrix);
			break;
		case AutoBinding::WorldViewMatrix:
			setFunction(&MaterialParameter::setWorldViewMatrix);
			break;
		case AutoBinding::ViewProjectionMatrix:
			setFunction(&MaterialParameter::setViewProjectionMatrix);
			break;
		case AutoBinding::WorldViewProjectionMatrix:
			setFunction(&MaterialParameter::setWorldViewProjectionMatrix);
			break;
		case AutoBinding::InverseTransposedWorldMatrix:
			setFunction(&MaterialParameter::setInverseTransposedWorldMatrix);
			break;
		case AutoBinding::InverseTransposedWorldViewMatrix:
			setFunction(&MaterialParameter::setInverseTransposedWorldViewMatrix);
			break;
		case AutoBinding::CameraWorldPosition:
			setFunction(&MaterialParameter::setCameraWorldPosition);
			break;
		case AutoBinding::None:
		default:
			break;
	}
}
