//#pragma once
//
//#include "SoloEffect.h"
//#include <GL/glew.h>
//
//namespace solo
//{
//    class GLSLEffect: public Effect
//    {
//    public:
//        virtual ~GLSLEffect();
//
//        virtual void bind() override;
//        virtual void unbind() override;
//
//    private:
//        friend class Effect;
//
//        GLSLEffect(const std::string& vsSrc, const std::string& fsSrc);
//
//        void deleteShader(GLuint vs);
//        void discoverVariables();
//        void discoverVertexAttributes();
//
//        GLuint program { 0 };
//    };
//}
