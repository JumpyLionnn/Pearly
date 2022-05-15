#pragma once
#include "Pearly/Core/Timestep.h"
#include "Pearly/Events/Event.h"
#include <entt.hpp>

namespace Pearly {

	class Entity;

	class Scene
	{
		friend class Entity;
		friend class SceneSerializer;
		friend class SceneHierarchyPanel;
	public:
		Scene() = default;
		Scene(const Scene& scene);
		~Scene();

		Entity CreateEntity(const std::string& name = "Entity");
		void DestroyEntity(Entity entity);

		virtual void OnUpdate(Timestep ts);
		virtual void OnEvent(Event& event) {};
		virtual void OnViewportResize(uint32 width, uint32 height);

		Entity GetPrimaryCameraEntity();

	protected:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);
	protected:
		entt::registry m_Registry;
		uint32 m_ViewportWidth = 0;
		uint32 m_ViewportHeight = 0;
	};

}