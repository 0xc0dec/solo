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

#include "../common/DemoBase.h"
#include "../common/Screenshoter.h"
#include "Rotator.h"
#include "DynamicQuadUpdater.h"
#include "LookAt.h"
#include "CurrentTimeText.h"
#include "PostProcessor1.h"
#include "PostProcessor2.h"
#include "SpawnedObjectTargeter.h"
#include "Spawner.h"
#include "Shaders.h"

using namespace solo;


class Demo final: public DemoBase
{
public:
    explicit Demo(Device *device): DemoBase(device)
    {
        initCommonAssets();
        initCamera();
        initOffscreenCamera();
        initSkybox();
        initCheckerBox();
        initDynamicQuad();
        initCurrentTimeLabel();
        loadTexture("../../assets/cobblestone.png", [&] (sptr<Texture> texture)
        {
            initMonkeyHead(texture);
            initFloor(texture);
        });
        loader->loadMeshAsync("../../assets/axes.obj")->done([&] (sptr<Mesh> mesh)
        {
            initMonitorQuad({}, mesh);
            initTransparentQuad(mesh);
        });
    }

private:
    void render() override final
    {
        offscreenCamera->render([&](const RenderContext& ctx)
        {
            renderByTags(skyboxTag, ctx);
            renderByTags(~(skyboxTag | monitorQuadTag | transparentTag | postProcessorTag), ctx);
            renderByTags(transparentTag, ctx);
        });

        mainCamera->render([&](const RenderContext& ctx)
        {
            renderByTags(skyboxTag, ctx);
            renderByTags(~(skyboxTag | transparentTag | postProcessorTag), ctx);
            renderByTags(transparentTag, ctx);
        });

        if (pp1)
            pp1->apply();
        else if (pp2)
            pp2->apply();
    }

    void update() override final
    {
        DemoBase::update();
        switchPostProcessors();
    }

    auto createColorMaterial(const Vector4 &color) const -> sptr<Material>
    {
        auto mat = Material::create(device, colorEffect);
        mat->setFaceCull(FaceCull::All);
        mat->bindWorldViewProjectionMatrixParameter("worldViewProjMatrix");
        mat->setVector4Parameter("color", color);
        return mat;
    }

