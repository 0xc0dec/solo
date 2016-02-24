#include "SoloOpenGLMeshEffectBinding.h"
#include "SoloEffect.h"
#include "SoloGLSLEffectVertexAttribute.h"
#include "SoloOpenGLMesh.h"

using namespace solo;


OpenGLMeshEffectBinding::OpenGLMeshEffectBinding(Mesh* mesh, Effect* effect)
{
    glGenVertexArrays(1, &vertexArrayHandle);
    if (!vertexArrayHandle)
        SL_THROW_FMT(ResourceException, "Unable to obtain vertex array handle");

    auto oglMesh = static_cast<OpenGLMesh*>(mesh);
    glBindVertexArray(vertexArrayHandle);

    auto vertexFormat = mesh->getVertexFormat();
    std::unordered_map<int, size_t> offsets;
    for (auto i = 0; i < vertexFormat.getElementCount(); ++i)
    {
        auto element = vertexFormat.getElement(i);
        EffectVertexAttribute* attr = nullptr;
        switch (element.semantics)
        {
        case VertexFormatElementSemantics::Position:
            attr = effect->findVertexAttribute("position");
            break;
        case VertexFormatElementSemantics::Normal:
            attr = effect->findVertexAttribute("normal");
            break;
        case VertexFormatElementSemantics::Color:
            attr = effect->findVertexAttribute("color");
            break;
        case VertexFormatElementSemantics::Tangent:
            attr = effect->findVertexAttribute("tangent");
            break;
        case VertexFormatElementSemantics::Binormal:
            attr = effect->findVertexAttribute("binormal");
            break;
        case VertexFormatElementSemantics::TexCoord0:
        case VertexFormatElementSemantics::TexCoord1:
        case VertexFormatElementSemantics::TexCoord2:
        case VertexFormatElementSemantics::TexCoord3:
        case VertexFormatElementSemantics::TexCoord4:
        case VertexFormatElementSemantics::TexCoord5:
        case VertexFormatElementSemantics::TexCoord6:
        case VertexFormatElementSemantics::TexCoord7:
            {
                auto name = "texCoord" + std::to_string(static_cast<int>(element.semantics) - static_cast<int>(VertexFormatElementSemantics::TexCoord0));
                attr = effect->findVertexAttribute(name);
                break;
            }
        default:
            break;
        }

        auto& offset = offsets[element.slot];

        if (attr)
        {
            glBindBuffer(GL_ARRAY_BUFFER, oglMesh->getBufferHandle(element.slot));
            auto glslAttr = static_cast<GLSLEffectVertexAttribute*>(attr);
            auto stride = static_cast<GLsizei>(vertexFormat.getVertexSize(element.slot));
            glVertexAttribPointer(glslAttr->getLocation(), element.size, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(offset));
            glEnableVertexAttribArray(glslAttr->getLocation());
        }

        offset += element.size * sizeof(float);
    }

    glBindVertexArray(0);
}


OpenGLMeshEffectBinding::~OpenGLMeshEffectBinding()
{
    if (vertexArrayHandle)
        glDeleteVertexArrays(1, &vertexArrayHandle);
}


void OpenGLMeshEffectBinding::bind()
{
    glBindVertexArray(vertexArrayHandle);
}


void OpenGLMeshEffectBinding::unbind()
{
    glBindVertexArray(0);
}
