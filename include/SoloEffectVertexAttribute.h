#pragma once

#include "SoloBase.h"


namespace solo
{
    class EffectVertexAttribute
    {
    public:
        SL_NONCOPYABLE(EffectVertexAttribute)
        virtual ~EffectVertexAttribute() {}

        std::string getName() const;

    protected:
        EffectVertexAttribute(const std::string& name): name(name) {}

    private:
        std::string name;
    };

    inline std::string EffectVertexAttribute::getName() const
    {
        return name;
    }
}
