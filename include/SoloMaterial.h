#pragma once

#include "SoloBase.h"

namespace solo
{
    class Effect;
    class MaterialParameter;
    struct RenderContext;
    enum class DeviceMode;

    enum class PolygonFace
    {
        CW,
        CCW,
        All
    };

    enum class DepthPassFunction
    {
        Never,
        Less,
        Equal,
        LEqual,
        Greater,
        NotEqual,
        GEqual,
        Always
    };

    class Material
    {
    public:
        static shared<Material> create(DeviceMode mode, shared<Effect> effect);

        Material(const Material &other) = delete;
        Material(Material &&device) = delete;
        Material &operator=(const Material &other) = delete;
        Material &operator=(Material &&other) = delete;
        virtual ~Material() {}

        void bind(RenderContext &context);
        void unbind(RenderContext &context);

        MaterialParameter *getParameter(const std::string &name);
        Effect *getEffect() const;

        PolygonFace getPolygonFace() const;
        void setPolygonFace(PolygonFace face);

        bool isDepthWriteEnabled() const;
        void setDepthWriteEnabled(bool enabled);

        bool isDepthTestEnabled() const;
        void setDepthTestEnabled(bool enabled);

        DepthPassFunction getDepthPassFunction() const;
        void setDepthPassFunction(DepthPassFunction func);

    protected:
        explicit Material(shared<Effect> effect);

        virtual void applyState() = 0;

        PolygonFace polygonFace = PolygonFace::CW;
        bool depthWrite = true;
        bool depthTest = true;
        DepthPassFunction depthPassFunc = DepthPassFunction::Less;

    private:
        shared<Effect> effect;
        std::unordered_map<std::string, shared<MaterialParameter>> parameters;
    };

    inline Effect *Material::getEffect() const
    {
        return effect.get();
    }

    inline PolygonFace Material::getPolygonFace() const
    {
        return polygonFace;
    }

    inline void Material::setPolygonFace(PolygonFace face)
    {
        polygonFace = face;
    }

    inline bool Material::isDepthWriteEnabled() const
    {
        return depthWrite;
    }

    inline void Material::setDepthWriteEnabled(bool enabled)
    {
        depthWrite = enabled;
    }

    inline bool Material::isDepthTestEnabled() const
    {
        return depthTest;
    }

    inline void Material::setDepthPassFunction(DepthPassFunction func)
    {
        depthPassFunc = func;
    }

    inline DepthPassFunction Material::getDepthPassFunction() const
    {
        return depthPassFunc;
    }

    inline void Material::setDepthTestEnabled(bool enabled)
    {
        depthTest = enabled;
    }
}
