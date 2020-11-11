#pragma once
#include "Geometry.h"

namespace Elite
{
	class Sphere final : public Geometry
	{
	public:
		Sphere(FPoint3 pos, Material* pMat, float radius = 1.f);
		~Sphere() = default;

		virtual bool Hit(const Ray& ray, HitRecord& hit) const override;

	private:
		float m_Radius;
	};
}

