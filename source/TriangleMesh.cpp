#include "pch.h"
#include "TriangleMesh.h"
#include <iostream>
#include <fstream>
#include <sstream>

Elite::TriangleMesh::TriangleMesh(const std::string& filePath, Material* pMat, const Transform& transform, bool isLight)
	: Geometry(pMat, transform, isLight)
	, m_FilePath(filePath)
{
}


bool Elite::TriangleMesh::ReadOBJFile()
{
	std::ifstream input{ m_FilePath };
	if (!input) return false;
	std::string line{};
	char first{};
	FPoint3 pos = m_Transform.GetPosition();

	while (!input.eof())
	{
		input >> first;

		switch (first)
		{
		case 'v':
		{
			FVector3 vertex{};
			input >> vertex.x >> vertex.y >> vertex.z;
			m_Vertices.push_back(vertex);
		}
			break;
		case 'f':
		{
			unsigned int i0{}, i1{}, i2{};
			input >> i0 >> i1 >> i2;
			--i0;
			--i1;
			--i2;
			m_Indices.push_back(i0);
			m_Indices.push_back(i1);
			m_Indices.push_back(i2);
			FVector3 v0{ m_Vertices[i0] }, v1{ m_Vertices[i1] };
			m_TriangleAreas.push_back(Magnitude(Cross((pos + m_Vertices[i1]) - (pos + m_Vertices[i2]), (pos + m_Vertices[i1]) - (pos + m_Vertices[i0]))) / 2.f);
			m_TriangleNormals.push_back(GetNormalized(Cross((pos + m_Vertices[i1]) - (pos + m_Vertices[i2]), (pos + m_Vertices[i1]) - (pos + m_Vertices[i0]))));
			m_TriangleCenters.push_back((m_Vertices[i0] + m_Vertices[i1] + m_Vertices[i2]) / 3.f);
		}
			break;
		default:
			std::getline(input, line);
			break;
		}
		
	}

	input.close();

	return true;
}

bool Elite::TriangleMesh::Hit(const Ray& ray, HitRecord& hit) const
{
	size_t counter{}, triangleCounter{};
	size_t size{ m_Indices.size() };
	FVector3 v0{}, v1{}, v2{}, normal{}, center{};

	bool gotHit{ false };
	FPoint3 pos = m_Transform.GetPosition();

	while (counter < size)
	{
		normal = m_TriangleNormals[counter / 3];
		center = m_TriangleCenters[counter / 3];
		v0 = m_Vertices[m_Indices[counter++]];
		v1 = m_Vertices[m_Indices[counter++]];
		v2 = m_Vertices[m_Indices[counter++]];

		float dotVN = Dot(ray.m_Direction, normal);

		if (AreEqual(dotVN, 0.f)) continue;

		FVector3 L = (pos + center) - ray.m_Origin;
		float t = Dot(L, normal) / Dot(ray.m_Direction, normal);

		if (t < ray.m_Min || t > ray.m_Max || t > hit.m_TValue) continue;

		FPoint3 hitPoint = ray.m_Origin + t * ray.m_Direction;

		FVector3 pointToSide = hitPoint - (pos + v0);
		FVector3 a{ v1 - v0 };
		FVector3 temp{ Cross(a,pointToSide) };
		if (Dot(normal, temp) < 0.f) continue;

		pointToSide = hitPoint - (pos + v1);
		a = v2 - v1;

		if (Dot(normal, Cross(a, pointToSide)) < 0.f) continue;

		pointToSide = hitPoint - (pos + v2);
		a = v0 - v2;

		if (Dot(normal, Cross(a, pointToSide)) < 0.f) continue;

		hit.m_TValue = t;
		hit.m_HitPoint = hitPoint;
		hit.m_Normal = normal;
		hit.m_pMaterial = m_pMat;
		hit.m_IsLight = true;
		gotHit = true;
	}
	
	return gotHit;
}

Elite::FPoint3 Elite::TriangleMesh::GetRandomSurfacePoint() const
{
	return FPoint3();
}

std::vector<unsigned int> Elite::TriangleMesh::GetVisibleTriangles(const FPoint3& pointInSpace, float& visibleArea)
{
	size_t counter{}, triangleCounter{};
	size_t size{ m_TriangleNormals.size() };
	FVector3 v0{}, v1{}, v2{}, normal{}, toCenter{};

	std::vector<unsigned int> visibleTriangles;
	bool gotHit{ false };
	FPoint3 pos = m_Transform.GetPosition();

	while (counter < size)
	{
		normal = m_TriangleNormals[counter / 3];
		toCenter = GetNormalized((pos + m_TriangleCenters[counter / 3]) - pointInSpace);

		if (Dot(normal, -toCenter) > 0.f)
		{
			visibleTriangles.push_back((unsigned int)counter);
			visibleArea += m_TriangleAreas[counter];
		}

		++counter;
	}
	return std::move(visibleTriangles);
}
