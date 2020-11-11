#include "pch.h"
#include "Scene.h"
#include "GameObject.h"
#include "PerspectiveCamera.h"
#include "Light.h"
#include "Geometry.h"

Elite::Scene::Scene(const std::string& sceneName)
	: m_SceneName(sceneName)
	, m_IsInitialized(false)
	, m_pCurrentCamera(nullptr)
{
}

Elite::Scene::~Scene()
{
	for (GameObject* o : m_Objects)
		delete o;

	for (Geometry* geo : m_Geometry)
		delete geo;

	for (Light* l : m_Lights)
		delete l;

	for (PerspectiveCamera* p : m_Cameras)
		delete p;

	m_Objects.clear();
	m_Geometry.clear();
	m_Lights.clear();
	m_Cameras.clear();
}

void Elite::Scene::AddObject(GameObject* object)
{
	if (std::find(m_Objects.begin(), m_Objects.end(), object) == m_Objects.end())
	{
		m_Objects.push_back(object);
	}
}

void Elite::Scene::RemoveObject(GameObject* object)
{
	auto iter = std::find(m_Objects.begin(), m_Objects.end(), object);
	if ( iter != m_Objects.end())
	{
		m_Objects.erase(iter);
	}
}

void Elite::Scene::AddGeometry(Geometry* geo)
{
	if (std::find(m_Geometry.begin(), m_Geometry.end(), geo) == m_Geometry.end())
		m_Geometry.push_back(geo);
}

void Elite::Scene::RemoveGeometry(Geometry* geo)
{
	auto iter = std::find(m_Geometry.begin(), m_Geometry.end(), geo);
	if (iter != m_Geometry.end())
	{
		m_Geometry.erase(iter);
	}
}

void Elite::Scene::AddCamera(PerspectiveCamera* camera)
{
	if (m_Cameras.size() == 0) 
		m_pCurrentCamera = camera;

	if (std::find(m_Cameras.begin(), m_Cameras.end(), camera) == m_Cameras.end())
		m_Cameras.push_back(camera);

}

void Elite::Scene::RemoveCamera(PerspectiveCamera* camera)
{
	auto iter = std::find(m_Cameras.begin(), m_Cameras.end(), camera);
	if (iter != m_Cameras.end())
	{
		m_Cameras.erase(iter);
	}
}

bool Elite::Scene::SelectCamera(unsigned int id)
{
	if (id >= m_Cameras.size()) return false;

	m_pCurrentCamera->Disable();
	m_pCurrentCamera = m_Cameras[id];
	m_pCurrentCamera->Enable();
	return true;
}

void Elite::Scene::AddLight(Light* light)
{
	if (std::find(m_Lights.begin(), m_Lights.end(), light) == m_Lights.end())
		m_Lights.push_back(light);
}

void Elite::Scene::RootInitialize()
{
	Initialize();
	m_IsInitialized = true;
}

void Elite::Scene::RootUpdate()
{
	Update();
	for (auto obj : m_Objects)
		obj->Update();
}
