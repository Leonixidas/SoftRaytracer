#pragma once
#include "Scene.h"

namespace Elite
{
	class PointLight;

	class GeometryTestScene : public Scene
	{
	public:
		GeometryTestScene(const std::string& sceneName);
		virtual ~GeometryTestScene() = default;

		virtual void Initialize() override;
		virtual void Update() override;

	private:
		PointLight* m_LeftPL, * m_RightPL, * m_BackPL;
	};
}