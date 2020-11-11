#include "pch.h"
#include "BasicMaterial.h"

Elite::BasicMaterial::BasicMaterial(const RGBColor& color)
	: Material(color)
{
}

Elite::RGBColor Elite::BasicMaterial::Shade(const HitRecord& hitRecord, const FVector3& w0, const FVector3& w1) const
{
	return m_DiffuseColor;
}
