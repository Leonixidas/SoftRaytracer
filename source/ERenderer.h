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
#include <vector>
#include "Light.h"
#include "Geometry.h"

struct SDL_Window;
struct SDL_Surface;

namespace Elite
{
	class PerspectiveCamera;
	class Threadpool;

	class Renderer final
	{
	public:
		Renderer(SDL_Window* pWindow);
		~Renderer() = default;

		Renderer(const Renderer&) = delete;
		Renderer(Renderer&&) noexcept = delete;
		Renderer& operator=(const Renderer&) = delete;
		Renderer& operator=(Renderer&&) noexcept = delete;

		void Render();
		bool SaveBackbufferToImage() const;

	private:
		SDL_Window* m_pWindow;
		SDL_Surface* m_pFrontBuffer;
		SDL_Surface* m_pBackBuffer;
		uint32_t* m_pBackBufferPixels;
		uint32_t m_Width;
		uint32_t m_Height;
		Threadpool* m_Threadpool;
	};
}

#endif