#pragma once
#include "Scene.h"

namespace Elite
{
	class PointLight;
	class DirectionalLight;

	class BunnyScene : public Scene
	{
	public:
		BunnyScene(const std::string& sceneName);

		void Initialize() override;
		void Update() override;

	private:
		PointLight* m_RightPL, * m_LeftPL, *m_BackPL;
		DirectionalLight* m_DL;
	};
}

