#include "pch.h"
#include "EmissiveMaterial.h"

Elite::EmissiveMaterial::EmissiveMaterial(const RGBColor& emissiveColor)
	: Material(emissiveColor)
{
}

Elite::RGBColor Elite::EmissiveMaterial::Shade(const HitRecord& hitRecord, const FVector3& w0, const FVector3& w1) const
{
	return m_DiffuseColor;
}
