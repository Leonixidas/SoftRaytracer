#include "pch.h"
#include "AreaLight.h"
#include "RayTracerStructs.h"

namespace Elite
{
	//-----------------------------------------------//
	//               RECTANGLE LIGHT                 //
	//-----------------------------------------------//

	RectLight::RectLight(Material* mat, const Transform& transform, float intensity)
		: Light(mat->GetMaterialColor(), intensity)
		, Rectangle(mat, transform, true)
	{
	}

	RGBColor RectLight::CalculateLight(const HitRecord& hit, FVector3& wi, float& pdf)
	{
		FPoint3 randomPoint = GetRandomSurfacePoint();
		wi = randomPoint - hit.m_HitPoint;
		float distanceSqrd = SqrMagnitude(wi);
		wi = GetNormalized(wi);

		FPoint3 scale = m_Transform.GetScale();
		float lightArea = scale.x * scale.z;

		float angle = Dot(-wi, m_Transform.GetUpVector());
		pdf =  distanceSqrd / (angle * lightArea);
	
		return angle > 0.f ? m_pMat->GetMaterialColor() * lightArea * float(E_PI) : RGBColor();
	}

	void RectLight::GetLightDirection(HitRecord& hit)
	{
		hit.m_IncomingLightDirection = m_Transform.GetUpVector();
	}

	//------------------------------------------//
	//               DISK LIGHT                 //
	//------------------------------------------//

	DiskLight::DiskLight(Material* mat, const Transform& transform, float radius, float intensity /*= 1.f*/)
		: Light(mat->GetMaterialColor(), intensity)
		, Disk(mat, transform, radius, true)
	{
	}

	RGBColor DiskLight::CalculateLight(const HitRecord& hit, FVector3& wi, float& pdf)
	{
		FPoint3 randomPoint = GetRandomSurfacePoint();
		wi = randomPoint - hit.m_HitPoint;
		float distanceSqrd = SqrMagnitude(wi);
		wi = GetNormalized(wi);
		
		float lightArea = (float)E_PI * (m_Radius * m_Radius);
		float angle = Dot(-wi, m_Transform.GetUpVector());
		pdf = distanceSqrd / (angle * lightArea);

		return angle > 0.f ? m_pMat->GetMaterialColor() * lightArea * float(E_PI): RGBColor();
	}

	void DiskLight::GetLightDirection(HitRecord& hit)
	{
	}

	//--------------------------------------------//
	//               SPHERE LIGHT                 //
	//--------------------------------------------//

	SphereLight::SphereLight(Material* mat, const Transform& transform, float intensity, float radius)
		: Light(mat->GetMaterialColor(), intensity)
		, Sphere(mat, transform, radius, true)
	{

	}

	Elite::RGBColor SphereLight::CalculateLight(const HitRecord& hit, FVector3& wi, float& pdf)
	{
		return RGBColor();
	}

	void SphereLight::GetLightDirection(HitRecord& hit)
	{

	}

}
