#include "pch.h"
#include "Sphere.h"

Elite::Sphere::Sphere(Material* pMat, const Transform& transform, float radius, bool isLight)
	: Geometry(pMat, transform, isLight)
	, m_Radius(radius)
{
}

bool Elite::Sphere::Hit(const Ray& ray, HitRecord& hit) const
{
	FPoint3 pos = m_Transform.GetPosition();
	FVector3 toCenter{ pos - ray.m_Origin };

	float tCA = Dot(toCenter, ray.m_Direction);
	if (tCA < 0.f) return false;

	float odSQ = SqrMagnitude(Reject(toCenter, ray.m_Direction));

	// if it's larger than the radius squared, we know that the ray does not hit the sphere
	if (odSQ > (m_Radius * m_Radius)) return false; 

	float tHC = sqrtf((m_Radius * m_Radius) - odSQ);

	float tValue = tCA - tHC;

	if (tValue > ray.m_Max) return false;

	if (tValue < hit.m_TValue)
	{
		hit.m_TValue = tValue;

		hit.m_HitPoint = ray.m_Origin + tValue * ray.m_Direction;

		hit.m_pMaterial = m_pMat;

		hit.m_Normal = GetNormalized(hit.m_HitPoint - pos);

		hit.m_HitSomething = true;
		hit.m_IsLight = m_IsLight;
	}
	
	return true;
}

Elite::FPoint3 Elite::Sphere::GetRandomSurfacePoint() const
{
	return FPoint3();
}
