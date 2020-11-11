#include "pch.h"
#include "GameObject.h"
#include "BaseComponent.h"

Elite::GameObject::GameObject()
	: m_IsEnabled(true)
{
}

Elite::GameObject::~GameObject()
{
	for (auto o : m_ChildObjects)
		delete o;

	for (auto c : m_Components)
		delete c;

	m_ChildObjects.clear();
	m_Components.clear();
}

void Elite::GameObject::Update()
{
	for (auto comp : m_Components)
		comp->Update();

	for (auto child : m_ChildObjects)
		child->Update();
}

void Elite::GameObject::AddComponent(BaseComponent* comp)
{
	if (std::find(m_Components.begin(), m_Components.end(), comp) == m_Components.end())
		m_Components.push_back(comp);
}

void Elite::GameObject::AddChild(GameObject* object)
{
	if (std::find(m_ChildObjects.begin(), m_ChildObjects.end(), object) == m_ChildObjects.end())
		m_ChildObjects.push_back(object);
}

void Elite::GameObject::RemoveChild(GameObject* object)
{
	auto iter = std::find(m_ChildObjects.begin(), m_ChildObjects.end(), object);
	if (iter != m_ChildObjects.end()) m_ChildObjects.erase(iter);
}
