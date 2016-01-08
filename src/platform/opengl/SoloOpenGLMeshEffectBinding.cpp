#include "SoloOpenGLMeshEffectBinding.h"
#include "SoloEffect.h"
#include "SoloMesh.h"
#include "SoloGLSLEffectVertexAttribute.h"

using namespace solo;


OpenGLMeshEffectBinding::OpenGLMeshEffectBinding(Mesh *mesh, Effect *effect)
{
    glGenVertexArrays(1, &vertexArrayHandle);
    if (!vertexArrayHandle)
        SL_THROW_FMT(EngineException, "Unable to obtain vertex array handle");

    glBindVertexArray(vertexArrayHandle);

    auto vertexFormat = mesh->getVertexFormat();
    size_t offset = 0;
    for (auto i = 0; i < vertexFormat.getElementCount(); ++i)
    {
        auto element = vertexFormat.getElement(i);
        EffectVertexAttribute *attr = nullptr;
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
            auto name = "texCoord" + (static_cast<int>(element.semantics) - static_cast<int>(VertexFormatElementSemantics::TexCoord0));
            attr = effect->findVertexAttribute(name);
            break;
        }
        default:
            break;
        }

        if (attr)
        {
            auto glAttr = static_cast<GLSLEffectVertexAttribute*>(attr);
            glVertexAttribPointer(glAttr->getLocation(), element.size, GL_FLOAT, GL_FALSE,
                static_cast<GLsizei>(vertexFormat.getVertexSize()), reinterpret_cast<void*>(offset));
            glEnableVertexAttribArray(glAttr->getLocation());
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
