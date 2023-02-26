/*
 * Copyright (c) Aleksey Fedotov
 * MIT license
 */

#pragma once

#include "SoloCommon.h"
#include <functional>

namespace solo {
    class Device;
    class Camera;
    class Mesh;
    class Transform;
    class Material;
    class DebugInterface;

    class Renderer {
    public:
        static auto fromDevice(Device *device) -> sptr<Renderer>;

        Renderer(const Renderer &other) = delete;
        Renderer(Renderer &&other) = delete;
        virtual ~Renderer() = default;

        auto operator=(const Renderer &other) -> Renderer & = delete;
        auto operator=(Renderer &&other) -> Renderer & = delete;

        void bootstrap();
        void cleanup();

        virtual void beginCamera(Camera *camera) = 0;
        virtual void endCamera(Camera *camera) = 0;
        virtual void renderMesh(Mesh *mesh, Transform *transform, Material *material) = 0;
        virtual void renderMeshIndex(Mesh *mesh, u32 part, Transform *transform, Material *material) = 0;
        virtual void renderDebugInterface(DebugInterface *debugInterface) = 0;

        virtual auto name() const -> const char * = 0;
        virtual auto gpuName() const -> const char * = 0;

        void renderFrame(const std::function<void()> &render);

    protected:
        Renderer(Device *device);

        virtual void beginFrame() = 0;
        virtual void endFrame() = 0;

        auto errorMaterial() const -> Material * { return errorMaterial_.get(); }

    private:
        Device *device_ = nullptr;
        sptr<Material> errorMaterial_;
    };
}