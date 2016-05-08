#include "SoloVulkanRenderer.h"

using namespace solo;


VulkanRenderer::VulkanRenderer()
{
}


VulkanRenderer::~VulkanRenderer()
{
}


auto VulkanRenderer::createTexture() -> TextureHandle
{
    return EmptyTextureHandle;
}


void VulkanRenderer::destroyTexture(const TextureHandle& handle)
{
}


void VulkanRenderer::set2DTexture(const TextureHandle& handle)
{
}


void VulkanRenderer::set2DTexture(const TextureHandle& handle, uint32_t flags)
{
}


void VulkanRenderer::set2DTexture(const TextureHandle& handle, uint32_t flags, float anisotropyLevel)
{
}


void VulkanRenderer::setCubeTexture(const TextureHandle& handle)
{
}


void VulkanRenderer::setCubeTexture(const TextureHandle& handle, uint32_t flags)
{
}


void VulkanRenderer::setCubeTexture(const TextureHandle& handle, uint32_t flags, float anisotropyLevel)
{
}


void VulkanRenderer::update2DTexture(const TextureHandle& handle, TextureFormat format, uint32_t width, uint32_t height, const void* data)
{
}


void VulkanRenderer::updateCubeTexture(const TextureHandle& handle, CubeTextureFace face, TextureFormat format, uint32_t width, uint32_t height, const void* data)
{
}


void VulkanRenderer::generateTexture2DMipmaps(const TextureHandle& handle)
{
}


void VulkanRenderer::generateCubeTextureMipmaps(const TextureHandle& handle)
{
}


auto VulkanRenderer::createFrameBuffer() -> FrameBufferHandle
{
    return EmptyFrameBufferHandle;
}


void VulkanRenderer::destroyFrameBuffer(const FrameBufferHandle& handle)
{
}


void VulkanRenderer::setFrameBuffer(const FrameBufferHandle& handle)
{
}


void VulkanRenderer::updateFrameBuffer(const FrameBufferHandle& handle, const std::vector<TextureHandle>& attachmentHandles)
{
}


auto VulkanRenderer::createVertexBuffer(const VertexBufferLayout& layout, const void* data, uint32_t vertexCount) -> VertexBufferHandle
{
    return EmptyVertexBufferHandle;
}


auto VulkanRenderer::createDynamicVertexBuffer(const VertexBufferLayout& layout, const void* data, uint32_t vertexCount) -> VertexBufferHandle
{
    return EmptyVertexBufferHandle;
}


void VulkanRenderer::updateDynamicVertexBuffer(const VertexBufferHandle& handle, const void* data, uint32_t offset, uint32_t vertexCount)
{
}


void VulkanRenderer::destroyVertexBuffer(const VertexBufferHandle& handle)
{
}


auto VulkanRenderer::createIndexBuffer(const void* data, uint32_t elementSize, uint32_t elementCount) -> IndexBufferHandle
{
    return EmptyIndexBufferHandle;
}


void VulkanRenderer::destroyIndexBuffer(const IndexBufferHandle& handle)
{
}


auto VulkanRenderer::createProgram(const char* vsSrc, const char* fsSrc) -> ProgramHandle
{
    return EmptyProgramHandle;
}


void VulkanRenderer::destroyProgram(const ProgramHandle& handle)
{
}


void VulkanRenderer::setProgram(const ProgramHandle& handle)
{
}


auto VulkanRenderer::createVertexObject(const VertexBufferHandle* bufferHandles, uint32_t bufferCount, ProgramHandle programHandle) -> VertexObjectHandle
{
    return EmptyVertexObjectHandle;
}


void VulkanRenderer::destroyVertexObject(const VertexObjectHandle& handle)
{
}


auto VulkanRenderer::createUniform(const char* name, UniformType type, ProgramHandle program) -> UniformHandle
{
    return EmptyUniformHandle;
}


void VulkanRenderer::destroyUniform(const UniformHandle& handle)
{
}


void VulkanRenderer::setUniform(const UniformHandle& handle, const void* value, uint32_t count)
{
}


void VulkanRenderer::setPolygonFace(PolygonFace face)
{
}


void VulkanRenderer::setBlend(bool enabled)
{
}


void VulkanRenderer::setBlendFactor(BlendFactor srcFactor, BlendFactor dstFactor)
{
}


void VulkanRenderer::setDepthWrite(bool enabled)
{
}


void VulkanRenderer::setDepthTest(bool enabled)
{
}


void VulkanRenderer::setDepthFunction(DepthFunction func)
{
}


void VulkanRenderer::setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
{
}


void VulkanRenderer::clear(bool color, bool depth, float r, float g, float b, float a)
{
}


void VulkanRenderer::drawIndexedVertexObject(PrimitiveType primitiveType, const VertexObjectHandle& vertexObjectHandle, const IndexBufferHandle& indexBufferHandle)
{
}


void VulkanRenderer::drawVertexObject(PrimitiveType primitiveType, const VertexObjectHandle& vertexObjectHandle, uint32_t vertexCount)
{
}
