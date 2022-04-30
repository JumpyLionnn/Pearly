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

		if (ImGui::BeginPopupContextWindow(0, 1, false))
		{
			if (ImGui::MenuItem("Create Empty"))
			{
				m_Context->CreateEntity("Empty Entity");
			}
			ImGui::EndPopup();
		}

		ImGui::End();

		ImGui::Begin("Inspector");
		if (m_Selected)
		{
			DrawComponents(m_Selected);

			if (ImGui::Button("Add Component"))
			{
				ImGui::OpenPopup("AddComponent");
			}

			if (ImGui::BeginPopup("AddComponent"))
			{
				if (ImGui::MenuItem("Camera"))
				{
					m_Selected.AddComponent<CameraComponent>();
					ImGui::CloseCurrentPopup();
				}

				if (ImGui::MenuItem("Sprite Renderer"))
				{
					m_Selected.AddComponent<SpriteRendererComponent>();
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}
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
		bool removeComponent = false;
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Delete"))
				removeComponent = true;
			ImGui::EndPopup();
		}

		if (opened)
		{
			ImGui::TreePop();
		}

		if (removeComponent)
		{
			if (m_Selected == entity)
				m_Selected = {};
			m_Context->DestroyEntity(entity);
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

	void SceneHierarchyPanel::DrawComponents(Entity entity)
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

		const ImGuiTreeNodeFlags treeNodesFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap;

		if (entity.HasComponent<TransformComponent>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), treeNodesFlags, "Transform"))
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
			bool open = ImGui::TreeNodeEx((void*)typeid(CameraComponent).hash_code(), treeNodesFlags, "Camera");
			ImGui::SameLine();
			if (ImGui::Button("+"))
			{
				ImGui::OpenPopup("ComponentSettings");
			}
			bool removeComponent = false;
			if (ImGui::BeginPopup("ComponentSettings"))
			{
				if (ImGui::MenuItem("Remove Component"))
				{
					removeComponent = true;
				}
				ImGui::EndPopup();
			}
			if (open)
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
			if (removeComponent)
				entity.RemoveComponent<CameraComponent>();
		}

		if (entity.HasComponent<SpriteRendererComponent>())
		{
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4.0f, 4.0f));
			bool open = ImGui::TreeNodeEx((void*)typeid(SpriteRendererComponent).hash_code(), treeNodesFlags, "Sprite Renderer");
			ImGui::SameLine(ImGui::GetWindowWidth() - 25.0f);
			if (ImGui::Button("+", ImVec2(20.0f, 20.0f)))
			{
				ImGui::OpenPopup("ComponentSettings");
			}
			ImGui::PopStyleVar();
			bool removeComponent = false;
			if (ImGui::BeginPopup("ComponentSettings"))
			{
				if (ImGui::MenuItem("Remove Component"))
				{
					removeComponent = true;
				}
				ImGui::EndPopup();
			}
			if (open)
			{
				SpriteRendererComponent& spriteRenderer = entity.GetComponent<SpriteRendererComponent>();
				ImGui::ColorEdit4("Color", glm::value_ptr(spriteRenderer.Color));
				ImGui::TreePop();
			}
			if (removeComponent)
				entity.RemoveComponent<SpriteRendererComponent>();
		}
	}
}