#pragma once
#include "Geometry.h"
#include "RayTracerEnums.h"

namespace Elite
{
	class Triangle : public Geometry
	{
	public:
		Triangle(const FVector3& v0, const FVector3& v1, const FVector3& v2, Material* pMat, const Transform& transform = {}, CullMode cullmode = CullMode::BACK_FACE_CULLING, bool isLight = false);
		~Triangle() = default;

		virtual bool Hit(const Ray& ray, HitRecord& hit) const override;

		void Rotate(FVector3 rotation);
		void Translate(FVector3 translation);

		void SetCullMode(CullMode cullmode) { m_CullMode = cullmode; }
		CullMode GetCullMode() { return m_CullMode; }

	private:
		FVector3 m_Normal, m_Rotation;
		FVector3 m_V0, m_V1, m_V2, m_Center;
		CullMode m_CullMode;
	};
}

