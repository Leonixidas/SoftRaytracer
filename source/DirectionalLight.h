#pragma once
#include "Light.h"

namespace Elite
{
	class DirectionalLight : public Light
	{
	public:;
		DirectionalLight(const FVector3& lightDirection, const RGBColor& lightColor = RGBColor{ 1.f,1.f,1.f }, float lightIntensity = 1.f);
		~DirectionalLight() = default;

		virtual RGBColor CalculateLight(const HitRecord& hit, FVector3& wi, float& pdf) override;
		virtual void GetLightDirection(HitRecord& hit) override;

	private:
		FVector3 m_LightDirection;
	};
}

