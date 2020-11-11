#pragma once
#include "Geometry.h"

namespace Elite
{
	class Plane final : public Geometry
	{
	public:
		Plane(FPoint3 pos, FVector3 normal, Material* pMat);
		~Plane() = default;

		virtual bool Hit(const Ray& ray, HitRecord& hit) const override;

	private:
		FVector3 m_Normal;
	};
}

