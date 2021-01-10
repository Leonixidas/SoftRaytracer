#include "pch.h"
#include "GeometryTestScene.h"
#include "Plane.h"
#include "Material_Lambert.h"
#include "Material_LambertPhong.h"
#include "PerspectiveCamera.h"
#include "PointLight.h"
#include "Rectangle.h"
#include "Material_PBR.h"
#include "InputManager.h"
#include "ETimer.h"
#include "Disk.h"

namespace Elite
{

	GeometryTestScene::GeometryTestScene(const std::string& sceneName)
		: Scene(sceneName)
	{

	}

	void GeometryTestScene::Initialize()
	{
		//WALLS
		AddGeometry(new Plane{ new Material_Lambert(RGBColor{0.7f,0.75f,0.7f}), Transform{ FPoint3{0.f,0.f,0.f}, FPoint3{0.f,0.f,0.f}} });
		AddGeometry(new Plane{ new Material_Lambert(RGBColor{0.7f,0.75f,0.7f}), Transform{ FPoint3{0.f,10.f,0.f}, FPoint3{180.f,0.f,0.f}} });
		AddGeometry(new Plane{ new Material_Lambert(RGBColor{0.7f,0.75f,0.7f}), Transform{ FPoint3{0.f,0.f,-7.f}, FPoint3{90.f,0.f,0.f}} });
		AddGeometry(new Plane{ new Material_Lambert(RGBColor{0.7f,0.75f,0.7f}), Transform{ FPoint3{-5.f,0.f,0.f}, FPoint3{0.f,0.f,-90.f}} });
		AddGeometry(new Plane{ new Material_Lambert(RGBColor{0.7f,0.75f,0.7f}), Transform{ FPoint3{5.f,0.f,0.f}, FPoint3{0.f,0.f,90.f}} });

		//GEO
		AddGeometry(new Rectangle{ new Material_Lambert(RGBColor{0.8f,0.8f,0.8f}), Transform{{0.f,5.f,0.f},{-90.f,0.f,0.f},{2.f,1.f,3.f}} });
		AddGeometry(new Disk{ new Material_Lambert(RGBColor{0.8f,0.8f,0.8f}), Transform{{-3.f,5.f,0.f},{-90.f,0.f,0.f}}, 2.f });

		//Camera
		PerspectiveCamera* cam = new PerspectiveCamera{ FPoint3{0.f,3.5f,12.f}, 45.f };
		AddCamera(cam);

		//Lights
		m_BackPL = new PointLight{ FPoint3{0.f,7.f,-5.f}, RGBColor{1.f,0.84f,0.66f}, 30.f };
		m_LeftPL = new PointLight{ FPoint3{-3.f,6.f,5.f}, RGBColor{1.f,0.84f,0.66f}, 40.f };
		m_RightPL = new PointLight{ FPoint3{3.f,5.f,5.f}, RGBColor{.95f,0.80f,0.75f}, 40.f };

		AddLight(m_LeftPL);
		AddLight(m_RightPL);
		AddLight(m_BackPL);
	}

	void GeometryTestScene::Update()
	{
		float deltaTime = Timer::GetInstance().GetElapsed();
		InputManager& input = InputManager::GetInstance();
		FVector3 translation{}, rotation{};

		if (input.IsKeyPressed(ScanCode::SDL_SCANCODE_D)) translation.x += m_pCurrentCamera->GetCameraSpeed() * deltaTime;
		if (input.IsKeyPressed(ScanCode::SDL_SCANCODE_A)) translation.x -= m_pCurrentCamera->GetCameraSpeed() * deltaTime;
		if (input.IsKeyPressed(ScanCode::SDL_SCANCODE_W)) translation.z += m_pCurrentCamera->GetCameraSpeed() * deltaTime;
		if (input.IsKeyPressed(ScanCode::SDL_SCANCODE_S)) translation.z -= m_pCurrentCamera->GetCameraSpeed() * deltaTime;

		if (input.IsKeyReleased(ScanCode::SDL_SCANCODE_K)) m_LeftPL->PressLightSwitch();
		if (input.IsKeyReleased(ScanCode::SDL_SCANCODE_L)) m_RightPL->PressLightSwitch();
		if (input.IsKeyReleased(ScanCode::SDL_SCANCODE_O)) m_BackPL->PressLightSwitch();

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