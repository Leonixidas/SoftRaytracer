#include "pch.h"
#include "SphereLightScene.h"
#include "Sphere.h"
#include "Plane.h"
#include "InputManager.h"
#include "PerspectiveCamera.h"
#include "RayTracerEnums.h"
#include "DirectionalLight.h"
#include "Material_Lambert.h"
#include "AreaLight.h"
#include "EmissiveMaterial.h"
#include "ETimer.h"

namespace Elite
{
	SphereLightScene::SphereLightScene(const std::string& sceneName)
		: Scene(sceneName)
	{
	}
	void SphereLightScene::Initialize()
	{
		//WALLS
		AddGeometry(new Plane{ new Material_Lambert(RGBColor{0.65f,0.65f,0.65f}), Transform{ FPoint3{0.f,0.f,0.f}, FPoint3{0.f,0.f,0.f}} });
		AddGeometry(new Plane{ new Material_Lambert(RGBColor{0.7f,0.75f,0.7f}), Transform{ FPoint3{0.f,8.f,0.f}, FPoint3{180.f,0.f,0.f}} });
		AddGeometry(new Plane{ new Material_Lambert(RGBColor{0.86f,0.86f,0.55f}), Transform{ FPoint3{0.f,0.f,-7.f}, FPoint3{90.f,0.f,0.f}} });
		AddGeometry(new Plane{ new Material_Lambert(RGBColor{0.7f,0.75f,0.7f}), Transform{ FPoint3{-5.f,0.f,0.f}, FPoint3{0.f,0.f,-90.f}} });
		AddGeometry(new Plane{ new Material_Lambert(RGBColor{0.7f,0.75f,0.7f}), Transform{ FPoint3{5.f,0.f,0.f}, FPoint3{0.f,0.f,90.f}} });


		SphereLight* light = new SphereLight{ new EmissiveMaterial(RGBColor{ 1.f,1.f,1.f }) ,Transform{FPoint3{0.f,3.f,0.f} }, 1.f,1.f };

		AddGeometry(light);
		//Camera
		PerspectiveCamera* cam = new PerspectiveCamera{ FPoint3{0.f,3.5f,12.f}, 45.f };
		AddCamera(cam);

		AddLight(light);
	}

	void SphereLightScene::Update()
	{
		float deltaTime = Timer::GetInstance().GetElapsed();
		InputManager& input = InputManager::GetInstance();
		FVector3 translation{}, rotation{};

		if (input.IsKeyPressed(ScanCode::SDL_SCANCODE_D)) translation.x += m_pCurrentCamera->GetCameraSpeed() * deltaTime;
		if (input.IsKeyPressed(ScanCode::SDL_SCANCODE_A)) translation.x -= m_pCurrentCamera->GetCameraSpeed() * deltaTime;
		if (input.IsKeyPressed(ScanCode::SDL_SCANCODE_W)) translation.z += m_pCurrentCamera->GetCameraSpeed() * deltaTime;
		if (input.IsKeyPressed(ScanCode::SDL_SCANCODE_S)) translation.z -= m_pCurrentCamera->GetCameraSpeed() * deltaTime;

		if (input.IsMouseButtonPressed(MouseButton::MOUSE_LEFT) && input.IsMouseButtonPressed(MouseButton::MOUSE_RIGHT))
		{
			SDL_SetRelativeMouseMode(SDL_TRUE);
			float x{}, y{};
			input.GetMouseValues(x, y);

			translation.y += -y * deltaTime;
		}
		else if (input.IsMouseButtonPressed(MouseButton::MOUSE_LEFT))
		{
			SDL_SetRelativeMouseMode(SDL_TRUE);
			float x{}, y{};
			input.GetMouseValues(x, y);

			rotation.y += -x * deltaTime;
			translation.z += -y * deltaTime;
		}
		else if (input.IsMouseButtonPressed(MouseButton::MOUSE_RIGHT))
		{
			SDL_SetRelativeMouseMode(SDL_TRUE);
			float x{}, y{};
			input.GetMouseValues(x, y);

			rotation.x += -y * 5 * deltaTime;
			rotation.y += -x * 5 * deltaTime;
		}
		else SDL_SetRelativeMouseMode(SDL_FALSE);

		m_pCurrentCamera->Translate(translation);
		m_pCurrentCamera->Rotate(rotation);
	}
	
}