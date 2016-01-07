#include "SoloOpenGLMeshEffectBinding.h"
#include "SoloEffect.h"
#include "SoloMesh.h"
#include "SoloGLSLEffectVertexAttribute.h"

using namespace solo;


OpenGLMeshEffectBinding::OpenGLMeshEffectBinding(Mesh2 *mesh, Effect *effect)
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
        case VertexFormatElement::Semantics::Position:
            attr = effect->findVertexAttribute("position");
            break;
        case VertexFormatElement::Semantics::Normal:
            attr = effect->findVertexAttribute("normal");
            break;
        case VertexFormatElement::Semantics::Color:
            attr = effect->findVertexAttribute("color");
            break;
        case VertexFormatElement::Semantics::Tangent:
            attr = effect->findVertexAttribute("tangent");
            break;
        case VertexFormatElement::Semantics::Binormal:
            attr = effect->findVertexAttribute("binormal");
            break;
        case VertexFormatElement::Semantics::TexCoord0:
        case VertexFormatElement::Semantics::TexCoord1:
        case VertexFormatElement::Semantics::TexCoord2:
        case VertexFormatElement::Semantics::TexCoord3:
        case VertexFormatElement::Semantics::TexCoord4:
        case VertexFormatElement::Semantics::TexCoord5:
        case VertexFormatElement::Semantics::TexCoord6:
        case VertexFormatElement::Semantics::TexCoord7:
        {
            auto name = "texCoord" + (static_cast<int>(element.semantics) - static_cast<int>(VertexFormatElement::Semantics::TexCoord0));
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
