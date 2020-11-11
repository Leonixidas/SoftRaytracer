#pragma once
#include "Material.h"

namespace Elite
{
	class Material_LambertPhong : public Material
	{
	public:

		Material_LambertPhong(const float diffuseReflectance, const RGBColor& diffuseColor, const float specularReflectance, const float phongExponent);

		~Material_LambertPhong() = default;

		RGBColor Shade(const HitRecord& hitRecord, const FVector3& w0, const FVector3& w1) const override;

	private:
		float m_DiffuseReflectance, m_SpecularReflectance, m_PhongExponent;
	};
}

