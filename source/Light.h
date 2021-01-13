#pragma once
#include "ERGBColor.h"

namespace Elite
{
	struct HitRecord;

	class Light
	{
	public:
		explicit Light(const RGBColor& lightColor, float lightIntensity);
		virtual ~Light() = default;

		virtual RGBColor CalculateLight(const HitRecord& hit, FVector3& wi, float& pdf) = 0;
		virtual void GetLightDirection(HitRecord& hit) = 0;

		void PressLightSwitch() { m_IsEnabled = !m_IsEnabled; }

		bool IsEnabled() { return m_IsEnabled; }

	protected:
		RGBColor m_LightColor;
		float m_LightIntensity;
		bool m_IsEnabled;
	};
}

