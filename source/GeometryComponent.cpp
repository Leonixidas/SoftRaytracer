#include "pch.h"
#include "GeometryComponent.h"
#include "Geometry.h"

Elite::GeometryComponent::GeometryComponent(GameObject* owner)
	: BaseComponent(owner)
{
}

Elite::GeometryComponent::~GeometryComponent()
{
	for (auto geo : m_Geometry)
		delete geo;

	m_Geometry.clear();
}

void Elite::GeometryComponent::AddGeometry(Geometry* geo)
{
	if (std::find(m_Geometry.begin(), m_Geometry.end(), geo) == m_Geometry.end())
		m_Geometry.push_back(geo);
}

void Elite::GeometryComponent::RemoveGeometry(Geometry* geo)
{
	auto iter = std::find(m_Geometry.begin(), m_Geometry.end(), geo);
	if (iter != m_Geometry.end()) m_Geometry.erase(iter);
}
