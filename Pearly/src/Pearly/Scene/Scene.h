#pragma once
#include "Pearly/Core/Timestep.h"
#include <entt.hpp>

namespace Pearly {

	class Entity;
	class SceneHierarchyPanel;

	class Scene
	{
		friend class Entity;
		friend class SceneHierarchyPanel;
	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& name = "Entity");

		void OnUpdate(Timestep ts);
		void OnViewportResize(uint32 width, uint32 height);
	private:
		entt::registry m_Registry;
		uint32 m_ViewportWidth;
		uint32 m_ViewportHeight;
	};

}