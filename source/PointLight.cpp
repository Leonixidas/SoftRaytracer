#include "pch.h"
#include "PointLight.h"
#include "RayTracerStructs.h"

Elite::PointLight::PointLight(FPoint3 pos, RGBColor color, float lightIntensity)
	: Light(color, lightIntensity)
	, m_Position(pos)
{
}

Elite::RGBColor Elite::PointLight::CalculateLight(const FPoint3& pointToShade)
{
	return (m_LightColor * m_LightIntensity) / SqrMagnitude(m_Position - pointToShade);
}

void Elite::PointLight::GetLightDirection(HitRecord& hit)
{
	hit.m_IncomingLightDirection = (hit.m_HitPoint - m_Position);
}
