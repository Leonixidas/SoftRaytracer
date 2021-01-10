#include "pch.h"
#include "PerspectiveCamera.h"

Elite::PerspectiveCamera::PerspectiveCamera(FPoint3 pos, float fov, FVector3 rot)
	: GameObject()
	, m_Position(pos)
	, m_FOV()
	, m_Degrees(fov)
	, m_CameraSpeed(5.0f)
	, m_Rotation(rot)
	, m_Forward(0.f,0.f,-1.f)
	, m_Up(0.f,1.f,0.f)
	, m_Right(1.f,0.f,0.f)
	, m_LookatMat()
{
	CalculateLookatMatrix();
	m_FOV = tanf((fov * (float(E_PI) / 180.f)) / 2.f);
}

Elite::FMatrix4 Elite::PerspectiveCamera::GetLookatMatrix()
{
	return m_LookatMat;
}

//This value will be added to the current rotation
void Elite::PerspectiveCamera::Rotate(FVector3 rotation)
{
	m_Rotation += (rotation * float((E_PI / 180.f)));
	CalculateLookatMatrix();
}

//This value will be added to the current position
void Elite::PerspectiveCamera::Translate(FVector3 translation)
{
	m_Position += m_Right * translation.x + m_Forward * translation.z + m_Up * translation.y;
	CalculateLookatMatrix();
}

float Elite::PerspectiveCamera::GetFOV()
{
	return m_FOV;
}

void Elite::PerspectiveCamera::CalculateLookatMatrix()
{
	m_RotMat = MakeRotationZYX(m_Rotation.x,m_Rotation.y,m_Rotation.z);

	m_Forward = GetNormalized(m_RotMat * m_WorldForward);
	m_Right = GetNormalized(m_RotMat * m_WorldRight);
	m_Up = GetNormalized(m_RotMat * m_WorldUp);

	m_LookatMat = FMatrix4{ m_Right,m_Up,m_Forward,FVector4{m_Position.x, m_Position.y, m_Position.z, 1.f} };
}
