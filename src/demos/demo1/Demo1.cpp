/*
    Copyright (c) Aleksey Fedotov

    This software is provided 'as-is', without any express or implied
    warranty. In no event will the authors be held liable for any damages
    arising from the use of this software.

    Permission is granted to anyone to use this software for any purpose,
    including commercial applications, and to alter it and redistribute it
    freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not
        claim that you wrote the original software. If you use this software
        in a product, an acknowledgment in the product documentation would be
        appreciated but is not required.
    2. Altered source versions must be plainly marked as such, and must not be
        misrepresented as being the original software.
    3. This notice may not be removed or altered from any source distribution.
*/

#include "../../../include/Solo.h"
#include "../common/Screenshoter.h"
#include "../common/Rotator.h"
#include "../common/Shaders.h"
#include "Shaders.h"
#include <cassert>

using namespace solo;


class Targeter: public ComponentBase<Targeter>
{
public:
    explicit Targeter(const Node &node, Vector3 targetPos):
        ComponentBase<Targeter>(node),
        targetPos(targetPos)
    {
    }

    void init() override final
    {
        transform = node.findComponent<Transform>();
    }

    void update() override final
    {
        transform->lookAt(targetPos, Vector3::unitY());
    }

private:
    Vector3 targetPos;
    Transform *transform = nullptr;
};


class DynamicQuadUpdater: public ComponentBase<DynamicQuadUpdater>
{
public:
    explicit DynamicQuadUpdater(const Node &node, std::vector<float> data, sptr<Mesh> mesh):
        ComponentBase<DynamicQuadUpdater>(node),
        device(node.getScene()->getDevice()),
        data(data),
        mesh(mesh)
    {
    }

    void update() override final
    {
        time += 2 * device->getTimeDelta();
        auto offset = 0.3f * sinf(time);
        data[2] = offset;
        data[7] = -offset;
        data[12] = offset;
        data[17] = -offset;
        mesh->updateDynamicVertexBuffer(0, 0, data.data(), 4);
    }

private:
    float time = 0;
    Device *device;
    std::vector<float> data;
    sptr<Mesh> mesh;
};


class Demo
{
public:
    Demo(Device *device):
        device(device), scene(device->getScene()),
        loader(device->getAssetLoader()), canvasSize(device->getCanvasSize())
    {
        initEffects();
        initCamera();
        initOffscreenCamera();
        initMaterials();
        initSkybox();
        initCheckerBox();
        initMesh();
        initDynamicQuad();
        initAxesMesh()->done([&] (sptr<Mesh> mesh)
        {
            axesMesh = mesh;
            initMonitorQuad(Vector3());
            initTransparentQuad();
        });
    }

    void update()
    {
        scene->visit([](Component *cmp)
        {
            cmp->update();
        });
    }

    void render()
    {
        renderOffscreenCamera();
        renderMainCamera();
    }

private:
    void renderOffscreenCamera()
    {
        offscreenCamera->apply([&](const RenderContext& ctx)
        {
            if (skybox)
                skybox->render(ctx);

            scene->visit([=](Component *cmp)
            {
                if (cmp != monitorQuad && cmp != skybox && cmp != transparentQuad)
                    cmp->render(ctx);
            });

            if (transparentQuad)
                transparentQuad->render(ctx);
        });
    }

    void renderMainCamera()
    {
        mainCamera->apply([&](const RenderContext& ctx)
        {
            if (skybox)
                skybox->render(ctx);

            scene->visit([=](Component *cmp)
            {
                if (cmp != skybox && cmp != transparentQuad)
                    cmp->render(ctx);
            });

            if (transparentQuad)
                transparentQuad->render(ctx);
        });
    }

    auto createColorMaterial(const Vector4 &color) -> sptr<Material>
    {
        auto mat = Material::create(device, colorEffect);
        mat->setFaceCull(FaceCull::All);
        mat->bindWorldViewProjectionMatrixParameter("worldViewProjMatrix");
        mat->setVector4Parameter("color", color);
        return mat;
    }

