#include "pch.h"
#include "Rectangle.h"

namespace Elite
{
	Rectangle::Rectangle(Material* pMat, const Transform& transform, bool isLight /*= false*/)
		: Geometry(pMat, transform, isLight)
	{
		FMatrix4 tempMat = m_Transform.GetTransformMatrix();
		m_Vertices.push_back(FVector3(tempMat * FVector4{ -0.5f,0.f,0.5f, 1.f }));
		m_Vertices.push_back(FVector3(tempMat * FVector4{ -0.5f,0.f,-0.5f, 1.f }));
		m_Vertices.push_back(FVector3(tempMat * FVector4{ 0.5f,0.f,0.5f, 1.f }));
		m_Vertices.push_back(FVector3(tempMat * FVector4{ 0.5f,0.f,-0.5f, 1.f }));

		m_Indices = { 0,2,1,1,2,3 };
	}

	bool Rectangle::Hit(const Ray& ray, HitRecord& hit) const
	{
		FPoint3 pos = m_Transform.GetPosition();
		FVector3 normal = m_Transform.GetUpVector();
		float dotVN = Dot(ray.m_Direction, normal);

		if (AreEqual(dotVN, 0.f)) return false;

		FVector3 L = pos - ray.m_Origin;
		float t = Dot(L, normal) / Dot(ray.m_Direction, normal);

		if (t < ray.m_Min || t > ray.m_Max) return false;

		FPoint3 hitPoint = ray.m_Origin + t * ray.m_Direction;
		
		bool inside = false;
		size_t index = 0;
		while (index < m_Indices.size() && !inside)
		{
			inside = true;
			FVector3 v0 = m_Vertices[m_Indices[index++]];
			FVector3 v1 = m_Vertices[m_Indices[index++]];
			FVector3 v2 = m_Vertices[m_Indices[index++]];

			FVector3 pointToSide = hitPoint - (pos + v0);
			FVector3 a{ v1 - v0 };

			if (Dot(normal, Cross(a, pointToSide)) < 0.f) { inside = false; continue; }

			pointToSide = hitPoint - (pos + v1);
			a = v2 - v1;

			if (Dot(normal, Cross(a, pointToSide)) < 0.f) { inside = false; continue; }

			pointToSide = hitPoint - (pos + v2);
			a = v0 - v2;

			if (Dot(normal, Cross(a, pointToSide)) < 0.f) { inside = false; continue; }
		}

		if (!inside) return false;


		if (t < hit.m_TValue)
		{
			hit.m_TValue = t;
			hit.m_HitPoint = hitPoint;
			hit.m_Normal = normal;
			hit.m_pMaterial = m_pMat;
			hit.m_HitSomething = true;
			hit.m_IsLight = m_IsLight;
			hit.m_IncomingLightDirection = normal;
		}

		return true;
	}

	FPoint3 Rectangle::GetRandomSurfacePoint() const
	{
		float randomX = RandomBinomial(0.5f);
		float randomZ = RandomBinomial(0.5f);
		return FPoint3(m_Transform.GetTransformMatrix() * FPoint4(randomX, 0.f, randomZ, 1.f));
	}

}