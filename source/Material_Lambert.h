#pragma once
#include "Material.h"

namespace Elite
{
	class Material_Lambert : public Material
	{
	public:
		Material_Lambert(const RGBColor& diffuseCcolor = RGBColor{ 1.f,0.f,0.f}, float diffuseReflectance = 1.f);
		~Material_Lambert() = default;

		RGBColor Shade(const HitRecord& hitRecord, const FVector3& w0, const FVector3& w1) const override;

	private:
		float m_DiffuseReflectance;
	};
}

