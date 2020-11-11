#pragma once
#include "SDL.h"
#include "Singleton.h"
#include "RayTracerStructs.h"
#include <vector>

namespace Elite
{
	class InputManager : public Singleton<InputManager>
	{
	public:
		bool HandleInput();
		bool IsKeyPressed(const ScanCode keyCode);
		bool IsKeyReleased(const ScanCode code);

		bool IsMouseButtonPressed(const MouseButton button);

		void GetMouseValues(float& x, float& y);

		bool IsScreenShotTaken() { return m_TakeScreenShot; }

		RenderMode GetRenderMode() { return m_RenderMode; }

		bool AreShadowsEnabled() { return m_ShadowsEnabled; }

	private:
		const Uint8* m_pKeyboardState = nullptr;
		Uint8 m_MouseState{};
		std::vector<SDL_Scancode> m_ReleasedKeys;
		int m_MouseXValue{}, m_MouseYValue{};
		RenderMode m_RenderMode{};
		bool m_TakeScreenShot{}, m_ShadowsEnabled{true};
	};
}
