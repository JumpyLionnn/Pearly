#pragma once
#include "Pearly/Core/Timestep.h"
#include <entt.hpp>

namespace Pearly {

	class Entity;

	class Scene
	{
		friend class Entity;
		friend class SceneSerializer;
		friend class SceneHierarchyPanel;
	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& name = "Entity");
		void DestroyEntity(Entity entity);

		void OnUpdate(Timestep ts);
		void OnViewportResize(uint32 width, uint32 height);

	private:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);
	private:
		entt::registry m_Registry;
		uint32 m_ViewportWidth;
		uint32 m_ViewportHeight;
	};

}