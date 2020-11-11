#pragma once
#include "Material.h"

namespace Elite
{
	class BasicMaterial : public Material
	{
	public:
		BasicMaterial(const RGBColor& color = RGBColor{ 0.77f,0.77f,0.77f });
		~BasicMaterial() = default;

		RGBColor Shade(const HitRecord& hitRecord, const FVector3& w0, const FVector3& w1)const override;
	};
}

