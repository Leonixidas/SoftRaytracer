#include "pch.h"
#include "DirectionalLight.h"
#include "RayTracerStructs.h"

Elite::DirectionalLight::DirectionalLight(const FVector3& lightDirection, const RGBColor& lightColor, float lightIntensity)
	: Light(lightColor, lightIntensity)
	, m_LightDirection(lightDirection)
{
}

Elite::RGBColor Elite::DirectionalLight::CalculateLight(const HitRecord& hit, FVector3& wi, float& pdf)
{
	return m_LightColor * m_LightIntensity;
}

void Elite::DirectionalLight::GetLightDirection(HitRecord& hit)
{
	hit.m_IncomingLightDirection = m_LightDirection;
}