    void initEffects()
    {
        simpleTextureEffect = Effect::create(device, commonShaders.vertex.basic, commonShaders.fragment.texture);
        colorEffect = Effect::create(device, commonShaders.vertex.basic, commonShaders.fragment.color);
        checkerEffect = Effect::create(device, commonShaders.vertex.basic, fsChecker);
        texWithLightingEffect = Effect::create(device, vsBasicLighting, fsTextureWithLighting);
    }

    void loadTexture(const std::string &path, std::function<void(sptr<RectTexture>)> callback)
    {
        loader->loadRectTextureAsync(path)->done([=](sptr<RectTexture> tex)
        {
            tex->generateMipmaps();
            tex->setFiltering(TextureFiltering::LinearMipmapNearest);
            tex->setAnisotropyLevel(8);
            callback(tex);
        });
    }

    auto initAxesMesh() -> sptr<AsyncHandle<Mesh>>
    {
        return loader->loadMeshAsync("../assets/axes.obj");
    }

    void initCamera()
    {
        auto node = scene->createNode();
        auto t = node->findComponent<Transform>();
        t->setLocalPosition(Vector3(0, 5, 10));
        t->lookAt(Vector3::zero(), Vector3::unitY());
        node->addComponent<Screenshoter>("demo1-screenshot.bmp");

        auto spectator = node->addComponent<Spectator>();
        spectator->setVerticalRotationSpeed(1);
        spectator->setHorizontalRotationSpeed(1);

        mainCamera = node->addComponent<Camera>();
        mainCamera->setClearColor(0.0f, 0.6f, 0.6f, 1.0f);
        mainCamera->setNear(0.05f);
    }

    void initOffscreenCamera()
    {
        offscreenCameraTex = RectTexture::create(device);
        offscreenCameraTex->setData(TextureFormat::RGB, {},
            static_cast<uint32_t>(floor(canvasSize.x / 8.0f)), static_cast<uint32_t>(floor(canvasSize.y / 8.0f)));
        offscreenCameraTex->setFiltering(TextureFiltering::Nearest);
        offscreenCameraTex->setWrapping(TextureWrapping::Clamp);

        auto node = scene->createNode();
        offscreenCamera = node->addComponent<Camera>();
        offscreenCamera->setClearColor(1, 0, 1, 1);
        offscreenCamera->setNear(0.05f);
        offscreenCamera->setViewport(0, 0, canvasSize.x / 8, canvasSize.y / 8);
        node->findComponent<Transform>()->setLocalPosition(Vector3(0, 0, 10));

        auto fb = FrameBuffer::create(device);
        fb->setAttachments({ offscreenCameraTex });
        offscreenCamera->setRenderTarget(fb);
    }

    void initMaterials()
    {
        redMat = createColorMaterial(Vector4(1, 0, 0, 1));
        greenMat = createColorMaterial(Vector4(0, 1, 0, 1));
        blueMat = createColorMaterial(Vector4(0, 0, 1, 1));
        whiteMat = createColorMaterial(Vector4(1, 1, 1, 1));

        checkerMat = Material::create(device, checkerEffect);
        checkerMat->setFaceCull(FaceCull::All);
        checkerMat->bindWorldViewProjectionMatrixParameter("worldViewProjMatrix");
        checkerMat->setVector4Parameter("color", Vector4(1, 1, 0, 1));

        monitorMat = Material::create(device, simpleTextureEffect);
        monitorMat->setFaceCull(FaceCull::All);
        monitorMat->bindWorldViewProjectionMatrixParameter("worldViewProjMatrix");
        monitorMat->setTextureParameter("mainTex", offscreenCameraTex);
    }