    void initCommonAssets()
    {
        simpleTextureEffect = Effect::create(device, vsBasic, fsTexture);
        colorEffect = Effect::create(device, vsBasic, fsColor);
        redMat = createColorMaterial({1, 0, 0, 1});
        greenMat = createColorMaterial({0, 1, 0, 1});
        blueMat = createColorMaterial({0, 0, 1, 1});
        whiteMat = createColorMaterial({1, 1, 1, 1});
        cubeMesh = Mesh::createFromPrefab(device, MeshPrefab::Cube);
        quadMesh = Mesh::createFromPrefab(device, MeshPrefab::Quad);
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

    void initCamera()
    {
        auto node = scene->createNode();
        
        auto t = node->findComponent<Transform>();
        t->setLocalPosition({0, 5, 10});
        t->lookAt({}, Vector3::unitY());
        
        node->addComponent<Screenshoter>("demo1.bmp");
        node->addComponent<Spectator>();
        node->addComponent<SpawnedObjectTargeter>();
        node->addComponent<Spawner>(cubeMesh);

        mainCamera = node->addComponent<Camera>();
        mainCamera->setClearColor({0.0f, 0.6f, 0.6f, 1.0f});
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
        node->findComponent<Transform>()->setLocalPosition({0, 0, 10});

        offscreenCamera = node->addComponent<Camera>();
        offscreenCamera->setClearColor({1, 0, 1, 1});
        offscreenCamera->setNear(0.05f);
        offscreenCamera->setViewport({0, 0, canvasSize.x / 8, canvasSize.y / 8});

        auto fb = FrameBuffer::create(device);
        fb->setAttachments({offscreenCameraTex});
        offscreenCamera->setRenderTarget(fb);
    }

    void initSkybox()
    {
        loader->loadCubeTextureAsync(
        {
            "../../assets/skyboxes/deep-space/front.png",
            "../../assets/skyboxes/deep-space/back.png",
            "../../assets/skyboxes/deep-space/left.png",
            "../../assets/skyboxes/deep-space/right.png",
            "../../assets/skyboxes/deep-space/top.png",
            "../../assets/skyboxes/deep-space/bottom.png"
        })->done([=](sptr<CubeTexture> tex)
        {
            tex->setWrapping(TextureWrapping::Clamp);
            tex->setFiltering(TextureFiltering::Linear);
            auto node = scene->createNode();
            auto renderer = node->addComponent<SkyboxRenderer>();
            renderer->setTexture(tex);
            renderer->setTags(skyboxTag);
        });
    }

    void initCheckerBox() const
    {
        auto effect = Effect::create(device, vsBasic, fsChecker);

        auto material = Material::create(device, effect);
        material->setFaceCull(FaceCull::All);
        material->bindWorldViewProjectionMatrixParameter("worldViewProjMatrix");
        material->setVector4Parameter("color", {1, 1, 0, 1});

        auto node = scene->createNode();
        node->addComponent<MeshRenderer>()->setMesh(cubeMesh);
        node->findComponent<MeshRenderer>()->setMaterial(0, material);
        node->findComponent<Transform>()->setLocalPosition({-5, 0, 0});
        node->addComponent<Rotator>("world", Vector3::unitY());
    }

    void initMonkeyHead(sptr<Texture> texture)
    {
        auto effect = Effect::create(device, vsBasicLighting, fsTextureWithLighting);

        auto material = Material::create(device, effect);
        material->setFaceCull(FaceCull::All);
        material->bindWorldViewProjectionMatrixParameter("worldViewProjMatrix");
        material->bindInvTransposedWorldMatrixParameter("invTransposedWorldMatrix");
        material->setTextureParameter("mainTex", texture);

        loader->loadMeshAsync("../../assets/monkey_hires.obj")->done([=](sptr<Mesh> mesh)
        {
            auto node = scene->createNode();
            auto renderer = node->addComponent<MeshRenderer>();
            renderer->setMesh(mesh);
            renderer->setMaterial(0, material);
            node->findComponent<Transform>()->setLocalPosition({});
            node->addComponent<Rotator>("local", Vector3::unitX());
        });
    }

    void initFloor(sptr<Texture> texture) const
    {
        auto material = Material::create(device, simpleTextureEffect);
        material->setFaceCull(FaceCull::All);
        material->bindWorldViewProjectionMatrixParameter("worldViewProjMatrix");
        material->setTextureParameter("mainTex", texture);

        auto node = scene->createNode();
        
        node->findComponent<Transform>()->setLocalScale({10, 0.1f, 10});
        node->findComponent<Transform>()->setLocalPosition({0, -2, 0});

        auto renderer = node->addComponent<MeshRenderer>();
        renderer->setMesh(cubeMesh);
        renderer->setMaterial(0, material);

        auto params = RigidBodyConstructionParameters();
        params.mass = 0;
        params.friction = 0.5f;
        auto rigidBody = node->addComponent<RigidBody>(params);
        rigidBody->setCollider(BoxCollider::create(Vector3::unit()));
    }

    void initDynamicQuad()
    {
        loadTexture("../../assets/freeman.png", [=](sptr<RectTexture> tex)
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

            auto material = Material::create(device, simpleTextureEffect);
            material->setFaceCull(FaceCull::All);
            material->bindWorldViewProjectionMatrixParameter("worldViewProjMatrix");
            material->setTextureParameter("mainTex", tex);

            auto node = scene->createNode();
            node->findComponent<Transform>()->setLocalPosition({0, 0, -5});
            auto renderer = node->addComponent<MeshRenderer>();
            renderer->setMesh(mesh);
            renderer->setMaterial(0, material);

            node->addComponent<DynamicQuadUpdater>(data, mesh);
        });
    }

    void attachAxesMesh(sptr<Node> node, sptr<Mesh> axesMesh) const
    {
        auto renderer = node->addComponent<MeshRenderer>();
        renderer->setMesh(axesMesh);
        renderer->setMaterial(0, blueMat);
        renderer->setMaterial(1, greenMat);
        renderer->setMaterial(2, whiteMat);
        renderer->setMaterial(3, redMat);
    }

