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
        vec4 c = texture(mainTex, uv0);
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

    sptr<Texture2D> renderTextToTexture(const std::string& text, uint32_t textureWidth, uint32_t textureHeight, uint32_t lineHeight)
    {
        auto fontBytes = Device::get()->getFileSystem()->readBytes("c:/windows/fonts/arialbd.ttf");

        stbtt_fontinfo font;
        stbtt_InitFont(&font, fontBytes.data(), 0);

        auto scale = stbtt_ScaleForPixelHeight(&font, lineHeight);

        int ascent, descent, lineGap, x = 0;
        stbtt_GetFontVMetrics(&font, &ascent, &descent, &lineGap);

        ascent *= scale;
        descent *= scale;

        auto pixels = std::make_unique<uint8_t[]>(textureWidth * textureHeight);

        for (int i = 0; i < text.size(); ++i)
        {
            /* get bounding box for character (may be offset to account for chars that dip above or below the line */
            int c_x1, c_y1, c_x2, c_y2;
            stbtt_GetCodepointBitmapBox(&font, text[i], scale, scale, &c_x1, &c_y1, &c_x2, &c_y2);

            /* compute y (different characters have different heights */
            int y = ascent + c_y1;

            /* render character (stride and offset is important here) */
            int byteOffset = x + (y  * textureWidth);
            stbtt_MakeCodepointBitmap(&font, pixels.get() + byteOffset, c_x2 - c_x1, c_y2 - c_y1, textureWidth, scale, scale, text[i]);

            /* how wide is this character */
            int ax;
            stbtt_GetCodepointHMetrics(&font, text[i], &ax, nullptr);
            x += ax * scale;

            /* add kerning */
            int kern;
            kern = stbtt_GetCodepointKernAdvance(&font, text[i], text[i + 1]);
            x += kern * scale;
        }

        auto fontTexture = Texture2D::create();
        fontTexture->setFiltering(TextureFiltering::Nearest);
        fontTexture->setData(TextureFormat::Red, pixels.get(), textureWidth, textureHeight);
        
        return fontTexture;
    }

    uptr<stbtt_packedchar[]> charInfo;
    uint32_t firstChar;

    sptr<Texture2D> renderFontAtlas(uint32_t firstChar, uint32_t charCount, uint32_t textureWidth, uint32_t textureHeight, float fontSize)
    {
        stbtt_pack_context context;

        auto pixels = std::make_unique<uint8_t[]>(textureWidth * textureHeight);
        stbtt_PackBegin(&context, pixels.get(), textureWidth, textureHeight, 0, 1, nullptr);
        stbtt_PackSetOversampling(&context, 2, 2);

        charInfo = std::make_unique<stbtt_packedchar[]>(charCount);

        auto fontBytes = Device::get()->getFileSystem()->readBytes("c:/windows/fonts/arialbd.ttf");
        stbtt_PackFontRange(&context, fontBytes.data(), 0, fontSize, firstChar, charCount, charInfo.get());
        stbtt_PackEnd(&context);

        auto fontTexture = Texture2D::create();
        fontTexture->setFiltering(TextureFiltering::Linear);
        fontTexture->setData(TextureFormat::Red, pixels.get(), textureWidth, textureHeight);

        this->firstChar = firstChar;

        return fontTexture;
    }

    sptr<Mesh> createFontMesh(const std::string& text, uint32_t textureWidth, uint32_t textureHeight)
    {
        float xpos = 0, ypos = 0;
        stbtt_aligned_quad quad;

        std::vector<Vector3> vertices;
        std::vector<Vector2> uvs;
        std::vector<uint16_t> indexes;

        uint16_t lastIndex = 0;
        for (auto c: text)
        {
            stbtt_GetPackedQuad(charInfo.get(), textureWidth, textureHeight, c - firstChar, &xpos, &ypos, &quad, 0);
            auto xmin = quad.x0;
            auto xmax = quad.x0 + (quad.x1 - quad.x0) * 1;
            auto ymin = quad.y1;
            auto ymax = quad.y1 + (quad.y0 - quad.y1) * 1;

            vertices.emplace_back(xmin, ymax, 0);
            vertices.emplace_back(xmin, ymin, 0);
            vertices.emplace_back(xmax, ymin, 0);
            vertices.emplace_back(xmax, ymax, 0);
            uvs.emplace_back(quad.s0, quad.t1);
            uvs.emplace_back(quad.s0, quad.t0);
            uvs.emplace_back(quad.s1, quad.t0);
            uvs.emplace_back(quad.s1, quad.t1);
            indexes.push_back(lastIndex);
            indexes.push_back(lastIndex + 1);
            indexes.push_back(lastIndex + 2);
            indexes.push_back(lastIndex);
            indexes.push_back(lastIndex + 2);
            indexes.push_back(lastIndex + 3);

            lastIndex += 4;
        }

        VertexBufferLayout layout1, layout2;
        layout1.add(VertexBufferLayoutSemantics::Position, 3);
        layout2.add(VertexBufferLayoutSemantics::TexCoord0, 2);

        auto mesh = Mesh::create();
        mesh->addVertexBuffer(layout1, reinterpret_cast<const float*>(vertices.data()), static_cast<uint32_t>(vertices.size()));
        mesh->addVertexBuffer(layout2, reinterpret_cast<const float*>(uvs.data()), static_cast<uint32_t>(uvs.size()));
        mesh->addPart(reinterpret_cast<const void*>(indexes.data()), static_cast<uint32_t>(indexes.size()));
        mesh->setPrimitiveType(PrimitiveType::Triangles);

        return mesh;
    }

    void initFontQuad()
    {
        const int textureWidth = 1024;
        const int textureHeight = 1024;
        const int lineHeight = 60;
//        auto fontTexture = renderTextToTexture("Hello, world!", textureWidth, textureHeight, lineHeight);
        auto fontTexture = renderFontAtlas('a', 26, textureWidth, textureHeight, lineHeight);
        auto mesh = createFontMesh("test", textureWidth, textureHeight);

//        auto mesh = Mesh::create(MeshPrefab::Quad);
        auto effect = Effect::create(commonShaders.vertex.basic, fsFont);
        auto mat = Material::create(effect);
        mat->setPolygonFace(PolygonFace::All);
        mat->setParameterAutoBinding("worldViewProjMatrix", AutoBinding::WorldViewProjectionMatrix);
        mat->setTextureParameter("mainTex", fontTexture);
        mat->setTransparent(true);

        auto node = scene->createNode();
        auto renderer = node->addComponent<MeshRenderer>();
        renderer->setMesh(mesh);
        renderer->setMaterial(0, mat);
        auto t = node->getComponent<Transform>();
        t->setLocalPosition(Vector3(0, 5, 0));
        t->setLocalScale(Vector3(0.05, 0.05, 1));
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
