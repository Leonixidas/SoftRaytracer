#pragma once
#include "Scene.h"

namespace Elite
{
	class SphereLightScene : public Scene
	{
	public:
		SphereLightScene(const std::string& sceneName);
		~SphereLightScene() = default;

		void Initialize() override;
		void Update() override;
	};
}