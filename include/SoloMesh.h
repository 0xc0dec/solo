#pragma once

#include "SoloBase.h"
#include "SoloVector3.h"
#include "SoloVector2.h"
#include "SoloVertexFormat.h"

namespace solo
{
    enum class DeviceMode;

	enum class PrimitiveType
	{
		Triangles,
		TriangleStrip,
		Lines,
		LineStrip,
		Points
	};

	class Mesh2
	{
	public:
		static shared<Mesh2> create(DeviceMode mode, const VertexFormat &vertexFormat, bool dynamic);

		Mesh2(const Mesh2 &other) = delete;
		Mesh2(Mesh2 &&other) = delete;
		Mesh2 &operator=(const Mesh2 &other) = delete;
		Mesh2 &operator=(Mesh2 &&other) = delete;
		virtual ~Mesh2() {}

		virtual void setVertexData(float *data, unsigned vertexCount, unsigned firstVertexIndex) = 0;

		void setPrimitiveType(PrimitiveType primitiveType);

	protected:
		Mesh2(const VertexFormat &vertexFormat, bool dynamic);

		VertexFormat vertexFormat;
		bool dynamic = false;
		PrimitiveType primitiveType = PrimitiveType::Triangles;
	};

	inline void Mesh2::setPrimitiveType(PrimitiveType primitiveType)
	{
		this->primitiveType = primitiveType;
	}


    class Mesh
    {
    public:
        static shared<Mesh> create(DeviceMode mode);

        Mesh(const Mesh &other) = delete;
        Mesh(Mesh &&other) = delete;
        Mesh &operator=(const Mesh &other) = delete;
        Mesh &operator=(Mesh &&other) = delete;
        virtual ~Mesh() {}

        virtual void draw() = 0;

        virtual void setVertices(const std::vector<Vector3> &vertices) = 0;
        virtual void setNormals(const std::vector<Vector3> &normals) = 0;
        virtual void setUVs(const std::vector<Vector2> &uvs) = 0;
        virtual void setIndices(const std::vector<unsigned short> &indices) = 0;

        void rebuildAsQuad();
        void rebuildAsBox();
        // TODO other types of primitives

    protected:
        Mesh() {}
    };
}