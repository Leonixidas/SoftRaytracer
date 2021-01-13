#pragma once
#include "RayTracerStructs.h"
#include "BasicMaterial.h"
#include "Transform.h"

namespace Elite
{
	class Geometry
	{
	public:
		explicit Geometry(Material* pMat, const Transform& transform = {}, bool isLight = false)
			: m_Transform(transform)
			, m_IsLight(isLight)
		{
			if (pMat == nullptr) m_pMat = new BasicMaterial();
			else m_pMat = pMat;
		}

		virtual ~Geometry()
		{
			delete m_pMat;
		}

		bool IsALight() { return m_IsLight; }

		virtual bool Hit(const Ray& ray, HitRecord& hit) const = 0;
		virtual FPoint3 GetRandomSurfacePoint() const = 0;

	protected:
		Transform m_Transform;
		Material* m_pMat;
		bool m_IsLight;
	};
}

