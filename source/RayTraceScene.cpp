#include "pch.h"
#include "RayTraceScene.h"
#include "Sphere.h"
#include "Plane.h"
#include "Triangle.h"
#include "InputManager.h"
#include "PerspectiveCamera.h"
#include "RayTracerEnums.h"
#include "ETimer.h"
#include "PointLight.h"
#include "DirectionalLight.h"
#include "Material_Lambert.h"
#include "Material_LambertPhong.h"
#include "Material_PBR.h"

Elite::RayTraceScene::RayTraceScene(const std::string& sceneName)
	: Scene(sceneName)
	, m_DL(nullptr)
	, m_LeftPL(nullptr)
	, m_RightPL(nullptr)
	, m_BackPL(nullptr)
	, m_LeftT(nullptr)
	, m_MiddleT(nullptr)
	, m_RightT(nullptr)
	, m_RotSpeed(1.f)
{
}

void Elite::RayTraceScene::Initialize()
{
	FPoint3 pos1{ -2.5f,4.f,0.f }, pos2{ 0.f,4.f,0.f }, pos3{ 2.5f,4.f,0.f };
	//DIELECTRICS
	AddGeometry(new Sphere{ new Material_PBR(RGBColor{0.8f,0.8f,0.8f}, 1.f, RGBColor{0.4f,0.4f,0.4f}, false, 1.f) , Transform{pos1} });
	AddGeometry(new Sphere{ new Material_PBR(RGBColor{0.8f,0.8f,0.8f}, 1.f, RGBColor{0.4f,0.4f,0.4f}, false, 0.6f), Transform{pos2} });
	AddGeometry(new Sphere{ new Material_PBR(RGBColor{0.8f,0.8f,0.8f}, 1.f, RGBColor{0.4f,0.4f,0.4f}, false, 0.1f), Transform{pos3} });

	//METALS
	pos1 = FPoint3{ -2.5f,1.f,0.f };
	pos2 = FPoint3{ 0.f,1.f,0.f };
	pos3 = FPoint3{ 2.5f,1.f,0.f };
	AddGeometry(new Sphere{ new Material_PBR(RGBColor{0.8f,0.8f,0.8f}, 1.f, RGBColor{0.4f,0.4f,0.4f}, true, 1.f), Transform{pos1} });
	AddGeometry(new Sphere{ new Material_PBR(RGBColor{0.8f,0.8f,0.8f}, 1.f, RGBColor{0.4f,0.4f,0.4f}, true, 0.6f), Transform{pos2} });
	AddGeometry(new Sphere{ new Material_PBR(RGBColor{0.8f,0.8f,0.8f}, 1.f, RGBColor{0.4f,0.4f,0.4f}, true, 0.1f), Transform{pos3} });


	//TRIANGLES
	FVector3 v0{ -0.75f,1.5f,0.f }, v1{ -0.75f,0.f,0.f }, v2{ 0.75f,0.f,0.f };
	pos1 = FPoint3{ -2.5f, 6.f, 0.f };
	pos2 = FPoint3{ 0.f, 6.f, 0.f };
	pos3 = FPoint3{ 2.5f, 6.f, 0.f };
	m_LeftT = new Triangle{ v0,v1,v2, new Material_Lambert(RGBColor{1.f,1.f,1.f})	, Transform{pos1},	CullMode::BACK_FACE_CULLING };
	m_MiddleT = new Triangle{ v0,v1,v2, new Material_Lambert(RGBColor{1.f,1.f,1.f})	, Transform{pos2}, CullMode::FRONT_FACE_CULLING };
	m_RightT = new Triangle{ v0,v1,v2, new Material_Lambert(RGBColor{1.f,1.f,1.f})	, Transform{pos3}, CullMode::NO_CULLING };
	AddGeometry(m_LeftT);
	AddGeometry(m_MiddleT);
	AddGeometry(m_RightT);

	//WALLS
	AddGeometry(new Plane{ new Material_Lambert(RGBColor{0.7f,0.75f,0.7f}), Transform{ FPoint3{0.f,0.f,0.f}, FPoint3{0.f,0.f,0.f}} });
	AddGeometry(new Plane{ new Material_Lambert(RGBColor{0.7f,0.75f,0.7f}), Transform{ FPoint3{0.f,10.f,0.f}, FPoint3{180.f,0.f,0.f}} });
	AddGeometry(new Plane{ new Material_Lambert(RGBColor{0.7f,0.75f,0.7f}), Transform{ FPoint3{0.f,0.f,-7.f}, FPoint3{90.f,0.f,0.f}} });
	AddGeometry(new Plane{ new Material_Lambert(RGBColor{0.7f,0.75f,0.7f}), Transform{ FPoint3{-5.f,0.f,0.f}, FPoint3{0.f,0.f,-90.f}} });
	AddGeometry(new Plane{ new Material_Lambert(RGBColor{0.7f,0.75f,0.7f}), Transform{ FPoint3{5.f,0.f,0.f}, FPoint3{0.f,0.f,90.f}} });

	//Camera
	PerspectiveCamera* cam = new PerspectiveCamera{ FPoint3{0.f,3.5f,12.f}, 45.f };
	AddCamera(cam);

	//Lights
	m_BackPL = new PointLight{ FPoint3{0.f,7.f,-5.f}, RGBColor{1.f,0.84f,0.66f}, 30.f };
	m_LeftPL = new PointLight{ FPoint3{-3.f,6.f,5.f}, RGBColor{1.f,0.84f,0.66f}, 40.f };
	m_RightPL = new PointLight{ FPoint3{3.f,5.f,5.f}, RGBColor{.95f,0.80f,0.75f}, 40.f };

	m_DL = new DirectionalLight{ GetNormalized(FVector3{0.77f,-0.77f,-0.77f}) };

	AddLight(m_LeftPL);
	AddLight(m_RightPL);
	AddLight(m_BackPL);
	AddLight(m_DL);
}

void Elite::RayTraceScene::Update()
{
	float deltaTime = Timer::GetInstance().GetElapsed();
	InputManager& input = InputManager::GetInstance();
	FVector3 translation{}, rotation{};

	float triangleRot = m_RotSpeed * deltaTime;
	m_LeftT->Rotate(FVector3{ 0.f,triangleRot, 0.f });
	m_MiddleT->Rotate(FVector3{ 0.f,triangleRot, 0.f });
	m_RightT->Rotate(FVector3{ 0.f,triangleRot, 0.f });

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
