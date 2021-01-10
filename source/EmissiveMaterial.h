#pragma once
#include "Material.h"

namespace Elite
{
	class EmissiveMaterial : public Material
	{
	public:
		EmissiveMaterial(const RGBColor& emissiveColor = RGBColor(1.f, 1.f, 1.f));
		~EmissiveMaterial() = default;

		RGBColor Shade(const HitRecord& hitRecord, const FVector3& w0, const FVector3& w1)const override;
	};
}

