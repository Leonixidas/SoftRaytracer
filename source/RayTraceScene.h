#pragma once
#include "Scene.h"

namespace Elite
{
	class PointLight;
	class DirectionalLight;
	class Triangle;

	class RayTraceScene : public Scene
	{
	public:
		RayTraceScene(const std::string& sceneName);

		void Initialize() override;
		void Update() override;

	private:
		PointLight* m_LeftPL, * m_RightPL, *m_BackPL;
		DirectionalLight* m_DL;
		Triangle* m_LeftT, * m_MiddleT, * m_RightT;
		float m_RotSpeed;
	};
}

