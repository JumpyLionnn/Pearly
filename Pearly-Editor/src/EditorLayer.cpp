#include "EditorLayer.h"
#include <iostream>
#include <glm/gtc/type_ptr.hpp>


namespace Pearly {

	class Controller : public ScriptableEntity
	{
	public:
		void OnUpdate(Timestep ts)
		{
			TransformComponent& transformComponent = GetComponent<TransformComponent>();

			if (Input::IsKeyPressed(PR_KEY_A))
				transformComponent.Position.x -= m_MovmentSpeed * ts;
			if (Input::IsKeyPressed(PR_KEY_D))
				transformComponent.Position.x += m_MovmentSpeed * ts;
			if (Input::IsKeyPressed(PR_KEY_W))
				transformComponent.Position.y += m_MovmentSpeed * ts;
			if (Input::IsKeyPressed(PR_KEY_S))
				transformComponent.Position.y -= m_MovmentSpeed * ts;
		}
	private:
		float m_MovmentSpeed = 5.0f;
	};

	EditorLayer::EditorLayer()
		: Layer("EditorLayer"), m_ViewportBounds(), m_CameraController(1280.0f / 720.0f), m_SpriteSheet("assets/textures/tiles.png", { 12, 12 })
	{}

	void EditorLayer::OnAttach()
	{
		PR_PROFILE_FUNCTION();

		ImGuiIO& io = ImGui::GetIO();
		io.Fonts->AddFontFromFileTTF("assets/fonts/opensans/static/OpenSans/OpenSans-Bold.ttf", 18.0f);
		io.FontDefault = io.Fonts->AddFontFromFileTTF("assets/fonts/opensans/static/OpenSans/OpenSans-Regular.ttf", 18.0f);

		m_CheckerboardTexture = Texture2D::Create("assets/textures/Checkerboard.png");
		m_LilyPadTexture = m_SpriteSheet.CreateSubTexture({ 13, 20 });
		m_BigTreeTexture = m_SpriteSheet.CreateSubTexture({ 1, 20 }, { 4, 4 });

		FrameBufferSpecification frameBufferSpec;
		frameBufferSpec.Width = 1280;
		frameBufferSpec.Height = 720;
		frameBufferSpec.Attachments = { FrameBufferTextureFormat::RGBA8, FrameBufferTextureFormat::RED_INTEGER, FrameBufferTextureFormat::Depth };
		m_FrameBuffer = FrameBuffer::Create(frameBufferSpec);

		NewScene();

		// setting the theme
		auto& style = ImGui::GetStyle();
		auto& colors = style.Colors;
		colors[ImGuiCol_WindowBg] = ImVec4{ 0.1f, 0.105f, 0.11f, 1.0f };

		// Headers
		colors[ImGuiCol_Header] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
		colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
		colors[ImGuiCol_HeaderActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

		// Buttons
		colors[ImGuiCol_Button] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
		colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
		colors[ImGuiCol_ButtonActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

		// Frame BG
		colors[ImGuiCol_FrameBg] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
		colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
		colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

		// Tabs
		colors[ImGuiCol_Tab] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TabHovered] = ImVec4{ 0.38f, 0.3805f, 0.381f, 1.0f };
		colors[ImGuiCol_TabActive] = ImVec4{ 0.28f, 0.2805f, 0.281f, 1.0f };
		colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };

		// Title
		colors[ImGuiCol_TitleBg] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

		style.WindowMenuButtonPosition = ImGuiDir_None;
		style.TabRounding = 6.0f;
		style.IndentSpacing = 10.0f;
		style.TabBorderSize = 1.0f;
	}

	void EditorLayer::OnDetach()
	{
		PR_PROFILE_FUNCTION();
	}

