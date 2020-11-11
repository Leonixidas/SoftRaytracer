#include "pch.h"
#include "Material_PBR.h"
#include "BRDF.h"

Elite::Material_PBR::Material_PBR(const RGBColor& diffuseColor, const float specularReflectance, const RGBColor& albedo, const bool metalness, const float roughness)
	: Material(diffuseColor)
	, m_Albedo(albedo)
	, m_Metalness(metalness)
	, m_Roughness(roughness)
	, m_SpecularReflectance(specularReflectance)
{
}

Elite::RGBColor Elite::Material_PBR::Shade(const HitRecord& hitRecord, const FVector3& w0, const FVector3& w1) const
{
	RGBColor finalColor{};
	FVector3 halfVector = w0 + w1;
	halfVector /= Magnitude(halfVector);

	float a = m_Roughness * m_Roughness;

	RGBColor fresnel = BRDF::SchlickFresnel(halfVector, w0, m_Metalness ? RGBColor{ 0.4f,0.4f,0.4f } : m_Albedo);
	float D = BRDF::TrowbridgeReitz_GGX(hitRecord.m_Normal, halfVector, a);
	a += 1;
	float G = BRDF::Smith_GGX(hitRecord.m_Normal, w0, w1, (a * a) / 8);

	finalColor = BRDF::CookTorrence(D, fresnel, G, hitRecord.m_Normal, w1, w0) * m_SpecularReflectance;

	RGBColor kd = m_Metalness ? RGBColor{} : RGBColor{ 1.f,1.f,1.f } - fresnel;

	finalColor += BRDF::Lambert(kd, m_DiffuseColor);
	
	return finalColor;
}
