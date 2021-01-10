#pragma once
#include "EMath.h"

namespace Elite
{
	class Transform
	{
	public:
		Transform(const FPoint3& pos = {}, const FPoint3& rot = {}, const FPoint3& scale = {1.f,1.f,1.f});
		~Transform() = default;

		//euler angles
		void Rotate(const FVector3& rot);
		//euler angles
		void Rotate(float x, float y, float z);

		void Translate(const FVector3& trans);
		void Translate(float x, float y, float z);

		void Scale(const FVector3& scale);
		void Scale(float x, float y, float z);

		const FMatrix4& GetTransformMatrix() const { return m_TransMat; }
		FMatrix4 GetInverseTransformMatrix() const { return Inverse(m_TransMat); }
		const FMatrix3& GetRotationMatrix() const { return m_RotMat; }

		const FVector3& GetUpVector() const { return m_Up; }
		const FVector3& GetForwardVector() const { return m_Forward; }
		const FVector3& GetRightVector() const { return m_Right; }

		const FPoint3& GetPosition() const { return m_Position; }
		const FPoint3& GetScale() const { return m_Scale; }

		static FVector3 m_WorldUp, m_WorldForward, m_WorldRight;

	private:
		void UpdateMatrices();

		FPoint3 m_Position, m_Rotation, m_Scale;
		FVector3 m_Up, m_Forward, m_Right;
		FMatrix3 m_RotMat;
		FMatrix4 m_TransMat;
	};
}