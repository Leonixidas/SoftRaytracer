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

	RGBColor RectLight::CalculateLight(const FPoint3& pointToShade)
	{
		return RGBColor{};
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

	RGBColor DiskLight::CalculateLight(const FPoint3& pointToShade)
	{
		return RGBColor();
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

	Elite::RGBColor SphereLight::CalculateLight(const FPoint3& pointToShade)
	{
		return RGBColor();
	}

	void SphereLight::GetLightDirection(HitRecord& hit)
	{

	}

}
