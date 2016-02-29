#pragma once

namespace solo
{
    class Renderer
    {
    public:
        void createVertexBuffer() {}
        void destroyVertexBuffer() {}

        void setIndexBuffer() {}
        void createDynamicIndexBuffer() {}
        void updateDynamicIndexBuffer() {}
        void destroyDynamicIndexBuffer() {}

        void createDynamicVertexBuffer() {}
        void updateDynamicVertexBuffer() {}
        void destroyDynamicVertexBuffer() {}

        void createShader() {}
        void getShaderUniforms() {}
        void destroyShader() {}

        void createProgram() {}
        void destroyProgram() {}

        void setTexture() {}

        void create2DTexture() {}
        void update2DTexture() {}

        void create3DTexture() {} 
        void update3DTexture() {}

        void createCubeTexture() {}
        void updateCubeTexture() {}

        void destroyTexture() {}

        void createFrameBuffer() {}
        void destroyFrameBuffer() {}

        void createUniform() {}
        void destroyUniform() {}

        void createOcclusionQuery() {}
        void getOcclusionQueryResult() {}
        void destroyOcclusionQuery() {}

        void setViewRect() {}
        void setViewClear() {}

        void setViewFrameBuffer() {}

        void setViewTransform() {}

        void resetView() {}

        void setState() {}

        void setOcclusionCondition() {}

        void setStencil() {}

        void setTransform() {}

        void setUniform() {}

        void setVertexBuffer() {}

        void submitForRendering() {}
        void render() {}

        void blit() {}
    };
}