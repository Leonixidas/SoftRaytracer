#pragma once
#include "BaseComponent.h"
#include <vector>

namespace Elite
{
	class Geometry;

	class GeometryComponent : public BaseComponent
	{
	public:
		GeometryComponent(GameObject* owner);
		~GeometryComponent();

		void Update() override {};

		void AddGeometry(Geometry* geo);
		void RemoveGeometry(Geometry* geo);

		std::vector<Geometry*> GetGeometry() { return m_Geometry; }
	private:
		std::vector<Geometry*> m_Geometry;
	};
}

