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
		pdf = distanceSqrd / (angle * lightArea);
	
		return angle > 0.f ? m_pMat->GetMaterialColor() * lightArea * float(E_PI): RGBColor();
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
		FPoint3 center = m_Transform.GetPosition();
		FVector3 toCenter = center - hit.m_HitPoint;
		float dist = Normalize(toCenter);
		float q = sqrtf(1 - (m_Radius / dist) * (m_Radius / dist));

		FVector3 r{}, u{};

		r = GetNormalized(Cross(FVector3{ 0.f,1.f,0.f }, toCenter));
		u = GetNormalized(Cross(toCenter, r));

		FMatrix3 transform{r,u,toCenter};

		float r0 = RandomBinomial(1.f);
		float r1 = RandomBinomial(1.f);

		float theta = acosf((1 - r0) + r0 * q);
		float phi = (2 * (float)E_PI) * r1;

		FVector3 local{};
		local.x = sinf(phi) * cosf(theta);
		local.y = (sinf(phi) * sinf(theta));
		local.z = cosf(phi);

		FVector3 nwp = transform * local;
		Ray ray{};
		ray.m_Direction = nwp;
		ray.m_Origin = hit.m_HitPoint;

		HitRecord nHit{};
		Hit(ray, nHit);
		
		wi = toCenter;
		
		float angle = Dot(nwp, GetNormalized(toCenter));

		pdf = (1.f/((2 * (float)E_PI) * (1-q)));

		return angle > 0.f ? m_pMat->GetMaterialColor() * 4 * float(E_PI) * float(E_PI): RGBColor();
	}

	void SphereLight::GetLightDirection(HitRecord& hit)
	{

	}

	//------------------------------------------//
	//               Mesh LIGHT                 //
	//------------------------------------------//

	MeshLight::MeshLight(const std::string& filePath, Material* pMat, const Transform& transform /*= {}*/, float intensity /*= 1.f*/)
		: Light(pMat->GetMaterialColor(),intensity)
		, TriangleMesh(filePath, pMat, transform, true)
	{

	}

	Elite::RGBColor MeshLight::CalculateLight(const HitRecord& hit, FVector3& wi, float& pdf)
	{
		float visibleArea{};
		std::vector<unsigned int> visibleTriangles = std::move(GetVisibleTriangles(hit.m_HitPoint,visibleArea));

		float randomX{10.f}, randomY{10.f};
		while (randomX < 1.f)
			randomX = RandomFloat(1.f);

		while (randomY + randomY < 1.f)
			randomY = RandomFloat(1.f);

		int32_t tri = RandomInt32(int32_t(visibleTriangles.size()) - 1);
		
		FVector3 v0{ m_Vertices[tri] }, v1{ m_Vertices[++tri] }, v2{ m_Vertices[++tri] };
		FPoint3 samplePoint = m_Transform.GetPosition() + (v0 * randomX + v1 * randomY + (1 - randomX - randomY) * v2);

		FVector3 normal{ m_TriangleNormals[tri] };
		wi = samplePoint - hit.m_HitPoint;
		float distanceSqrd = SqrMagnitude(wi);
		Normalize(wi);
		float angle = Dot(-wi, normal);
		pdf = distanceSqrd / (angle * visibleArea);

		return angle > 0.f ? m_pMat->GetMaterialColor() * visibleArea * (float)E_PI : RGBColor();
	}

	void MeshLight::GetLightDirection(HitRecord& hit)
	{

	}

}
