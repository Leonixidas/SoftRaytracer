#pragma once
#include "Scene.h"

namespace Elite
{
	class MeshLightScene : public Scene
	{
	public: 
		MeshLightScene(const std::string& sceneName);
		virtual ~MeshLightScene() = default;

		virtual void Initialize() override;
		virtual void Update() override;
	};
}