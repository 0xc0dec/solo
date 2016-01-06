#pragma once

#include "SoloBase.h"

namespace solo
{
    enum class DeviceMode;

    class EffectVertexAttribute
    {
    public:
        EffectVertexAttribute(const EffectVertexAttribute &other) = delete;
        EffectVertexAttribute(EffectVertexAttribute &&other) = delete;
        EffectVertexAttribute &operator=(const EffectVertexAttribute &other) = delete;
        EffectVertexAttribute &operator=(EffectVertexAttribute &&other) = delete;
        virtual ~EffectVertexAttribute() {}

		std::string getName() const;

    protected:
		EffectVertexAttribute(const std::string &name): name(name) {}

	private:
		std::string name;
    };

	inline std::string EffectVertexAttribute::getName() const
	{
		return name;
	}
}
