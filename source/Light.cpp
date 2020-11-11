#include "pch.h"
#include "Light.h"

Elite::Light::Light(const RGBColor& lightColor, float lightIntensity)
	: m_LightColor(lightColor)
	, m_LightIntensity(lightIntensity)
	, m_IsEnabled(true)
{
}
