#include <Pearly.h>
#include <Pearly/Core/EntryPoint.h>

#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>

class SandboxLayer : public Pearly::Layer
{
public:
	SandboxLayer()
		: Layer("Sandbox"), m_CameraController(1280.0f / 720.0f), m_SpriteSheet("assets/textures/tiles.png", { 12, 12 })
	{}
	virtual ~SandboxLayer() override {}

	virtual void OnAttach() override
	{
		PR_PROFILE_FUNCTION();
		m_CheckerboardTexture = Pearly::Texture2D::Create("assets/textures/Checkerboard.png");
		m_LilyPadTexture = m_SpriteSheet.CreateSubTexture({ 13, 20 });
		m_BigTreeTexture = m_SpriteSheet.CreateSubTexture({ 1, 20 }, {4, 4});
	}

	virtual void OnDetach() override
	{
		PR_PROFILE_FUNCTION();
	}

	virtual void OnUpdate(Pearly::Timestep ts) 
	{
		PR_PROFILE_FUNCTION();
		m_CameraController.OnUpdate(ts);

		Pearly::Renderer::ResetStats();

		Pearly::RenderCommand::SetClearColor(glm::vec4(0.32f, 0.42f, 0.52f, 1.0f));
		Pearly::RenderCommand::Clear();

		/*
		Pearly::Renderer::BeginScene(m_CameraController.GetCamera());

		glm::vec4 redColor(0.8f, 0.2f, 0.3f, 1.0f);
		glm::vec4 blueColor(0.2f, 0.3f, 0.8f, 1.0f);

		Pearly::Renderer::DrawQuad({ { 0.0f, 0.0f }, { 1.0f, 1.0f } }, m_Color);
		Pearly::Renderer::DrawQuad({ { 1.2f, -0.3f }, { 1.0f, 2.0f } }, redColor);
		Pearly::Renderer::DrawQuad({ { -1.2f, -0.5f }, { 1.0f, 2.0f } }, blueColor);
		Pearly::Renderer::DrawQuad({ { 0.0f, 0.0f, -0.1f }, { 15.0f, 15.0f } }, m_CheckerboardTexture, {1.0f, 1.0f, 1.0f, 1.0f}, 20);
		Pearly::Renderer::DrawQuad({ { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f }, glm::radians(45.0f) }, m_CheckerboardTexture, {1.0f, 1.0f, 1.0f, 1.0f}, 20);
		Pearly::Renderer::EndScene();
		*/
		
		// tiles
		Pearly::Renderer::BeginScene(m_CameraController.GetCamera());
		Pearly::Renderer::DrawQuad({ { -1.0f, 1.0f }, { 4.0f, 4.0f } }, m_BigTreeTexture);
		Pearly::Renderer::DrawQuad({ { 1.0f, 0.0f, 0.1f }, { 1.0f, 1.0f } }, m_LilyPadTexture);
		Pearly::Renderer::EndScene();
	}

	virtual void OnEvent(Pearly::Event& event)
	{
		m_CameraController.OnEvent(event);
	}

	virtual void OnImGuiRender() 
	{
		PR_PROFILE_FUNCTION();
		ImGui::Begin("Settings");

		Pearly::Renderer::Statistics stats = Pearly::Renderer::GetStats();
		ImGui::Text("Renderer Stats:");
		ImGui::Text("Draw Calls: %d", stats.DrawCalls);
		ImGui::Text("Quad Count: %d", stats.QuadCount);
		ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
		ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

		ImGui::ColorEdit4("Square Color", glm::value_ptr(m_Color));
		ImGui::End();
	}

private:
	Pearly::SpriteSheet m_SpriteSheet;
	Pearly::Ref<Pearly::Texture2D> m_CheckerboardTexture;

	Pearly::Ref<Pearly::SubTexture2D> m_LilyPadTexture;
	Pearly::Ref<Pearly::SubTexture2D> m_BigTreeTexture;

	Pearly::OrthographicCameraController m_CameraController;

	glm::vec4 m_Color = { 0.2f, 0.3f, 0.8f, 1.0f };
};

class Sandbox : public Pearly::Application
{
public:
	Sandbox()
	{
		PushLayer(new SandboxLayer());
	}
	~Sandbox()
	{

	}

private:

};
Pearly::Application* Pearly::CreateApplication()
{
	return new Sandbox();
}