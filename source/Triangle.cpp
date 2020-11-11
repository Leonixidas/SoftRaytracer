#include "pch.h"
#include "Triangle.h"
#include "EMath.h"

Elite::Triangle::Triangle(const FVector3& v0, const FVector3& v1, const FVector3& v2, const FPoint3& pos, Material* pMat, CullMode cullmode)
	: Geometry(pos, pMat)
	, m_V0(v0)
	, m_V1(v1)
	, m_V2(v2)
	, m_CullMode(cullmode)
{
	m_Normal = GetNormalized(Cross((pos + v1) - (pos + v2),(pos + v1) - (pos + v0)));
	m_Center = (v0 + v1 + v2) / 3.f;
}

bool Elite::Triangle::Hit(const Ray& ray, HitRecord& hit) const
{
	float dotVN = Dot(ray.m_Direction, m_Normal);

	if (AreEqual(dotVN, 0.f)) return false;
	
	if (hit.m_ShadowCalc)
	{
		if (dotVN > 0.f && m_CullMode == CullMode::FRONT_FACE_CULLING) return false;
		if (dotVN < 0.f && m_CullMode == CullMode::BACK_FACE_CULLING) return false;
	}
	else
	{
		if (dotVN > 0.f && m_CullMode == CullMode::BACK_FACE_CULLING) return false;
		if (dotVN < 0.f && m_CullMode == CullMode::FRONT_FACE_CULLING) return false;
	}

	FVector3 L = (m_Position + m_Center) - ray.m_Origin;
	float t = Dot(L, m_Normal) / Dot(ray.m_Direction, m_Normal);

	if (t < ray.m_Min || t > ray.m_Max) return false;

	FPoint3 hitPoint = ray.m_Origin + t * ray.m_Direction;

	FVector3 pointToSide = hitPoint - (m_Position + m_V0);
	FVector3 a{m_V1 - m_V0};
	FVector3 temp{ Cross(a,pointToSide) };
	if (Dot(m_Normal, temp) < 0.f) return false;

	pointToSide = hitPoint - (m_Position + m_V1);
	a = m_V2 - m_V1;

	if (Dot(m_Normal, Cross(a, pointToSide)) < 0.f) return false;

	pointToSide = hitPoint - (m_Position + m_V2);
	a = m_V0 - m_V2;

	if (Dot(m_Normal, Cross(a, pointToSide)) < 0.f) return false;

	hit.m_TValue = t;
	hit.m_HitPoint = hitPoint;
	hit.m_Normal = m_Normal;
	hit.m_pMaterial = m_pMat;

	return true;
}

void Elite::Triangle::Rotate(FVector3 rotation)
{
	m_Rotation += rotation;
	FMatrix3 rotMat = MakeRotationZYX(rotation.x, rotation.y, rotation.z);
	m_V0 = rotMat * m_V0;
	m_V1 = rotMat * m_V1;
	m_V2 = rotMat * m_V2;
	m_Normal = rotMat * m_Normal;
}

void Elite::Triangle::Translate(FVector3 translation)
{
	m_Position += translation;
}
