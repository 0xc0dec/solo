/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloVulkanMaterial.h"

#ifdef SL_VULKAN_RENDERER

#include "SoloEffect.h"
#include "SoloDevice.h"
#include "SoloVulkanRenderer.h"

using namespace solo;

vk::Material::Material(sptr<Effect> effect):
    solo::Material(effect)
{
}

vk::Material::~Material()
{
}

void vk::Material::setFloatParameter(const std::string &name, float value)
{
}

void vk::Material::setFloatArrayParameter(const std::string &name, const std::vector<float> &value)
{
}

void vk::Material::setVector2Parameter(const std::string &name, const Vector2 &value)
{
}

void vk::Material::setVector2ArrayParameter(const std::string &name, const std::vector<Vector2> &value)
{
}

void vk::Material::setVector3Parameter(const std::string &name, const Vector3 &value)
{
}

void vk::Material::setVector3ArrayParameter(const std::string &name, const std::vector<Vector3> &value)
{
}

void vk::Material::setVector4Parameter(const std::string &name, const Vector4 &value)
{
}

void vk::Material::setVector4ArrayParameter(const std::string &name, const std::vector<Vector4> &value)
{
}

void vk::Material::setMatrixParameter(const std::string &name, const Matrix &value)
{
}

void vk::Material::setMatrixArrayParameter(const std::string &name, const std::vector<Matrix> &value)
{
}

void vk::Material::setTextureParameter(const std::string &name, sptr<Texture> value)
{
}

void vk::Material::bindWorldMatrixParameter(const std::string &name)
{
}

void vk::Material::bindViewMatrixParameter(const std::string &name)
{
}

void vk::Material::bindProjectionMatrixParameter(const std::string &name)
{
}

void vk::Material::bindWorldViewMatrixParameter(const std::string &name)
{
}

void vk::Material::bindViewProjectionMatrixParameter(const std::string &name)
{
}

void vk::Material::bindWorldViewProjectionMatrixParameter(const std::string &name)
{
}

void vk::Material::bindInvTransposedWorldMatrixParameter(const std::string &name)
{
}

void vk::Material::bindInvTransposedWorldViewMatrixParameter(const std::string &name)
{
}

void vk::Material::bindCameraWorldPositionParameter(const std::string &name)
{
}

#endif
