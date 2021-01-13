#pragma once
#include "Geometry.h"

namespace Elite
{
	class Sphere : public Geometry
	{
	public:
		Sphere(Material* pMat, const Transform& transform = {}, float radius = 1.f, bool isLight = false);
		~Sphere() = default;

		virtual bool Hit(const Ray& ray, HitRecord& hit) const override;
		virtual FPoint3 GetRandomSurfacePoint() const override;

	private:
		float m_Radius;
	};
}

