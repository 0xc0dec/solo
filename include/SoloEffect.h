#pragma once

#include "SoloBase.h"


namespace solo
{
    class EffectVariable;
    class EffectVertexAttribute;
    enum class DeviceMode;

    enum class EffectPrefab
    {
        Skybox
    };

    class Effect
    {
    public:
        static shared<Effect> create(DeviceMode mode, const std::string& vsSrc, const std::string& fsSrc);
        static shared<Effect> createPrefab(DeviceMode mode, EffectPrefab prefab);

        SL_NONCOPYABLE(Effect)
        virtual ~Effect() {}

        virtual void bind() = 0;
        virtual void unbind() = 0;

        virtual EffectVariable* findVariable(const std::string& name) const;
        virtual EffectVertexAttribute* findVertexAttribute(const std::string& name) const;

    protected:
        Effect() {}

        std::unordered_map<std::string, shared<EffectVariable>> variables;
        std::unordered_map<std::string, shared<EffectVertexAttribute>> vertexAttributes;
    };
}
