#pragma once
#include <string>
#include <vector>

namespace Elite
{
	class GameObject;
	class Geometry;
	class PerspectiveCamera;
	class Light;

	class Scene
	{
	public:
		explicit Scene(const std::string& sceneName);
		virtual ~Scene();

		void AddObject(GameObject* object);
		void RemoveObject(GameObject* object);

		void AddGeometry(Geometry* geo);
		void RemoveGeometry(Geometry* geo);

		void AddCamera(PerspectiveCamera* camera);
		void RemoveCamera(PerspectiveCamera* camera);
		bool SelectCamera(unsigned int id);

		void AddLight(Light* light);

		PerspectiveCamera* GetCurrentCamera() { return m_pCurrentCamera; }

		const std::vector<GameObject*>& GetSceneObjects() { return m_Objects; }

		const std::vector<Geometry*>& GetSceneGeometry() { return m_Geometry; }

		const std::vector<Light*>& GetSceneLights() { return m_Lights; }

		void RootInitialize();
		void RootUpdate();

		virtual void Initialize() = 0;
		virtual void Update() = 0;

	protected:
		PerspectiveCamera* m_pCurrentCamera;

	private:
		std::vector<GameObject*> m_Objects;
		std::vector<Geometry*> m_Geometry;
		std::string m_SceneName;
		std::vector<PerspectiveCamera*> m_Cameras;
		std::vector<Light*> m_Lights;
		bool m_IsInitialized;
	};
}
