#include "pch.h"
#include "Transform.h"

namespace Elite
{
	FVector3 Transform::m_WorldUp = { 0.f,1.f,0.f };

	FVector3 Transform::m_WorldForward = { 0.f,0.f,-1.f };

	FVector3 Transform::m_WorldRight = { 1.f,0.f,0.f };

	Transform::Transform(const FPoint3& pos, const FPoint3& rot, const FPoint3& scale)
	{
		m_Position = pos;
		m_Rotation = FPoint3(FVector3(rot) * float((E_PI / 180.f)));
		m_Scale = scale;
		UpdateMatrices();
	}

	void Transform::Rotate(const FVector3& rot)
	{
		m_Rotation += rot * float((E_PI/180.f));
		UpdateMatrices();
	}

	void Transform::Rotate(float x, float y, float z)
	{
		m_Rotation += FVector3{ x,y,z } * float((E_PI / 180.f));
		UpdateMatrices();
	}

	void Transform::Translate(const FVector3& trans)
	{
		m_Position += trans;
		UpdateMatrices();
	}

	void Transform::Translate(float x, float y, float z)
	{
		m_Position += FVector3{ x,y,z };
		UpdateMatrices();
	}

	void Transform::Scale(const FVector3& scale)
	{
		m_Scale += scale;
		UpdateMatrices();
	}

	void Transform::Scale(float x, float y, float z)
	{
		m_Scale += FVector3{ x,y,z };
		UpdateMatrices();
	}

	void Transform::UpdateMatrices()
	{
		m_RotMat = MakeRotationZYX(m_Rotation.x, m_Rotation.y, m_Rotation.z);
		FMatrix4 temp = MakeScale(m_Scale.x, m_Scale.y, m_Scale.z);
		m_Forward = GetNormalized(m_RotMat * m_WorldForward);
		m_Right = GetNormalized(m_RotMat * m_WorldRight);
		m_Up = GetNormalized(m_RotMat * m_WorldUp);

		FMatrix4 trans = MakeTranslation(FVector3(m_Position));
		m_TransMat = trans * (FMatrix4)m_RotMat * temp;
		//m_TransMat = FMatrix4{ m_Right, m_Up, m_Forward, FVector4{m_Position.x, m_Position.y, m_Position.z, 1.f} };
	}
}