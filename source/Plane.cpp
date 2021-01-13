#include "pch.h"
#include "Plane.h"

Elite::Plane::Plane(Material* pMat, const Transform& transform)
	: Geometry(pMat, transform)
{
}

bool Elite::Plane::Hit(const Ray& ray, HitRecord& hit) const
{
	FVector3 toPlane{ m_Transform.GetPosition() - ray.m_Origin };
	FVector3 normal = m_Transform.GetUpVector();
	float dotToPlane = Dot(toPlane, normal);
	float dotToPoint = Dot(ray.m_Direction, normal);
	float tValue = dotToPlane / dotToPoint;

	if (tValue > hit.m_TValue) return false;

	if (ray.m_Min <= tValue && ray.m_Max > tValue)
	{
		hit.m_TValue = tValue;
		hit.m_HitPoint = ray.m_Origin + hit.m_TValue * ray.m_Direction;
		hit.m_pMaterial = m_pMat;
		hit.m_Normal = normal;
		hit.m_HitSomething = true;
		hit.m_IsLight = m_IsLight;
		return true;
	}

	return false;
}