	void EditorLayer::OnUpdate(Timestep ts)
	{
		PR_PROFILE_FUNCTION();

		// Update
		if(m_ViewportFocused)
			m_CameraController.OnUpdate(ts);

		// Render
		Renderer::ResetStats();

		// Resize
		FrameBufferSpecification spec = m_FrameBuffer->GetSpecification();
		if (m_ViewportSize.x > 0 && m_ViewportSize.y > 0 &&
			(spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
		{
			m_FrameBuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_CameraController.OnResize(m_ViewportSize.x, m_ViewportSize.y);

			m_ActiveScene->OnViewportResize((uint32)m_ViewportSize.x, (uint32)m_ViewportSize.y);
		}

		m_FrameBuffer->Bind();
		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		RenderCommand::Clear();
		
		/*
		Renderer::BeginScene(m_CameraController.GetCamera());
		Renderer::DrawQuad({ { -1.0f, 1.0f }, { 4.0f, 4.0f } }, m_BigTreeTexture);
		Renderer::DrawQuad({ { 1.0f, 0.0f, 0.1f }, { 1.0f, 1.0f } }, m_LilyPadTexture);
		Renderer::EndScene();
		*/

		m_FrameBuffer->ClearColorAtachment(1, -1);
		
		m_ActiveScene->OnUpdate(ts);

		m_FrameBuffer->Unbind();
		
	}

	void EditorLayer::OnImGuiRender()
	{
		PR_PROFILE_FUNCTION();

		ImGuizmo::BeginFrame();

		static bool dockspaceOpen = true;
		static bool opt_fullscreen_persistant = true;
		bool opt_fullscreen = opt_fullscreen_persistant;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive, 
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise 
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
		ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// DockSpace
		ImGuiIO& io = ImGui::GetIO();
		ImGuiStyle& style = ImGui::GetStyle();
		float windowMinSize = style.WindowMinSize.x;
		style.WindowMinSize.x = 250.0f;
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}
		style.WindowMinSize.x = windowMinSize;
		static bool showAppRendererStats = false;
		if (showAppRendererStats)
		{
			ImGui::Begin("Renderer Stats", &showAppRendererStats);

			Renderer::Statistics stats = Renderer::GetStats();
			ImGui::Text("Draw Calls: %d", stats.DrawCalls);
			ImGui::Text("Quads: %d", stats.QuadCount);
			ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
			ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

			ImGui::End();
		}
		static bool showAppStyleEditor = false;
		if (showAppStyleEditor)
		{
			ImGui::Begin("Style Editor", &showAppStyleEditor);
			ImGui::ShowStyleEditor();
			ImGui::End();
		}

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("New", "Ctrl+N")) NewScene();
				if (ImGui::MenuItem("Open...", "Ctrl+O")) OpenScene();
				if (ImGui::MenuItem("Save", "Ctrl+S")) SaveScene();
				if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S")) SaveAsScene();

				ImGui::Separator();

				if (ImGui::MenuItem("Exit")) Application::Get().Close();
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Debug Tools"))
			{
				ImGui::MenuItem("Renderer Stats", NULL, &showAppRendererStats);
				ImGui::MenuItem("Style Editor", NULL, &showAppStyleEditor);
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}
		
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0,0));
		ImGui::Begin("Viewport");

		auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
		auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
		auto viewportOffset = ImGui::GetWindowPos();
		m_ViewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
		m_ViewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };


		m_ViewportFocused = ImGui::IsWindowFocused();
		m_ViewportHovered = ImGui::IsWindowHovered();
		Application::Get().GetImGuiLayer()->SetBlockEvents(!m_ViewportFocused && !m_ViewportHovered);

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
		ImGui::Image((void*)(uint64)m_FrameBuffer->GetColorAttachmentRendererID(), { m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
		
		Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();
		if (selectedEntity)
		{
			ImGuizmo::SetOrthographic(true);
			ImGuizmo::SetDrawlist();
			ImVec2 windowPosition = ImGui::GetWindowPos();
			float windowWidth = ImGui::GetWindowWidth();
			float windowHeight = ImGui::GetWindowHeight();
			ImGuizmo::SetRect(m_ViewportBounds[0].x, m_ViewportBounds[0].y, m_ViewportBounds[1].x - m_ViewportBounds[0].x, m_ViewportBounds[1].y - m_ViewportBounds[0].y);

			const EditorCamera& camera = m_ActiveScene->GetCamera();
			const glm::mat4& cameraProjection = camera.GetProjection();
			glm::mat4 cameraView = camera.GetView();

			TransformComponent& entityTransform = selectedEntity.GetComponent<TransformComponent>();
			glm::mat4 transform = entityTransform.GetTransform();
			float originalRotation = entityTransform.Rotation;

			ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection), (ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::MODE::LOCAL, glm::value_ptr(transform));
			
			if (ImGuizmo::IsUsing())
			{
				glm::vec3 position;
				float rotation;
				glm::vec2 scale;
				Math::DecomposeTransform(transform, position, rotation, scale);
				entityTransform.Position = glm::vec2(position);
				entityTransform.Rotation = glm::degrees(rotation);
				entityTransform.Scale = glm::vec2(scale);
			}
		}
		

		ImGui::End();
		ImGui::PopStyleVar();

		m_SceneHierarchyPanel.OnRender();

		ImGui::End();
	}

	void EditorLayer::OnEvent(Event& e)
	{
		m_CameraController.OnEvent(e);
		m_ActiveScene->OnEvent(e);

		EventDispacher dispacher(e);
		dispacher.Dispatch<KeyPressedEvent>(PR_BIND_EVENT_FN(EditorLayer::OnKeyPressed));
		dispacher.Dispatch<MouseButtonPressedEvent>(PR_BIND_EVENT_FN(EditorLayer::OnMouseButtonPressed));
	}

	bool EditorLayer::OnKeyPressed(KeyPressedEvent& event)
	{
		if (event.GetRepeatCount() == 0)
		{
			bool control = Input::IsKeyPressed(PR_KEY_LEFT_CONTROL) || Input::IsKeyPressed(PR_KEY_RIGHT_CONTROL);
			bool shift = Input::IsKeyPressed(PR_KEY_LEFT_SHIFT) || Input::IsKeyPressed(PR_KEY_RIGHT_SHIFT);
			switch (event.GetKeyCode())
			{
				case PR_KEY_N:
				{
					if (control)
					{
						NewScene();
					}
					return true;
				}
				case PR_KEY_O:
				{
					if (control)
					{
						OpenScene();
					}
					return true;
				}
				case PR_KEY_S:
				{
					if (control)
					{
						SaveScene();
					}
					if (control && shift)
					{
						SaveAsScene();
					}
					return true;
				}

				case PR_KEY_W:
					m_GizmoType = GizmoOperation::Translation;
					return true;
				case PR_KEY_E:
					m_GizmoType = GizmoOperation::Rotation;
					return true;
				case PR_KEY_R:
					m_GizmoType = GizmoOperation::Scale;
					return true;
			}
			return false;
		}
		else
		{
			return false;
		}
	}

	bool EditorLayer::OnMouseButtonPressed(MouseButtonPressedEvent& event)
	{
		// mouse picking
		if (event.GetMouseButton() == PR_MOUSE_BUTTON_LEFT)
		{
			if (m_ViewportHovered && !ImGuizmo::IsOver())
			{
				auto [mx, my] = ImGui::GetMousePos();
				mx -= m_ViewportBounds[0].x;
				my -= m_ViewportBounds[0].y;
				glm::vec2 viewportSize = m_ViewportBounds[1] - m_ViewportBounds[0];
				my = viewportSize.y - my;

				int mouseX = (int)mx;
				int mouseY = (int)my;

				/*
				if (mouseX >= 0 && mouseY >= 0 && mouseX < (int)viewportSize.x && mouseY < (int)viewportSize.y)
				{
					int pixelData = m_FrameBuffer->ReadPixel(1, mouseX, mouseY);
					PR_CORE_INFO("pixel data = {0}", pixelData);
				}
				*/
				m_FrameBuffer->Bind();
				int pixelData = m_FrameBuffer->ReadPixel(1, mouseX, mouseY);
				m_FrameBuffer->Unbind();
				Entity entity = pixelData == -1 ? Entity() : Entity((entt::entity)pixelData, m_ActiveScene.get());
				m_SceneHierarchyPanel.SetSelectedEntity(entity);
			}
			
		}
		return false;
	}

	void EditorLayer::NewScene()
	{
		m_ActiveScene = CreateRef<EditorScene>();
		m_ActiveScene->OnViewportResize((uint32)m_ViewportSize.x, (uint32)m_ViewportSize.y);
		m_SceneHierarchyPanel.SetContext(m_ActiveScene);
		m_CurrentSceneFilePath = std::string();
	}
	void EditorLayer::OpenScene()
	{
		std::string filepath = FileDialogs::OpenFile("Pearly Scene (*.pearly)\0*.pearly\0");
		if (!filepath.empty())
		{
			NewScene();
			m_CurrentSceneFilePath = filepath;
			SceneSerializer serializer(m_ActiveScene);
			serializer.Deserialize(filepath);
		}
	}
	void EditorLayer::SaveScene()
	{
		if (m_CurrentSceneFilePath.empty())
		{
			SaveAsScene();
		}
		else
		{
			SceneSerializer serializer(m_ActiveScene);
			serializer.Serialize(m_CurrentSceneFilePath);
		}
	}
	void EditorLayer::SaveAsScene()
	{
		std::string filepath = FileDialogs::SaveFile("Pearly Scene (*.pearly)\0*.pearly\0");
		if (!filepath.empty())
		{
			m_CurrentSceneFilePath = filepath;
			SceneSerializer serializer(m_ActiveScene);
			serializer.Serialize(filepath);
		}
	}
}