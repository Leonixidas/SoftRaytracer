#include "pch.h"
#include "BunnyScene.h"
#include "TriangleMesh.h"
#include "Plane.h"
#include "Material_PBR.h"
#include "Material_Lambert.h"
#include "PerspectiveCamera.h"
#include "PointLight.h"
#include "InputManager.h"
#include "ETimer.h"
#include "DirectionalLight.h"

Elite::BunnyScene::BunnyScene(const std::string& sceneName)
	: Scene(sceneName)
	, m_LeftPL(nullptr)
	, m_RightPL(nullptr)
	, m_BackPL(nullptr)
	, m_DL(nullptr)
{
}

void Elite::BunnyScene::Initialize()
{
	TriangleMesh* bunny = new TriangleMesh{ "lowpoly_bunny.obj", FPoint3{0.f,0.f,0.f}, new Material_PBR(RGBColor{1.f,1.f,1.f}, 1.f, RGBColor{0.4f,0.4f,0.4f}, false, 0.5f) };

	if (!bunny->ReadOBJFile()) return;

	AddGeometry(bunny);

	//WALLS
	AddGeometry(new Plane{ FPoint3{0.f,0.f,0.f}, FVector3{0.f,1.f,0.f}, new Material_Lambert(RGBColor{0.7f,0.75f,0.7f}) });
	AddGeometry(new Plane{ FPoint3{0.f,10.f,0.f}, FVector3{0.f,-1.f,0.f}, new Material_Lambert(RGBColor{0.7f,0.75f,0.7f}) });
	AddGeometry(new Plane{ FPoint3{0.f,0.f,-7.f}, FVector3{0.f,0.f,1.f}, new Material_Lambert(RGBColor{0.7f,0.75f,0.7f}) });
	AddGeometry(new Plane{ FPoint3{-5.f,0.f,0.f}, FVector3{1.f,0.f,0.f}, new Material_Lambert(RGBColor{0.7f,0.75f,0.7f}) });
	AddGeometry(new Plane{ FPoint3{5.f,0.f,0.f}, FVector3{-1.f,0.f,0.f}, new Material_Lambert(RGBColor{0.7f,0.75f,0.7f}) });

	//Camera
	PerspectiveCamera* cam = new PerspectiveCamera{ FPoint3{0.f,2.f,10.f}, 45.f };
	AddCamera(cam);

	//Lights
	m_BackPL = new PointLight{ FPoint3{0.f,5.f,-3.f}, RGBColor{1.f,0.84f,0.66f},40.f };
	m_LeftPL = new PointLight{ FPoint3{-2.5f,3.f,3.f}, RGBColor{1.f,0.84f,0.66f}, 20.f };
	m_RightPL = new PointLight{ FPoint3{2.5f,2.f,3.f}, RGBColor{1.f,0.84f,0.66f}, 10.f };
	m_DL = new DirectionalLight{ GetNormalized(FVector3{0.77f,-0.77f,-0.77f}) };

	AddLight(m_LeftPL);
	AddLight(m_RightPL);
	AddLight(m_BackPL);
	AddLight(m_DL);
}

void Elite::BunnyScene::Update()
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
	if (input.IsKeyReleased(ScanCode::SDL_SCANCODE_J)) m_DL->PressLightSwitch();

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
