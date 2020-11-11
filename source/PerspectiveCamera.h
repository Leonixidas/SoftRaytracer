#pragma once
#include "EMath.h"
#include "GameObject.h"

namespace Elite
{
	class PerspectiveCamera : public GameObject
	{
	public:
		PerspectiveCamera(FPoint3 pos, float FOV, FVector3 rot = {});
		virtual ~PerspectiveCamera() = default;

		FMatrix4 GetLookatMatrix();

		void Rotate(FVector3 rotation);
		void Translate(FVector3 translation);

		const FMatrix3& GetRotationMatrix() { return m_RotMat; }
		const FPoint3& GetPosition() { return m_Position; }

		float GetFOV();
		float GetViewAngleInDegrees() { return m_FOV; }
		float GetViewAngleInRadians() { return float(m_FOV * (E_PI / 180.f)); }

		float GetCameraSpeed() { return m_CameraSpeed; }

	private:
		void CalculateLookatMatrix();

		FMatrix4 m_LookatMat;

		FVector3 m_Forward;
		FVector3 m_Right;
		FVector3 m_Up;

		const FVector3 m_WorldUp{ 0.f,1.f,0.f };
		const FVector3 m_WorldForward{ 0.f,0.f,-1.f };
		const FVector3 m_WorldRight{ -1.f,0.f,0.f };

		FMatrix3 m_RotMat;
		FVector3 m_Rotation;
		FPoint3 m_Position;

		float m_FOV;
		float m_Degrees;
		float m_CameraSpeed;
	};
}

