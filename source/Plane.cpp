#include "pch.h"
#include "Plane.h"

Elite::Plane::Plane(FPoint3 pos, FVector3 normal, Material* pMat)
	: Geometry(pos, pMat)
	, m_Normal(normal)
{
}

bool Elite::Plane::Hit(const Ray& ray, HitRecord& hit) const
{
	FVector3 toPlane{ m_Position - ray.m_Origin };
	float dotToPlane = Dot(toPlane, m_Normal);
	float dotToPoint = Dot(ray.m_Direction, m_Normal);
	float tValue = dotToPlane / dotToPoint;

	if (tValue > hit.m_TValue) return false;

	if (ray.m_Min <= tValue && ray.m_Max > tValue)
	{
		hit.m_TValue = tValue;
		hit.m_HitPoint = ray.m_Origin + hit.m_TValue * ray.m_Direction;
		hit.m_pMaterial = m_pMat;
		hit.m_Normal = m_Normal;
		hit.m_HitSomething = true;
		return true;
	}

	return false;
}
