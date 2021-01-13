#pragma once
/*=============================================================================*/
// Copyright 2017-2019 Elite Engine
// Authors: Matthieu Delaere
/*=============================================================================*/
// ERenderer.h: class that holds the surface to render to, does traverse the pixels 
// and traces the rays using a tracer
/*=============================================================================*/
#ifndef ELITE_RAYTRACING_RENDERER
#define	ELITE_RAYTRACING_RENDERER

#include <cstdint>
#include "RayTracerEnums.h"
#include <mutex>

struct SDL_Window;
struct SDL_Surface;

namespace Elite
{
	class PerspectiveCamera;
	class Scene;
	struct Ray;
	class Threadpool;

	class Renderer final
	{
	public:
		Renderer(SDL_Window* pWindow);
		~Renderer();

		Renderer(const Renderer&) = delete;
		Renderer(Renderer&&) noexcept = delete;
		Renderer& operator=(const Renderer&) = delete;
		Renderer& operator=(Renderer&&) noexcept = delete;

		void Render();
		bool SaveBackbufferToImage() const;

		void FlipShadowsEnabled();

		void SwitchRenderMode();

		void Trace(uint32_t r, uint32_t c, Scene* scene, Ray ray, FMatrix4 lookat, float aspectRatio, float fov);

	private:
		SDL_Window* m_pWindow;
		SDL_Surface* m_pFrontBuffer;
		SDL_Surface* m_pBackBuffer;
		uint32_t* m_pBackBufferPixels;
		uint32_t m_Width;
		uint32_t m_Height;

		Threadpool* m_Threadpool;

		std::mutex m_BufferMutex;

		RenderMode m_RenderMode;
		bool m_AreShadowsEnabled;
		int m_RaysPerPixel;
	};
}

#endif