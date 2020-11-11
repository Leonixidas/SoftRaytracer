#pragma once
#include "Material.h"

namespace Elite
{
	class Material_PBR : public Material
	{
	public:
		Material_PBR(const RGBColor& diffuseColor, const float specularReflectance, const RGBColor& albedo, const bool metalness, const float roughness);

		~Material_PBR() = default;

		virtual RGBColor Shade(const HitRecord& hitRecord, const FVector3& w0, const FVector3& w1) const override;

	private:
		RGBColor m_Albedo;
		float m_Roughness;
		float m_SpecularReflectance;
		bool m_Metalness;
	};
}

