#pragma once
#include "Scene.h"

namespace Elite
{
	class DiskLightScene : public Scene
	{
	public:
		DiskLightScene(const std::string& sceneName);
		virtual ~DiskLightScene() = default;

		virtual void Initialize() override;
		virtual void Update() override;
	};
}