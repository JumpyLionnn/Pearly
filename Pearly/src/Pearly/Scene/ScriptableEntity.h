#pragma once

#include "Entity.h"

namespace Pearly {

	class ScriptableEntity
	{
		friend class Scene;
	public:
		virtual ~ScriptableEntity() = default;
	protected:
		template<typename T>
		T& GetComponent()
		{
			return m_Entity.GetComponent<T>();
		}

		virtual void OnStart() {}
		virtual void OnUpdate(Timestep ts) {}
		virtual void OnDestroy() {}
	private:
		Entity m_Entity;
	};
}