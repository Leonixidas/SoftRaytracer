#pragma once
#include "RayTracerStructs.h"
#include "BasicMaterial.h"

namespace Elite
{
	class Geometry
	{
	public:
		explicit Geometry(FPoint3 pos, Material* pMat)
			: m_Position(pos)
		{
			if (pMat == nullptr) m_pMat = new BasicMaterial();
			else m_pMat = pMat;
		}

		virtual ~Geometry()
		{
			delete m_pMat;
		}

		virtual bool Hit(const Ray& ray, HitRecord& hit) const = 0;

	protected:
		FPoint3 m_Position;
		Material* m_pMat;
	};
}

