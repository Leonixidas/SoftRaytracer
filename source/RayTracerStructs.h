#pragma once
#include "EMath.h"
#include "ERGBColor.h"
#include "RayTracerEnums.h"

namespace Elite
{
	class Material;
	class Geometry;

	struct Ray
	{
		//This will be {0,0,-1} for orthographic view, it will change once we enter the perspective view
		FVector3 m_Direction{};
		FPoint3 m_Origin{};
		float m_Min = 0.00001f;
		float m_Max = FLT_MAX;
	};

	struct HitRecord
	{
		FVector3 m_Normal{};
		FVector3 m_IncomingLightDirection{};
		FPoint3 m_HitPoint{};
		Material* m_pMaterial = nullptr;
		float m_TValue{ FLT_MAX };
		bool m_ShadowCalc = false;
	};

	struct InputAction
	{
		InputAction(const ScanCode keyCode, unsigned int inputID)
			: m_KeyCode(keyCode)
			, m_InputID(inputID)
		{}

		~InputAction() = default;
		ScanCode GetKeyCode() { return m_KeyCode; }

	private:
		ScanCode m_KeyCode;
		unsigned int m_InputID;
	};
}