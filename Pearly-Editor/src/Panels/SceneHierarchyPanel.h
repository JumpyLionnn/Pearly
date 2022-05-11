#pragma once
#include <Pearly.h>
#include "gui/Image.h"

namespace Pearly {

	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel();
		SceneHierarchyPanel(const Ref<Scene>& scene);

		void SetContext(const Ref<Scene>& scene);

		Entity GetSelectedEntity() const;
		void SetSelectedEntity(Entity entity);

		void OnRender();
	private:
		void DrawEntityNode(const Entity& entity);
		void DrawComponents(Entity entity);
		void DrawAddComponentPopup(bool open);


		template<typename T, typename UIFunc>
		void DrawComponent(const std::string& label, Entity entity, UIFunc function);
	private:
		Ref<Scene> m_Context;
		Entity m_Selected;
		Ref<Image> m_ViewMoreIcon;
	};
}