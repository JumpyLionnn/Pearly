#include "prpch.h"
#include "Entity.h"

namespace Pearly {

	Entity::Entity(entt::entity entityHandle, Scene* scene)
		: m_EntityHandle(entityHandle), m_Scene(scene)
	{}
}