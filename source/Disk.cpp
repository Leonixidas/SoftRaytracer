#include "pch.h"
#include "Disk.h"

namespace Elite
{
	Disk::Disk(Material* pMat, const Transform& transform, float radius, bool isLight)
		: Geometry(pMat, transform, isLight)
		, m_Radius(radius)
	{

	}

	bool Disk::Hit(const Ray& ray, HitRecord& hit) const
	{
		FVector3 toPlane{ m_Transform.GetPosition() - ray.m_Origin };
		FVector3 normal = m_Transform.GetUpVector();
		float dotToPlane = Dot(toPlane, normal);
		float dotToPoint = Dot(ray.m_Direction, normal);
		float tValue = dotToPlane / dotToPoint;

		if (tValue > hit.m_TValue) return false;
		FPoint3 p0 = ray.m_Origin + tValue * ray.m_Direction;
		FVector3 v = m_Transform.GetPosition() - p0;
		if (Dot(v, v) > m_Radius * m_Radius) return false;

		if (tValue > ray.m_Min && tValue < ray.m_Max)
		{
			hit.m_TValue = tValue;
			hit.m_HitPoint = p0;
			hit.m_Normal = normal;
			hit.m_pMaterial = m_pMat;
			hit.m_HitSomething = true;
			return true;
		}

		return false;
	}
}