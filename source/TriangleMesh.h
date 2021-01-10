#pragma once
#include "Geometry.h"
#include <string>
#include <vector>
#include "EMath.h"

namespace Elite
{
	class Triangle;

	class TriangleMesh : public Geometry
	{
	public:
		TriangleMesh(const std::string& filePath, Material* pMat, const Transform& transform = {}, bool isLight = false);

		~TriangleMesh() = default;

		bool ReadOBJFile();

		virtual bool Hit(const Ray& ray, HitRecord& hit) const override;

	private:
		std::vector<FVector3> m_Vertices;
		std::vector<FVector3> m_TriangleNormals;
		std::vector<FVector3> m_TriangleCenters;
		std::vector<unsigned int> m_Indices;
		std::vector<float> m_TriangleAreas;
		std::string m_FilePath;
		float m_Area;
	};
}

