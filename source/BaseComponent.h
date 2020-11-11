#pragma once
namespace Elite
{
	class GameObject;

	class BaseComponent
	{
	public:
		explicit BaseComponent(GameObject* owner)
			: m_Owner(owner)
		{}

		virtual ~BaseComponent() = default;

		virtual void Update() = 0;

	protected:
		GameObject* m_Owner;
	};
}

