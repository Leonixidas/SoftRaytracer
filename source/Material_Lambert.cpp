#include "pch.h"
#include "Material_Lambert.h"
#include "BRDF.h"

Elite::Material_Lambert::Material_Lambert(const RGBColor& diffuseCcolor, float diffuseReflectance)
	: Material(diffuseCcolor)
	, m_DiffuseReflectance(diffuseReflectance)
{
}

Elite::RGBColor Elite::Material_Lambert::Shade(const HitRecord& hitRecord, const FVector3& w0, const FVector3& w1) const
{
	return BRDF::Lambert(m_DiffuseReflectance, m_DiffuseColor);
}
