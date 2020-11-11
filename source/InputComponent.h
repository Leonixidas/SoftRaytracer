#pragma once
#include "BaseComponent.h"

namespace Elite
{
	class InputComponent : public BaseComponent
	{
		InputComponent(GameObject* owner);
		~InputComponent() = default;

		void AddInputAction() {};
	};
}

