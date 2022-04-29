#pragma once
#include <Pearly.h>

namespace Pearly {

	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<Scene>& scene);

		void SetContext(const Ref<Scene>& scene);

		void OnRender();
	private:
		void DrawEntityNode(const Entity& entity);
	private:
		Ref<Scene> m_Context;
		Entity m_Selected;
	};
}