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

Elite::Renderer::Renderer(SDL_Window * pWindow)
	:m_pWindow{ pWindow }
	, m_pFrontBuffer{ SDL_GetWindowSurface(pWindow) }
	, m_pBackBuffer{}
	, m_pBackBufferPixels{}
	, m_Width{}
	, m_Height{}
{
	//Initialize
	int width, height = 0;
	SDL_GetWindowSize(pWindow, &width, &height);
	m_Width = static_cast<uint32_t>(width);
	m_Height = static_cast<uint32_t>(height);
	m_pBackBuffer = SDL_CreateRGBSurface(0, m_Width, m_Height, 32, 0, 0, 0, 0);
	m_pBackBufferPixels = (uint32_t*)m_pBackBuffer->pixels;
}

void Elite::Renderer::Render()
{
	SDL_LockSurface(m_pBackBuffer);
	const std::vector<Geometry*>& geom = SceneManager::GetInstance().GetCurrentScene()->GetSceneGeometry();
	const std::vector<Light*>& lights = SceneManager::GetInstance().GetCurrentScene()->GetSceneLights();
	PerspectiveCamera* pCam = SceneManager::GetInstance().GetCurrentScene()->GetCurrentCamera();

	Ray ray{}, rayToLight{};
	HitRecord hit{}, hitToLight{};

	FMatrix4 lookat{};
	float fov{};

	if (pCam != nullptr)
	{
		lookat = pCam->GetLookatMatrix();
		ray.m_Origin = pCam->GetPosition();
		fov = pCam->GetFOV();
	}
	bool hitSomething{}, skipLight{};
	
	float aspectRatio = float(m_Width) / m_Height;
	
	FPoint4 pixelPos{};
	RGBColor finalColor{}, tempColor{1.f,1.f,1.f};

	//Loop over all the pixels
	for (uint32_t r = 0; r < m_Height; ++r)
	{
		for (uint32_t c = 0; c < m_Width; ++c)
		{
			pixelPos.x = (2 * ((c + 0.5f) / m_Width) - 1) * aspectRatio * fov;
			pixelPos.y = (1 - 2 * ((r + 0.5f) / m_Height)) * fov;
			pixelPos.z = -1.f;
			pixelPos.w = 1.f;
			pixelPos = lookat * pixelPos;

			ray.m_Direction = FPoint3{ pixelPos.x, pixelPos.y, pixelPos.z } - ray.m_Origin;
			Normalize(ray.m_Direction);

			for (size_t j{}, size{ geom.size() }; j < size; ++j)
			{
				if (geom[j]->Hit(ray, hit))
				{
					hitSomething = true;
				}
			}

			if (hitSomething)
			{
				for (size_t j{}; j < lights.size(); ++j)
				{
					if(!lights[j]->IsEnabled()) continue;

					hitToLight = {};
					skipLight = false;

					lights[j]->GetLightDirection(hit);
					if (!AreEqual(SqrMagnitude(hit.m_IncomingLightDirection), 1.f))
						rayToLight.m_Max = Normalize(hit.m_IncomingLightDirection);
					else rayToLight.m_Max = FLT_MAX;

					rayToLight.m_Direction = -hit.m_IncomingLightDirection;
					rayToLight.m_Origin = hit.m_HitPoint;

					if (InputManager::GetInstance().AreShadowsEnabled())
					{
						hitToLight.m_ShadowCalc = true;

						for (Geometry* g : geom)
						{
							if (g->Hit(rayToLight, hitToLight))
							{
								skipLight = true;
								break;
							}
						}

						if(skipLight) continue;
					}

					float dotL = Dot(hit.m_Normal, -hit.m_IncomingLightDirection);

					if(dotL < 0.f) continue;

					switch (InputManager::GetInstance().GetRenderMode())
					{
					case RenderMode::COMBINED:
						finalColor += lights[j]->CalculateLight(hit.m_HitPoint) * hit.m_pMaterial->Shade(hit, -ray.m_Direction, -hit.m_IncomingLightDirection) * dotL;
						break;
					case RenderMode::IRRADIANCE_ONLY:
						finalColor += lights[j]->CalculateLight(hit.m_HitPoint) * dotL;
						break;
					case RenderMode::BRDF_ONLY:
						finalColor += hit.m_pMaterial->Shade(hit, -ray.m_Direction, -hit.m_IncomingLightDirection) * dotL;
						break;
					}
				}

				finalColor.MaxToOne();
				hitSomething = false;
			}
			//Fill the pixels - pixel access demo
			m_pBackBufferPixels[c + (r * m_Width)] = SDL_MapRGB(m_pBackBuffer->format,
				static_cast<uint8_t>(finalColor.r * 255.f),
				static_cast<uint8_t>(finalColor.g * 255.f),
				static_cast<uint8_t>(finalColor.b * 255.f));

			hit = {};
			finalColor = {};
		}
	}

	SDL_UnlockSurface(m_pBackBuffer);
	SDL_BlitSurface(m_pBackBuffer, 0, m_pFrontBuffer, 0);
	SDL_UpdateWindowSurface(m_pWindow);
}

bool Elite::Renderer::SaveBackbufferToImage() const
{
	return SDL_SaveBMP(m_pBackBuffer, "BackbufferRender.bmp");
}