    void initMonitorQuad(Vector3 targetPos, sptr<Mesh> axesMesh) const
    {
        auto material = Material::create(device, simpleTextureEffect);
        material->setFaceCull(FaceCull::All);
        material->bindWorldViewProjectionMatrixParameter("worldViewProjMatrix");
        material->setTextureParameter("mainTex", offscreenCameraTex);

        auto parent = scene->createNode();
        parent->findComponent<Transform>()->setLocalPosition({-2, 2, -2});
        parent->addComponent<Rotator>("world", Vector3::unitY());
        attachAxesMesh(parent, axesMesh);

        auto node = scene->createNode();
        node->addComponent<LookAt>(targetPos);

        auto transform = node->findComponent<Transform>();
        transform->setParent(parent->findComponent<Transform>());
        transform->setLocalPosition({5, 2, -5});
        transform->setLocalScale({5, 5 * canvasSize.y / canvasSize.x, 1});
        
        auto renderer = node->addComponent<MeshRenderer>();
        renderer->setMesh(quadMesh);
        renderer->setMaterial(0, material);
        renderer->setTags(monitorQuadTag);
    }

    void initTransparentQuad(sptr<Mesh> axesMesh)
    {
        loadTexture("../../assets/flammable.png", [=](sptr<RectTexture> tex)
        {
            tex->setWrapping(TextureWrapping::Clamp);

            auto material = Material::create(device, simpleTextureEffect);
            material->setFaceCull(FaceCull::All);
            material->bindWorldViewProjectionMatrixParameter("worldViewProjMatrix");
            material->setTextureParameter("mainTex", tex);
            material->setBlend(true);
            material->setDepthTest(true);
            material->setDepthWrite(false);

            auto parent = scene->createNode();
            parent->findComponent<Transform>()->setLocalPosition({5, 0, 0});
            parent->addComponent<Rotator>("world", Vector3::unitY());
            attachAxesMesh(parent, axesMesh);

            auto node = scene->createNode();
            node->addComponent<Rotator>("local", Vector3::unitX());
            node->findComponent<Transform>()->setParent(parent->findComponent<Transform>());
            node->findComponent<Transform>()->setLocalPosition({2, 0, 0});

            auto renderer = node->addComponent<MeshRenderer>();
            renderer->setMesh(quadMesh);
            renderer->setMaterial(0, material);
            renderer->setTags(transparentTag);
        });
    }

    void initCurrentTimeLabel() const
    {
        auto node = scene->createNode();
        node->addComponent<CurrentTimeText>();
        node->findComponent<FontRenderer>()->setTags(transparentTag);
        node->findComponent<Transform>()->setLocalScale({0.02f, 0.02f, 1});
        node->findComponent<Transform>()->setLocalPosition({-3, 0, 4});
    }

    void switchPostProcessors()
    {
        if (device->isKeyPressed(KeyCode::Digit1, true))
        {
            if (!pp1)
            {
                pp2 = nullptr;
                pp1 = std::make_unique<PostProcessor1>(device, mainCamera, postProcessorTag);
            }
        }
        if (device->isKeyPressed(KeyCode::Digit2, true))
        {
            if (!pp2)
            {
                pp1 = nullptr;
                pp2 = std::make_unique<PostProcessor2>(device, mainCamera, postProcessorTag);
            }
        }
        if (device->isKeyPressed(KeyCode::Digit3, true))
        {
            pp1 = nullptr;
            pp2 = nullptr;
        }
    }

    const uint32_t skyboxTag = 1 << 1;
    const uint32_t transparentTag = 1 << 2;
    const uint32_t monitorQuadTag = 1 << 3;
    const uint32_t postProcessorTag = 1 << 4;
    Camera *mainCamera = nullptr;
    Camera *offscreenCamera = nullptr;
    sptr<Effect> simpleTextureEffect;
    sptr<Effect> colorEffect;
    sptr<Material> redMat;
    sptr<Material> greenMat;
    sptr<Material> blueMat;
    sptr<Material> whiteMat;
    sptr<RectTexture> offscreenCameraTex;
    sptr<Mesh> cubeMesh;
    sptr<Mesh> quadMesh;
    uptr<PostProcessor1> pp1;
    uptr<PostProcessor2> pp2;
};


int main()
{
    DeviceSetup setup;
    setup.mode = DeviceMode::OpenGL;
    setup.canvasWidth = 1200;
    setup.canvasHeight = 600;
    setup.logFilePath = "demo.log";
    setup.windowTitle = "Demo 1";
    
    auto device = Device::create(setup);
    Demo(device.get()).run();
    
    return 0;
}
