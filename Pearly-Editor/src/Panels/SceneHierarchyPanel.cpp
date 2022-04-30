#include "SceneHierarchyPanel.h"

#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>
#include <imgui/imgui_internal.h>

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
			Entity entity { entityID, m_Context.get() };
			DrawEntityNode(entity);
		});

		if (ImGui::IsMouseDown(ImGuiMouseButton_Left) && ImGui::IsWindowHovered())
		{
			m_Selected = {};
		}

		ImGui::End();

		ImGui::Begin("Inspector");
		if (m_Selected)
		{
			DrawComponents(m_Selected);
		}
		ImGui::End();
	}

	void SceneHierarchyPanel::DrawEntityNode(const Entity& entity)
	{
		TagComponent& tag = entity.GetComponent<TagComponent>();
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

	static void DrawVec2Controls(const std::string& label, glm::vec2& values, float speed = 0.01f, float columnWidth = 100.0f)
	{
		ImGui::PushID(label.c_str());
		ImGui::Columns(2, "", false);

		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());

		ImGui::NextColumn();
			
		ImGui::PushMultiItemsWidths(2, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(5.0f, 3.0f));

		ImGui::Text("X");
		ImGui::SameLine();
		ImGui::DragFloat("##X", &values.x, speed);
		ImGui::PopItemWidth();

		ImGui::SameLine();

		ImGui::Text("Y");
		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values.y, speed);
		ImGui::PopItemWidth();
		
		ImGui::PopStyleVar();
		ImGui::Columns(1);
		ImGui::PopID();
	}

	static void DrawFloatControls(const std::string& label, float& value, float speed = 0.1f, float componentWidthAddition = 0.0f, float labelWidth = 100.0f)
	{
		ImGui::PushID(label.c_str());
		ImGui::Columns(2, "", false);

		ImGui::SetColumnWidth(0, labelWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();
		
		ImGui::SetNextItemWidth(ImGui::CalcItemWidth() + componentWidthAddition);
		ImGui::DragFloat("##Float", &value, speed);

		ImGui::Columns(1);
		ImGui::PopID();
	}

	void SceneHierarchyPanel::DrawComponents(const Entity& entity)
	{
		if (entity.HasComponent<TagComponent>())
		{
			TagComponent& tag = entity.GetComponent<TagComponent>();
			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), tag.Tag.c_str());
			if (ImGui::InputText("Tag", buffer, sizeof(buffer)))
			{
				tag.Tag = std::string(buffer);
			}
		}

		if (entity.HasComponent<TransformComponent>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Transform"))
			{
				TransformComponent& transformComponent = entity.GetComponent<TransformComponent>();

				DrawVec2Controls("Positon", transformComponent.Position);

				float fontSize = GImGui->Font->FontSize;
				// taking the font size * 2 for the x and the y width rendered in the position and the scale
				float widthAdditon = fontSize * 2;
				DrawFloatControls("Rotation", transformComponent.Rotation, 0.1f, widthAdditon);

				DrawVec2Controls("Scale", transformComponent.Scale);

				ImGui::TreePop();
			}
		}

		if (entity.HasComponent<CameraComponent>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(CameraComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Camera"))
			{
				CameraComponent& camera = entity.GetComponent<CameraComponent>();

				ImGui::Checkbox("Primary", &camera.Primary);

				float size = camera.Camera.GetOrthographicSize();
				bool changed = ImGui::DragFloat("Size", &size, 0.05f);

				float nearClip = camera.Camera.GetOrthographicNearClip();
				changed = ImGui::DragFloat("Near clip", &nearClip, 0.1f) || changed;

				float farClip = camera.Camera.GetOrthographicFarClip();
				changed = ImGui::DragFloat("Far clip", &farClip, 0.1f) || changed;

				if(changed)
					camera.Camera.SetOrthographic(size, nearClip, farClip);

				ImGui::Checkbox("Fixed aspect ratio", &camera.FixedAspectRatio);

				ImGui::TreePop();
			}
		}

		if (entity.HasComponent<SpriteRendererComponent>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(SpriteRendererComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Sprite Renderer"))
			{
				SpriteRendererComponent& spriteRenderer = entity.GetComponent<SpriteRendererComponent>();
				ImGui::ColorEdit4("Color", glm::value_ptr(spriteRenderer.Color));
				ImGui::TreePop();
			}
		}
	}
}