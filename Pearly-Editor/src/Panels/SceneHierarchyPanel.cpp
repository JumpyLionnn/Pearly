#include "SceneHierarchyPanel.h"

#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>
#include <imgui/imgui_internal.h>
#include "gui/Widgets.h"
#include "gui/Layout.h"
#include <iostream>

namespace Pearly {

	SceneHierarchyPanel::SceneHierarchyPanel()
	{
		m_ViewMoreIcon = Image::Load("assets/textures/icons/viewMore.png");
	}

	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& scene)
		: m_Context(scene)
	{
		m_ViewMoreIcon = Image::Load("assets/textures/icons/viewMore.png");
	}

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

			float width = ImGui::GetContentRegionAvail().x;
			Widgets::Separator();
			Layout::Space(10.0f);
			Layout::Indent(width * 0.15f);
			bool open = Widgets::Button("Add Component", {width * 0.7f, 0.0f});
			Layout::EndIndent();

			DrawAddComponentPopup(open);
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
		bool opened = Widgets::TreeNode((void*)(uint64)(uint32)entity, flags, tag.Tag);
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

	void SceneHierarchyPanel::DrawAddComponentPopup(bool open)
	{
		if (open)
		{
			ImGui::OpenPopup("AddComponent");
		}

		if (Widgets::BeginModal("AddComponent", true, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove))
		{
			//TODO: improve this system
			float modalWidth = ImGui::GetContentRegionAvail().x;
			static Widgets::TextFilter filter("##Filter", "Search...");
			//filter.
			//Widgets::TextInput("##Search")
			filter.Draw();
			std::vector<std::string> components = { "Camera", "SpriteRenderer" };
			if (ImGui::BeginListBox("##Components"))
			{

				for (int i = 0; i < components.size(); i++)
				{
					if (filter.PassFilter(components[i].c_str()))
					{
						if (ImGui::Selectable(components[i].c_str()))
						{
							std::cout << "selected" << std::endl;
							// using string comparation for now will be removed later
							if (components[i] == "Camera")
							{
								m_Selected.AddComponent<CameraComponent>();
								ImGui::CloseCurrentPopup();
							}
							if (components[i] == "SpriteRenderer")
							{
								m_Selected.AddComponent<SpriteRendererComponent>();
								ImGui::CloseCurrentPopup();
							}
						}
					}

				}
				ImGui::EndListBox();
			}

			Layout::Indent(modalWidth * 0.25f);
			if (Widgets::Button("Cancel", { modalWidth * 0.5f, 0 }))
				ImGui::CloseCurrentPopup();
			Layout::EndIndent();
			ImGui::EndPopup();
		}
	}

	static const ImGuiTreeNodeFlags componentTreeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_FramePadding;

	template<typename T, typename UIFunc>
	void SceneHierarchyPanel::DrawComponent(const std::string& label, Entity entity, UIFunc function)
	{
		if (entity.HasComponent<T>())
		{
			T& component = entity.GetComponent<T>();
			ImVec2 contentRegionAvail = ImGui::GetContentRegionAvail();
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4.0f, 4.0f));
			Widgets::Separator();
			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
			float lineWidth = GImGui->Font->FontSize + GImGui->Style.FramePadding.x * 2.0f;
			bool open = Widgets::TreeNode((void*)typeid(T).hash_code(), componentTreeNodeFlags, label);
			ImGui::PopStyleVar();
			ImGui::SameLine(contentRegionAvail.x - lineHeight * 0.5f - 4.0f);

			bool settingsOpen = Widgets::Button(m_ViewMoreIcon, { lineHeight, lineHeight });
			
			bool removeComponent = false;
			if (ImGui::BeginPopup("ComponentSettings"))
			{
				if (ImGui::MenuItem("Remove Component"))
				{
					removeComponent = true;
				}
				ImGui::EndPopup();
			}
			if (settingsOpen)
			{
				ImGui::OpenPopup("ComponentSettings");
			}
			if (open)
			{
				function(component);
				ImGui::TreePop();
			}
			if (removeComponent)
				entity.RemoveComponent<T>();
		}
	}

	void SceneHierarchyPanel::DrawComponents(Entity entity)
	{
		if (entity.HasComponent<TagComponent>())
		{
			TagComponent& tag = entity.GetComponent<TagComponent>();
			Widgets::TextInput("##Tag", tag.Tag);
		}

		if (entity.HasComponent<TransformComponent>())
		{
			Widgets::Separator();
			if (Widgets::TreeNode((void*)typeid(TransformComponent).hash_code(), componentTreeNodeFlags, "Transform"))
			{
				TransformComponent& transformComponent = entity.GetComponent<TransformComponent>();

				Widgets::Vec2Controls("Positon", transformComponent.Position);

				float fontSize = GImGui->Font->FontSize;
				// taking the font size * 2 for the x and the y width rendered in the position and the scale
				float widthAdditon = fontSize * 2;
				// TODO: find a better layout for those 3 inputs
				Widgets::FloatControls("Rotation", transformComponent.Rotation, 0.1f, widthAdditon);

				Widgets::Vec2Controls("Scale", transformComponent.Scale);

				ImGui::TreePop();
			}

			
		}


		DrawComponent<CameraComponent>("Camera", entity, [](CameraComponent& component)
		{
				Widgets::Checkbox("Primary", component.Primary);

				float size = component.Camera.GetOrthographicSize();
				bool changed = Widgets::FloatControls("Size", size, 0.05f);

				float nearClip = component.Camera.GetOrthographicNearClip();
				changed = Widgets::FloatControls("Near clip", nearClip, 0.1f) || changed;

				float farClip = component.Camera.GetOrthographicFarClip();
				changed = Widgets::FloatControls("Far clip", farClip, 0.1f) || changed;

				if (changed)
					component.Camera.SetOrthographic(size, nearClip, farClip);

				Widgets::Checkbox("Fixed Aspect Ratio", component.FixedAspectRatio);
		});

		DrawComponent<SpriteRendererComponent>("Sprite Renderer", entity, [](SpriteRendererComponent& component) 
		{
				Widgets::ColorEdit4("Color", component.Color);
		});
	}
}