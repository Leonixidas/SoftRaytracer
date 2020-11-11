#pragma once
#include "RayTracerStructs.h"

namespace Elite
{
	class Material
	{
	public:
		explicit Material(const RGBColor& color);
		virtual ~Material() = default;

		virtual RGBColor Shade(const HitRecord& hitRecord, const FVector3& w0, const FVector3& w1) const = 0;

		const RGBColor& GetMaterialColor() { return m_DiffuseColor; }

	protected:
		RGBColor m_DiffuseColor;
	};
}

