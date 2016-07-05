#pragma once


namespace solo
{
    struct OpenGLPrefabShaders
    {
        struct Vertex
        {
            static const char* skybox;
            static const char* simple;
        };

        struct Fragment
        {
            static const char* skybox;
            static const char* font;
        };
    };
}