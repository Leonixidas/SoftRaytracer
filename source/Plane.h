#pragma once
#include "Geometry.h"

namespace Elite
{
	class Plane final : public Geometry
	{
	public:
		Plane(Material* pMat, const Transform& transform = {});
		~Plane() = default;

		virtual bool Hit(const Ray& ray, HitRecord& hit) const override;
		virtual FPoint3 GetRandomSurfacePoint() const override { return FPoint3(); }
	};
}

