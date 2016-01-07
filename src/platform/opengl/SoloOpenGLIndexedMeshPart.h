#pragma once

#include "SoloIndexedMeshPart.h"
#include "SoloMesh.h"
#include <GL/glew.h>

namespace solo
{
    class OpenGLIndexedMeshPart: public IndexedMeshPart
    {
    public:
        virtual ~OpenGLIndexedMeshPart();

        virtual void resetIndexData(MeshIndexFormat indexFormat, float* data, unsigned elementCount, bool dynamic) override;
        virtual void updateIndexData(float *data, unsigned elementCount, unsigned updateFromIndex) override;

        virtual unsigned getElementCount() const override;
        virtual MeshIndexFormat getIndexFormat() const override;

        GLuint getBufferHandle() const;

    private:
        friend class OpenGLMesh2;

        OpenGLIndexedMeshPart();

        static unsigned getElementSize(MeshIndexFormat indexFormat);
        static GLenum convertPrimitiveType(PrimitiveType primitiveType);

        GLuint bufferHandle = 0;
        MeshIndexFormat indexFormat = MeshIndexFormat::UnsignedShort;
        unsigned bufferElementCount = 0;
        unsigned elementSize = 0;
    };

    inline unsigned OpenGLIndexedMeshPart::getElementCount() const
    {
        return bufferElementCount;
    }

    inline MeshIndexFormat OpenGLIndexedMeshPart::getIndexFormat() const
    {
        return indexFormat;
    }

    inline GLuint OpenGLIndexedMeshPart::getBufferHandle() const
    {
        return bufferHandle;
    }
}