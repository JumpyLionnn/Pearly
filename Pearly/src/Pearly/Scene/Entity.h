#pragma once
#include <entt.hpp>
#include "Scene.h"

namespace Pearly {

	class Entity
	{
	public:
		Entity() = default;
		Entity(entt::entity entityHandle, Scene* scene);
		Entity(const Entity& other) = default;

		template<typename T, typename ... Args>
		T& AddComponent(Args&& ... args)
		{
			PR_CORE_ASSERT(!HasComponent<T>(), "This entity already has this component!");
			T& component = m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
			m_Scene->OnComponentAdded<T>(*this, component);
			return component;
		}

		template<typename T>
		bool HasComponent() const
		{
			return m_Scene->m_Registry.any_of<T>(m_EntityHandle);
		}

		template<typename T>
		T& GetComponent() const
		{
			PR_CORE_ASSERT(HasComponent<T>(), "This entity does not have this component!");
			return m_Scene->m_Registry.get<T>(m_EntityHandle);
		}

		template<typename T>
		void RemoveComponent()
		{
			PR_CORE_ASSERT(HasComponent<T>(), "This entity does not have this component!");
			m_Scene->m_Registry.remove<T>(m_EntityHandle);
		}

		bool operator ==(const Entity& other) const { return m_EntityHandle == other.m_EntityHandle && m_Scene == other.m_Scene; }
		bool operator !=(const Entity& other) const { return m_EntityHandle != other.m_EntityHandle || m_Scene != other.m_Scene; }
		operator bool() const { return m_EntityHandle != entt::null; }
		operator uint32() const { return (uint32)m_EntityHandle; }
		operator entt::entity() const { return m_EntityHandle; }
	private:
		entt::entity m_EntityHandle = entt::null;
		Scene* m_Scene = nullptr;
	};

}