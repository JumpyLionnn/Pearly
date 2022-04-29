#include "SceneHierarchyPanel.h"

#include <imgui/imgui.h>

namespace Pearly {


	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& scene)
		: m_Context(scene)
	{}

	void SceneHierarchyPanel::SetContext(const Ref<Scene>& scene)
	{
		m_Context = scene;
	}

	void SceneHierarchyPanel::OnRender()
	{
		ImGui::Begin("Hierarchy");
		m_Context->m_Registry.each([&](auto entityID) 
		{
			PR_CORE_INFO("entity");
			Entity entity { entityID, m_Context.get() };
			DrawEntityNode(entity);
		});
		ImGui::End();
	}

	void SceneHierarchyPanel::DrawEntityNode(const Entity& entity)
	{
		TagComponent& tag = entity.GetComponent<TagComponent>();
		//ImGui::Text("%s", tag.Tag.c_str());
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
		if (entity == m_Selected)
			flags |= ImGuiTreeNodeFlags_Selected;
		// if the entity doesnt have childrent it cannt be opened
		//if (entity.Children.Empty()) add this back when entities can have children
			flags |= ImGuiTreeNodeFlags_Leaf;
		bool opened = ImGui::TreeNodeEx((void*)(uint64)(uint32)entity, flags, tag.Tag.c_str());
		if (ImGui::IsItemClicked())
		{
			m_Selected = entity;
		}

		if (opened)
		{
			ImGui::TreePop();
		}
	}
}