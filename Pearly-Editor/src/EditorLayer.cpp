#include "EditorLayer.h"
#include <iostream>


#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>




namespace Pearly {


	class Controller : public ScriptableEntity
	{
	public:
		void OnUpdate(Timestep ts)
		{
			auto& transformComponent = GetComponent<TransformComponent>();
			float speed = 5.0f;

			if (Input::IsKeyPressed(PR_KEY_A))
				transformComponent.Transform[3][0] -= speed * ts;
			if (Input::IsKeyPressed(PR_KEY_D))
				transformComponent.Transform[3][0] += speed * ts;
			if (Input::IsKeyPressed(PR_KEY_W))
				transformComponent.Transform[3][1] += speed * ts;
			if (Input::IsKeyPressed(PR_KEY_S))
				transformComponent.Transform[3][1] -= speed * ts;

		}
	};

	EditorLayer::EditorLayer()
		: Layer("EditorLayer"), m_CameraController(1280.0f / 720.0f), m_SpriteSheet("assets/textures/tiles.png", { 12, 12 })
	{}

	void EditorLayer::OnAttach()
	{
		PR_PROFILE_FUNCTION();

		m_CheckerboardTexture = Texture2D::Create("assets/textures/Checkerboard.png");
		m_LilyPadTexture = m_SpriteSheet.CreateSubTexture({ 13, 20 });
		m_BigTreeTexture = m_SpriteSheet.CreateSubTexture({ 1, 20 }, { 4, 4 });

		FrameBufferSpecification frameBufferSpec;
		frameBufferSpec.Width = 1280;
		frameBufferSpec.Height = 720;
		m_FrameBuffer = FrameBuffer::Create(frameBufferSpec);

		m_ActiveScene = CreateRef<Scene>();
		m_SquareEntity = m_ActiveScene->CreateEntity("Square");
		m_SquareEntity.AddComponent<SpriteRendererComponent>(glm::vec4(0.2f, 1.0f, 0.3f, 1.0f));

		m_CameraEntity = m_ActiveScene->CreateEntity("camera");
		CameraComponent& camera = m_SquareEntity.AddComponent<CameraComponent>();
		camera.Primary = true;

		m_CameraEntity.AddComponent<NativeScriptComponent>().Bind<Controller>();
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
		
		m_ActiveScene->OnUpdate(ts);

		m_FrameBuffer->Unbind();
		
	}

	void EditorLayer::OnImGuiRender()
	{
		PR_PROFILE_FUNCTION();
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
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				// Disabling fullscreen would allow the window to be moved to the front of other windows, 
				// which we can't undo at the moment without finer window depth/z control.
				//ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);

				if (ImGui::MenuItem("Exit")) Application::Get().Close();
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		ImGui::Begin("Settings");
		if (m_SquareEntity)
		{
			ImGui::Text("%s", m_SquareEntity.GetComponent<TagComponent>().Tag.c_str());

			glm::vec4& color = m_SquareEntity.GetComponent<SpriteRendererComponent>().Color;
			ImGui::ColorEdit4("Square Color", glm::value_ptr(color));
		}
		ImGui::Separator();
		

		Renderer::Statistics stats = Renderer::GetStats();
		ImGui::Text("Renderer Stats:");
		ImGui::Text("Draw Calls: %d", stats.DrawCalls);
		ImGui::Text("Quads: %d", stats.QuadCount);
		ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
		ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0,0));
		ImGui::Begin("Viewport");

		m_ViewportFocused = ImGui::IsWindowFocused();
		m_ViewportHovered = ImGui::IsWindowHovered();
		Application::Get().GetImGuiLayer()->SetBlockEvents(!m_ViewportFocused || !m_ViewportHovered);

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
		if (m_ViewportSize != *((glm::vec2*)&viewportPanelSize))
		{
			//m_FrameBuffer->Resize((uint32)viewportPanelSize.x, (uint32)viewportPanelSize.y);

			//m_CameraController.OnResize(m_ViewportSize.x, m_ViewportSize.y);
		}
		ImGui::Image((void*)m_FrameBuffer->GetColorAttachmentRendererID(), { m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
		ImGui::End();
		ImGui::PopStyleVar();

		ImGui::End();
	}

	void EditorLayer::OnEvent(Event& e)
	{
		m_CameraController.OnEvent(e);
	}
}