#pragma once
#include "Geometry.h"
#include <vector>

namespace Elite
{
	class Rectangle : public Geometry
	{
	public:
		Rectangle(Material* pMat, const Transform& transform = {}, bool isLight = false);
		virtual ~Rectangle() = default;

		virtual bool Hit(const Ray& ray, HitRecord& hit) const override;
		virtual FPoint3 GetRandomSurfacePoint() const override;

	private:
		std::vector<FVector3> m_Vertices;
		std::vector<uint32_t> m_Indices;
	};
}