    void initSkybox()
    {
        loader->loadCubeTextureAsync(
        {
            "../assets/skyboxes/deep-space/front.png",
            "../assets/skyboxes/deep-space/back.png",
            "../assets/skyboxes/deep-space/left.png",
            "../assets/skyboxes/deep-space/right.png",
            "../assets/skyboxes/deep-space/top.png",
            "../assets/skyboxes/deep-space/bottom.png"
        })->done([=](sptr<CubeTexture> tex)
        {
            tex->setWrapping(TextureWrapping::Clamp);
            tex->setFiltering(TextureFiltering::Linear);
            auto node = scene->createNode();
            skybox = node->addComponent<SkyboxRenderer>();
            skybox->setTexture(tex);
        });
    }

    auto createPrefabMeshNode(MeshPrefab prefab) -> sptr<Node>
    {
        auto mesh = Mesh::create(device, prefab);
        assert(mesh != nullptr);
        auto node = scene->createNode();
        node->addComponent<MeshRenderer>()->setMesh(mesh);
        return node;
    };

    void initCheckerBox()
    {
        auto node = createPrefabMeshNode(MeshPrefab::Cube);
        node->findComponent<MeshRenderer>()->setMaterial(0, checkerMat);
        node->findComponent<Transform>()->setLocalPosition(Vector3(-5, 0, 0));
        node->addComponent<Rotator>("world", Vector3::unitY());
    }

    void initMesh()
    {
        loader->loadRectTextureAsync("../assets/cobblestone.png")->done([=](sptr<RectTexture> tex)
        {
            tex->setWrapping(TextureWrapping::Clamp);
            tex->generateMipmaps();

            auto mat = Material::create(device, texWithLightingEffect);
            mat->setFaceCull(FaceCull::All);
            mat->bindWorldViewProjectionMatrixParameter("worldViewProjMatrix");
            mat->bindInvTransposedWorldMatrixParameter("invTransposedWorldMatrix");
            mat->setTextureParameter("mainTex", tex);

            loader->loadMeshAsync("../assets/monkey_hires.obj")->done([=](sptr<Mesh> mesh)
            {
                auto node = scene->createNode();
                auto renderer = node->addComponent<MeshRenderer>();
                renderer->setMesh(mesh);
                renderer->setMaterial(0, mat);
                node->findComponent<Transform>()->setLocalPosition(Vector3::zero());
                node->addComponent<Rotator>("local", Vector3::unitX());
            });
        });
    }

    void initDynamicQuad()
    {
        loadTexture("../assets/freeman.png", [=](sptr<RectTexture> tex)
        {
            tex->setWrapping(TextureWrapping::Clamp);

            auto layout = VertexBufferLayout();
            layout.add(VertexBufferLayoutSemantics::Position, 3);
            layout.add(VertexBufferLayoutSemantics::TexCoord0, 2);

            std::vector<float> data =
            {
                -1, -1, 0,	0, 0,
                -1,  1, 0,	0, 1,
                1,  1, 0,	1, 1,
                1, -1, 0,	1, 0
            };

            std::vector<uint16_t> indices =
            {
                0, 1, 2,
                0, 2, 3
            };

            auto mesh = Mesh::create(device);
            mesh->addDynamicVertexBuffer(layout, data.data(), 4);
            mesh->addPart(indices.data(), 6);
            mesh->setPrimitiveType(PrimitiveType::Triangles);

            auto mat = Material::create(device, simpleTextureEffect);
            mat->setFaceCull(FaceCull::All);
            mat->bindWorldViewProjectionMatrixParameter("worldViewProjMatrix");
            mat->setTextureParameter("mainTex", tex);

            auto node = scene->createNode();
            node->findComponent<Transform>()->setLocalPosition(Vector3(0, 0, -5));
            auto renderer = node->addComponent<MeshRenderer>();
            renderer->setMesh(mesh);
            renderer->setMaterial(0, mat);

            node->addComponent<DynamicQuadUpdater>(data, mesh);
        });
    }

