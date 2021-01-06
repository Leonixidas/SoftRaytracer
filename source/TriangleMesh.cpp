#include "pch.h"
#include "TriangleMesh.h"
#include <iostream>
#include <fstream>
#include <sstream>

Elite::TriangleMesh::TriangleMesh(const std::string& filePath, const FPoint3& pos, Material* pMat)
	: Geometry(pos, pMat)
	, m_Position(pos)
	, m_FilePath(filePath)
{
}


bool Elite::TriangleMesh::ReadOBJFile()
{
	std::ifstream input{ m_FilePath };
	if (!input) return false;
	std::string line{};
	char first{};

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
			m_Indices.push_back(i0 - 1);
			m_Indices.push_back(i1 - 1);
			m_Indices.push_back(i2 - 1);
			m_TriangleNormals.push_back(GetNormalized(Cross((m_Position + m_Vertices[i1-1]) - (m_Position + m_Vertices[i2 - 1]), (m_Position + m_Vertices[i1 - 1]) - (m_Position + m_Vertices[i0 - 1]))));
			m_TriangleCenters.push_back((m_Vertices[i0 - 1] + m_Vertices[i1 - 1] + m_Vertices[i2 - 1]) / 3.f);
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

	while (counter < size)
	{
		normal = m_TriangleNormals[counter / 3];
		center = m_TriangleCenters[counter / 3];
		v0 = m_Vertices[m_Indices[counter++]];
		v1 = m_Vertices[m_Indices[counter++]];
		v2 = m_Vertices[m_Indices[counter++]];

		float dotVN = Dot(ray.m_Direction, normal);

		if (AreEqual(dotVN, 0.f)) continue;

		FVector3 L = (m_Position + center) - ray.m_Origin;
		float t = Dot(L, normal) / Dot(ray.m_Direction, normal);

		if (t < ray.m_Min || t > ray.m_Max || t > hit.m_TValue) continue;

		FPoint3 hitPoint = ray.m_Origin + t * ray.m_Direction;

		FVector3 pointToSide = hitPoint - (m_Position + v0);
		FVector3 a{ v1 - v0 };
		FVector3 temp{ Cross(a,pointToSide) };
		if (Dot(normal, temp) < 0.f) continue;

		pointToSide = hitPoint - (m_Position + v1);
		a = v2 - v1;

		if (Dot(normal, Cross(a, pointToSide)) < 0.f) continue;

		pointToSide = hitPoint - (m_Position + v2);
		a = v0 - v2;

		if (Dot(normal, Cross(a, pointToSide)) < 0.f) continue;

		hit.m_TValue = t;
		hit.m_HitPoint = hitPoint;
		hit.m_Normal = normal;
		hit.m_pMaterial = m_pMat;

		gotHit = true;
	}
	hit.m_HitSomething = gotHit;
	return gotHit;
}
