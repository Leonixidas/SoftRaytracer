#include "pch.h"
//External includes
#include "vld.h"
#include "SDL.h"
#include "SDL_surface.h"
#undef main

//Standard includes
#include <iostream>

//Project includes
#include "ETimer.h"
#include "ERenderer.h"
#include "SceneManager.h"
#include "InputManager.h"
#include "RayTraceScene.h"
#include "BunnyScene.h"

void ShutDown(SDL_Window* pWindow)
{
	SDL_DestroyWindow(pWindow);
	SDL_Quit();
}

int main(int argc, char* args[])
{
	//Unreferenced parameters
	(void)argc;
	(void)args;

	//Create window + surfaces
	SDL_Init(SDL_INIT_VIDEO);

	const uint32_t width = 640;
	const uint32_t height = 480;
	SDL_Window* pWindow = SDL_CreateWindow(
		"RayTracer - **Leander De Hertogh**",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		width, height, 0);

	if (!pWindow)
		return 1;

	//Initialize "framework"
	Elite::Timer& pTimer = Elite::Timer::GetInstance();
	Elite::Renderer* pRenderer = new Elite::Renderer(pWindow);
	Elite::SceneManager& sceneManager = Elite::SceneManager::GetInstance();
	sceneManager.SetRenderer(pRenderer);
	Elite::InputManager& inputManager = Elite::InputManager::GetInstance();
	sceneManager.AddScene(new Elite::RayTraceScene{ "RaytracerScene"});
	sceneManager.AddScene(new Elite::BunnyScene{ "BunnyScene"});

	std::cout << "Both scenes are initialized while initializing,\n";
	std::cout << "check memory leaks by running the solution in debug,\nboth scenes will be initialized and the first scene will be loaded which won't take that long\n\n";
	std::cout << "\n----Lights Controls----\n";
	std::cout << "'K': Left light\n";
	std::cout << "'L': Right light\n";
	std::cout << "'O': Light in the back\n";
	std::cout << "'J': Directional light\n";

	std::cout << "\n----Scene Controls----\n";
	std::cout << "WASD-keys for horizontal movement\n";
	std::cout << "Hold the right mouse button and move to rotate the camera\n";
	std::cout << "Hold the right and left mouse button to move up and down\n";
	std::cout << "Hold the left mouse button to move forward and look left and right\n";
	std::cout << "'1': Previous scene\n";
	std::cout << "'2': Next scene\n";
	std::cout << "'T': Change render mode\n";
	std::cout << "'Z': Enable/Disable shadows\n";
	std::cout << "'X': Take a screenshot\n";

	{
		//Start loop
		pTimer.Start();
		sceneManager.Initialize();
		float printTimer = 0.f;
		bool isLooping = true;
		bool takeScreenshot = false;

		

		while (isLooping)
		{
			//--------- Get input events ---------
			isLooping = !inputManager.HandleInput();
			takeScreenshot = inputManager.IsScreenShotTaken();

			sceneManager.Update();

			//--------- Render ---------
			pRenderer->Render();

			//--------- Timer ---------
			pTimer.Update();
			printTimer += pTimer.GetElapsed();
			if (printTimer >= 1.f)
			{
				printTimer = 0.f;
				std::cout << "FPS: " << pTimer.GetFPS() << std::endl;
			}

			//Save screenshot after full render
			if (takeScreenshot)
			{
				if (!pRenderer->SaveBackbufferToImage())
					std::cout << "Screenshot saved!" << std::endl;
				else
					std::cout << "Something went wrong. Screenshot not saved!" << std::endl;
				takeScreenshot = false;
			}
		}
		pTimer.Stop();
		sceneManager.Close();
	}

	//Shutdown "framework"
	delete pRenderer;

	ShutDown(pWindow);
	return 0;
}