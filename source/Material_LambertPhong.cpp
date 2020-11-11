#include "pch.h"
#include "Material_LambertPhong.h"
#include "BRDF.h"

Elite::Material_LambertPhong::Material_LambertPhong(const float diffuseReflectance, const RGBColor& diffuseColor, float specularReflectance, float phongExponent)
	: Material(diffuseColor)
	, m_DiffuseReflectance(diffuseReflectance)
	, m_SpecularReflectance(specularReflectance)
	, m_PhongExponent(phongExponent)
{
}

Elite::RGBColor Elite::Material_LambertPhong::Shade(const HitRecord& hitRecord, const FVector3& w0, const FVector3& w1) const
{
	return BRDF::Lambert(m_DiffuseReflectance, m_DiffuseColor) + BRDF::Phong(m_SpecularReflectance,m_PhongExponent, w0, w1, hitRecord.m_Normal);
}
