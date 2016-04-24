#include "../../include/Solo.h"
#include "../common/EscapeWatcher.h"
#include "../common/Screenshoter.h"
#include "../common/Shaders.h"

#define STB_TRUETYPE_IMPLEMENTATION
#include "../../vendor/stb/truetype/1.11/stb_truetype.h"

using namespace solo;


const char* fsFont = R"(
    #version 330 core

    uniform sampler2D mainTex;

    in vec2 uv0;
	out vec4 fragColor;

    void main()
	{
        vec2 texCoord = vec2(uv0.x, 1 - uv0.y);
        vec4 c = texture(mainTex, texCoord);
		fragColor = vec4(c.r, c.r, c.r, c.r);
	}
)";


class Demo
{
public:
    void run()
    {
        initEngine();
        initCamera();
        initSkybox();
        initMesh();
        initFontQuad();
        device->run();
    }

    void initEngine()
    {
        DeviceCreationArgs args;
        args.mode = DeviceMode::OpenGL;
        args.canvasWidth = 1200;
        args.canvasHeight = 600;
        args.logFilePath = "demo3.log";
        device = Device::init(args);

        scene = device->getScene();
        loader = device->getAssetLoader();
    }

    void initCamera()
    {
        auto node = scene->createNode();
        auto t = node->getComponent<Transform>();
        t->setLocalPosition(Vector3(0, 0, 5));
        auto cam = node->addComponent<Camera>();
        cam->setClearColor(0.0f, 0.6f, 0.6f, 1.0f);
        cam->setNear(0.05f);
        node->addComponent<Spectator>();
        node->addComponent<EscapeWatcher>();
        node->addComponent<Screenshoter>("demo3-screenshot.bmp");
    }

    void initSkybox()
    {
        loader->loadCubeTextureAsync({
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
            auto renderer = node->addComponent<SkyboxRenderer>();
            renderer->setTexture(tex);
        });
    }

    void initFontQuad()
    {
        const int b_w = 512;
        const int b_h = 128;
        const int l_h = 60; 

        auto fontBytes = Device::get()->getFileSystem()->readBytes("c:/windows/fonts/arialbd.ttf");

        stbtt_fontinfo font;
        stbtt_InitFont(&font, fontBytes.data(), 0);

        auto scale = stbtt_ScaleForPixelHeight(&font, l_h);

        int ascent, descent, lineGap, x = 0;
        stbtt_GetFontVMetrics(&font, &ascent, &descent, &lineGap);

        ascent *= scale;
        descent *= scale;

        auto bitmap = std::make_unique<uint8_t[]>(b_w * b_h);

        const std::string phrase = "Hello, world!";
        for (int i = 0; i < phrase.size(); ++i)
        {
            /* get bounding box for character (may be offset to account for chars that dip above or below the line */
            int c_x1, c_y1, c_x2, c_y2;
            stbtt_GetCodepointBitmapBox(&font, phrase[i], scale, scale, &c_x1, &c_y1, &c_x2, &c_y2);

            /* compute y (different characters have different heights */
            int y = ascent + c_y1;

            /* render character (stride and offset is important here) */
            int byteOffset = x + (y  * b_w);
            stbtt_MakeCodepointBitmap(&font, bitmap.get() + byteOffset, c_x2 - c_x1, c_y2 - c_y1, b_w, scale, scale, phrase[i]);

            /* how wide is this character */
            int ax;
            stbtt_GetCodepointHMetrics(&font, phrase[i], &ax, nullptr);
            x += ax * scale;

            /* add kerning */
            int kern;
            kern = stbtt_GetCodepointKernAdvance(&font, phrase[i], phrase[i + 1]);
            x += kern * scale;
        }

        auto fontTexture = Texture2D::create();
        fontTexture->setFiltering(TextureFiltering::Nearest);
        fontTexture->setData(TextureFormat::Red, bitmap.get(), b_w, b_h);

        auto mesh = Mesh::create(MeshPrefab::Quad);
        auto effect = Effect::create(commonShaders.vertex.passThrough, fsFont);
        auto mat = Material::create(effect);
        mat->setPolygonFace(PolygonFace::All);
//        mat->setParameterAutoBinding("worldViewProjMatrix", AutoBinding::WorldViewProjectionMatrix);
        mat->setTextureParameter("mainTex", fontTexture);
        mat->setTransparent(true);

        auto node = scene->createNode();
        auto renderer = node->addComponent<MeshRenderer>();
        renderer->setMesh(mesh);
        renderer->setMaterial(0, mat);
        node->getComponent<Transform>()->setLocalPosition(Vector3(0, 5, 0));
    }

    void initMesh()
    {
        loader->loadTexture2DAsync("../assets/cobblestone.png")->done([=](sptr<Texture2D> tex)
        {
            tex->setWrapping(TextureWrapping::Clamp);
            tex->generateMipmaps();
            auto effect = Effect::create(commonShaders.vertex.basic, commonShaders.fragment.texture);
            auto mat = Material::create(effect);
            mat->setPolygonFace(PolygonFace::All);
            mat->setParameterAutoBinding("worldViewProjMatrix", AutoBinding::WorldViewProjectionMatrix);
            mat->setTextureParameter("mainTex", tex);

            loader->loadMeshAsync("../assets/monkey.obj")->done([=](sptr<Mesh> mesh)
            {
                auto node = scene->createNode();
                auto renderer = node->addComponent<MeshRenderer>();
                renderer->setMesh(mesh);
                renderer->setMaterial(0, mat);
                node->getComponent<Transform>()->setLocalPosition(Vector3::zero());
            });
        });
    }

private:
    Scene* scene = nullptr;
    AssetLoader* loader = nullptr;
    Device* device = nullptr;
};


int main()
{
    Demo().run();
    return 0;
}
