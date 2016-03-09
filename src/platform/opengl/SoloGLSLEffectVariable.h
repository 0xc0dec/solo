//#pragma once
//
//#include "SoloBase.h"
//#include "SoloEffectVariable.h"
//#include <GL/glew.h>
//
//namespace solo
//{
//    class GLSLEffectVariable : public EffectVariable
//    {
//    public:
//        virtual void setFloat(float value) override;
//        virtual void setFloatArray(const std::vector<float>& value) override;
//        virtual void setInt(int value) override;
//        virtual void setIntArray(const std::vector<int>& value) override;
//        virtual void setVector2(const Vector2& value) override;
//        virtual void setVector2Array(const std::vector<Vector2>& value) override;
//        virtual void setVector3(const Vector3& value) override;
//        virtual void setVector3Array(const std::vector<Vector3>& value) override;
//        virtual void setVector4(const Vector4& value) override;
//        virtual void setVector4Array(const std::vector<Vector4>& value) override;
//        virtual void setMatrix(const Matrix& value) override;
//        virtual void setMatrixArray(const std::vector<Matrix>& value) override;
//        virtual void setTexture(const shared<Texture> texture) override;
//        virtual void setTextureArray(const std::vector<shared<Texture>>& textures) override;
//
//    private:
//        friend class GLSLEffect;
//
//        GLSLEffectVariable(const std::string& name, GLint location, GLenum type, int index);
//
//        GLint location = 0;
//        GLenum type = 0;
//        int index = 0;
//    };
//}