    void attachAxesMesh(sptr<Node> node)
    {
        auto renderer = node->addComponent<MeshRenderer>();
        renderer->setMesh(axesMesh);
        renderer->setMaterial(0, blueMat);
        renderer->setMaterial(1, greenMat);
        renderer->setMaterial(2, whiteMat);
        renderer->setMaterial(3, redMat);
    }

    void initMonitorQuad(Vector3 targetPos)
    {
        auto parent = scene->createNode();
        parent->findComponent<Transform>()->setLocalPosition(Vector3(-2, 2, -2));
        parent->addComponent<Rotator>("world", Vector3::unitY());
        attachAxesMesh(parent);

        auto quad = createPrefabMeshNode(MeshPrefab::Quad);
        auto transform = quad->findComponent<Transform>();
        transform->setParent(parent->findComponent<Transform>());
        transform->setLocalPosition(Vector3(5, 2, -5));
        transform->setLocalScale(Vector3(5, 5 * canvasSize.y / canvasSize.x, 1));
        quad->addComponent<Targeter>(targetPos);
        monitorQuad = quad->findComponent<MeshRenderer>();
        monitorQuad->setMaterial(0, monitorMat);
    }

    void initTransparentQuad()
    {
        loadTexture("../assets/flammable.png", [=](sptr<RectTexture> tex)
        {
            tex->setWrapping(TextureWrapping::Clamp);

            auto mat = Material::create(device, simpleTextureEffect);
            mat->setFaceCull(FaceCull::All);
            mat->bindWorldViewProjectionMatrixParameter("worldViewProjMatrix");
            mat->setTextureParameter("mainTex", tex);
            mat->setTransparent(true);

            auto parent = scene->createNode();
            parent->findComponent<Transform>()->setLocalPosition(Vector3(5, 0, 0));
            parent->addComponent<Rotator>("world", Vector3::unitY());
            attachAxesMesh(parent);

            auto quad = createPrefabMeshNode(MeshPrefab::Quad);
            quad->addComponent<Rotator>("local", Vector3::unitX());
            quad->findComponent<Transform>()->setParent(parent->findComponent<Transform>());
            quad->findComponent<Transform>()->setLocalPosition(Vector3(2, 0, 0));

            transparentQuad = quad->findComponent<MeshRenderer>();
            transparentQuad->setMaterial(0, mat);
            transparentQuad->setRenderQueue(KnownRenderQueues::Transparent);
        });
    }

    const int renderTargetQuadTag = 2;
    Device *device = nullptr;
    Scene *scene = nullptr;
    AssetLoader *loader = nullptr;
    Vector2 canvasSize;
    Camera *mainCamera = nullptr;
    Camera *offscreenCamera = nullptr;
    MeshRenderer *monitorQuad = nullptr;
    SkyboxRenderer *skybox = nullptr;
    MeshRenderer *transparentQuad = nullptr;
    sptr<Effect> simpleTextureEffect = nullptr;
    sptr<Effect> colorEffect = nullptr;
    sptr<Effect> checkerEffect = nullptr;
    sptr<Effect> texWithLightingEffect = nullptr;
    sptr<Mesh> axesMesh = nullptr;
    sptr<Material> redMat = nullptr;
    sptr<Material> greenMat = nullptr;
    sptr<Material> blueMat = nullptr;
    sptr<Material> whiteMat = nullptr;
    sptr<Material> checkerMat = nullptr;
    sptr<Material> monitorMat = nullptr;
    sptr<RectTexture> offscreenCameraTex = nullptr;
};


int main()
{
    auto device = Device::create(DeviceSetup().withMode(DeviceMode::OpenGL).withDimensions(1200, 600).withLogFilePath("demo1.log"));
    Demo demo(device.get());
    while (!device->isQuitRequested() && !device->isWindowCloseRequested() && !device->isKeyPressed(KeyCode::Escape, true))
    {
        device->beginUpdate();
        device->getAssetLoader()->update();
        device->getRenderer()->beginFrame();

        demo.update();
        demo.render();

        device->getRenderer()->endFrame();
        device->endUpdate();
    }
    return 0;
}
