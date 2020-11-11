#pragma once
#include "EMath.h"
#include "ERGBColor.h"
#include "Light.h"

namespace Elite
{
	class PointLight : public Light
	{
	public:
		PointLight(FPoint3 pos, RGBColor color = RGBColor{ 1.f,1.f,1.f }, float lightIntensity = 10);
		~PointLight() = default;
		virtual RGBColor CalculateLight(const FPoint3& pointToShade) override;
		virtual void GetLightDirection(HitRecord& hit) override;
		const FPoint3& GetLightPosition() { return m_Position; }

	private:
		FPoint3 m_Position;
	};
}

