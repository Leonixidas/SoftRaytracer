#include "pch.h"
#include "SphereLightScene.h"
#include "Sphere.h"
#include "Plane.h"
#include "InputManager.h"
#include "PerspectiveCamera.h"
#include "RayTracerEnums.h"
#include "PointLight.h"
#include "DirectionalLight.h"
#include "Material_Lambert.h"
#include "Material_LambertPhong.h"
#include "Material_PBR.h"

namespace Elite
{
	SphereLightScene::SphereLightScene(const std::string& sceneName)
		: Scene(sceneName)
	{
	}
	void SphereLightScene::Initialize()
	{
	}

	void SphereLightScene::Update()
	{
	}
}