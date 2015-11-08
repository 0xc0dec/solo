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

	class Material
	{
	public:
		Material(const Material& other) = delete;
		Material(Material&& device) = delete;
		Material& operator=(const Material& other) = delete;
		Material& operator=(Material&& other) = delete;
		virtual ~Material() {}

		void bind(RenderContext& context);
		void unbind(RenderContext& context);

		MaterialParameter* getParameter(const std::string &name);
		Effect* getEffect() const;

		PolygonFace getPolygonFace() const;
		void setPolygonFace(PolygonFace face);

		bool getZWriteEnabled() const;
		void setZWriteEnabled(bool enabled);

	protected:
		explicit Material(shared<Effect> effect);

		virtual void applyFaceCull() = 0;
		virtual void applyZWrite() = 0;

		PolygonFace polygonFace = PolygonFace::CW;
		bool zwrite = true;

	private:
		shared<Effect> effect;
		std::unordered_map<std::string, shared<MaterialParameter>> parameters;
	};

	inline Effect* Material::getEffect() const
	{
		return effect.get();
	}

	inline void Material::setPolygonFace(PolygonFace face)
	{
		polygonFace = face;
	}

	inline PolygonFace Material::getPolygonFace() const
	{
		return polygonFace;
	}

	inline bool Material::getZWriteEnabled() const
	{
		return zwrite;
	}

	inline void Material::setZWriteEnabled(bool enabled)
	{
		zwrite = enabled;
	}

	class MaterialFactory
	{
		friend class ResourceManager;
		static shared<Material> create(DeviceMode mode, shared<Effect> effect);
	};
}
