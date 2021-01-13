#pragma once
#include "Geometry.h"

namespace Elite
{
	class Disk : public Geometry
	{
	public:
		Disk(Material* pMat, const Transform& transform = {}, float radius = 1.f, bool isLight = false);
		virtual ~Disk() = default;

		virtual bool Hit(const Ray& ray, HitRecord& hit) const override;
		virtual FPoint3 GetRandomSurfacePoint() const override;

	protected:
		float m_Radius;
	};

	

}