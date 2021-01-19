#include "pch.h"
//External includes
#include "SDL.h"
#include "SDL_surface.h"
#include <vector>

//Project includes
#include "ERenderer.h"
#include "PerspectiveCamera.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Geometry.h"
#include "Light.h"
#include "InputManager.h"
#include "Threadpool.h"

Elite::Renderer::Renderer(SDL_Window * pWindow)
	: m_pWindow( pWindow )
	, m_pFrontBuffer(SDL_GetWindowSurface(pWindow))
	, m_pBackBuffer()
	, m_pBackBufferPixels()
	, m_Width()
	, m_Height()
	, m_AreShadowsEnabled(false)
	, m_RenderMode()
	, m_Threadpool(new Threadpool(std::thread::hardware_concurrency()-1))
	, m_RaysPerPixel(10)
{
	//Initialize
	int width, height = 0;
	SDL_GetWindowSize(pWindow, &width, &height);
	m_Width = static_cast<uint32_t>(width);
	m_Height = static_cast<uint32_t>(height);
	m_pBackBuffer = SDL_CreateRGBSurface(0, m_Width, m_Height, 32, 0, 0, 0, 0);
	m_pBackBufferPixels = (uint32_t*)m_pBackBuffer->pixels;
}

Elite::Renderer::~Renderer()
{
	delete m_Threadpool;
}

void Elite::Renderer::Render()
{
	SDL_LockSurface(m_pBackBuffer);
	Scene* scene =				SceneManager::GetInstance().GetCurrentScene();
	PerspectiveCamera* pCam =	scene->GetCurrentCamera();

	Ray ray{};

	FMatrix4 lookat{};
	float fov{};

	if (pCam != nullptr)
	{
		lookat = pCam->GetLookatMatrix();
		ray.m_Origin = pCam->GetPosition();
		fov = pCam->GetFOV();
	}
	
	float aspectRatio = float(m_Width) / m_Height;
	
	// calculating the amount of pixels for each render block
	uint32_t stepSize = m_Width / 32;

	for (uint32_t r = 0; r < m_Height; r += stepSize)
	{
		for (uint32_t c = 0; c < m_Width; c += stepSize)
		{
			uint32_t cEnd = c + stepSize;
			uint32_t rEnd = r + stepSize;
			m_Threadpool->Enqueue([this, lookat, scene, aspectRatio, fov, ray, c, cEnd, r, rEnd]
			{
				for (uint32_t i = r; i < rEnd; ++i)
				{
					for (uint32_t j = c; j < cEnd; ++j)
					{
						Trace(i, j, scene, ray, lookat, aspectRatio, fov);
					}
				}
			});
		}
	}

	m_Threadpool->WaitFinish();


	//SaveBackbufferToImage();
	
	//InputManager::GetInstance().Quit();
	SDL_UnlockSurface(m_pBackBuffer);
	SDL_BlitSurface(m_pBackBuffer, 0, m_pFrontBuffer, 0);
	SDL_UpdateWindowSurface(m_pWindow);

}

bool Elite::Renderer::SaveBackbufferToImage() const
{
	return SDL_SaveBMP(m_pBackBuffer, "BackbufferRender.bmp");
}

void Elite::Renderer::FlipShadowsEnabled()
{
	m_AreShadowsEnabled = !m_AreShadowsEnabled;
}

void Elite::Renderer::SwitchRenderMode()
{
	m_RenderMode = RenderMode(((int)m_RenderMode + 1) % 3);
}

void Elite::Renderer::Trace(uint32_t r, uint32_t c, Scene* scene, Ray ray, FMatrix4 lookat, float aspectRatio, float fov)
{
	FPoint4 pixelPos{};
	RGBColor finalColor{};
	Ray rayToLight{};
	HitRecord hit{}, hitToLight{};
	const std::vector<Geometry*>& geom = scene->GetSceneGeometry();
	const std::vector<Light*>& lights = scene->GetSceneLights();

	pixelPos.x = (2 * ((c + 0.5f) / m_Width) - 1) * aspectRatio * fov;
	pixelPos.y = (1 - 2 * ((r + 0.5f) / m_Height)) * fov;
	pixelPos.z = 1.f;
	pixelPos.w = 1.f;
	pixelPos = lookat * pixelPos;

	ray.m_Direction = FPoint3{ pixelPos.x, pixelPos.y, pixelPos.z } - ray.m_Origin;
	Normalize(ray.m_Direction);

	for (size_t j{}, size{ geom.size() }; j < size; ++j)
	{
		geom[j]->Hit(ray, hit);
	}

	if (hit.m_HitSomething)
	{
		if (!hit.m_IsLight)
		{
			for (size_t j{}; j < lights.size(); ++j)
			{
				//if (!lights[j]->IsEnabled()) continue;

				hitToLight = {};

				//lights[j]->GetLightDirection(hit);
				/*if (!AreEqual(SqrMagnitude(hit.m_IncomingLightDirection), 1.f))
					rayToLight.m_Max = Normalize(hit.m_IncomingLightDirection);
				else rayToLight.m_Max = FLT_MAX;

				rayToLight.m_Direction = -hit.m_IncomingLightDirection;
				rayToLight.m_Origin = hit.m_HitPoint;

				if (m_AreShadowsEnabled)
				{
					hitToLight.m_ShadowCalc = true;

					for (Geometry* g : geom)
					{
						if (g->Hit(rayToLight, hitToLight))
							break;
					}

					if (hitToLight.m_HitSomething) continue;
				}

				float dotL = Dot(hit.m_Normal, -hit.m_IncomingLightDirection);

				if(dotL < 0.f) continue;

				FVector3 wi{};
				float pdf{};
				switch (m_RenderMode)
				{
				case RenderMode::COMBINED:
					finalColor += lights[j]->CalculateLight(hit, wi, pdf) * hit.m_pMaterial->Shade(hit, -ray.m_Direction, -hit.m_IncomingLightDirection) * dotL;
					break;
				case RenderMode::IRRADIANCE_ONLY:
					finalColor += lights[j]->CalculateLight(hit,wi, pdf) * dotL;
					break;
				case RenderMode::BRDF_ONLY:
					finalColor += hit.m_pMaterial->Shade(hit, -ray.m_Direction, -hit.m_IncomingLightDirection) * dotL;
					break;
				}*/

				FVector3 wi{};
				float pdf{};
				RGBColor tempColor{};
				for (int i{}; i < m_RaysPerPixel; ++i)
				{
					//Area light calculation
					RGBColor shade = hit.m_pMaterial->Shade(hit, -ray.m_Direction, wi);
					RGBColor light = lights[j]->CalculateLight(hit, wi, pdf);
					float dotL = Dot(hit.m_Normal, wi);
					if (dotL < 0.f) continue;
					tempColor += shade * light * dotL / pdf;
				}
				finalColor += tempColor / float(m_RaysPerPixel);
			}
		}
		else
		{
			finalColor = hit.m_pMaterial->GetMaterialColor();
		}
		
		finalColor.MaxToOne();
	}

	m_pBackBufferPixels[c + (r * m_Width)] = SDL_MapRGB(m_pBackBuffer->format,
		static_cast<uint8_t>(finalColor.r * 255.f),
		static_cast<uint8_t>(finalColor.g * 255.f),
		static_cast<uint8_t>(finalColor.b * 255.f));
}
