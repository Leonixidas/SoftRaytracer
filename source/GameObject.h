#pragma once
#include <vector>
#include <functional>

namespace Elite
{
	class BaseComponent;

	class GameObject
	{
	public:
		GameObject();
		~GameObject();

		void Update();
		void AddComponent(BaseComponent* comp);

		void AddChild(GameObject* object);
		void RemoveChild(GameObject* object);

		void Enable() { m_IsEnabled = true; }
		void Disable() { m_IsEnabled = false; }

		bool IsEnabled() { return m_IsEnabled; }

		template<typename T>
		T* GetComponent();

		template<typename T>
		bool RemoveComponent();

	private:
		std::vector<GameObject*> m_ChildObjects;
		std::vector<BaseComponent*> m_Components;

		bool m_IsEnabled;
	};

	
}

template<class T>
T* Elite::GameObject::GetComponent()
{
	//const type_info& ti = typeid(T);
	for (BaseComponent* c : m_Components)
	{
		if (typeid(*c) == typeid(T))
		{
			return static_cast<T*>(c);
		}
	}

	return nullptr;
}

//returns if the removing of the component was successful
template<class T>
bool Elite::GameObject::RemoveComponent()
{
	for (auto c : m_Components)
	{
		if (typeid(c) == typeid(T))
		{
			m_Components.erase(std::remove(m_Components.begin(), m_Components.end(), c));
			return true;
		}
	}
	return false;
}

