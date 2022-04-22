#include <Pearly.h>
#include <Pearly/Core/EntryPoint.h>

#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>

class SandboxLayer : public Pearly::Layer
{
public:
	SandboxLayer()
		: Layer("Sandbox"), m_CameraController(1280.0f / 720.0f)
	{}
	virtual ~SandboxLayer() override {}

	virtual void OnAttach() override
	{
		m_Texture = Pearly::Texture2D::Create("assets/textures/Checkerboard.png");
	}

	virtual void OnDetach() override
	{
		
	}

	virtual void OnUpdate(Pearly::Timestep ts) 
	{
		m_CameraController.OnUpdate(ts);

		Pearly::RenderCommand::SetClearColor(glm::vec4(0.32f, 0.42f, 0.52f, 1.0f));
		Pearly::RenderCommand::Clear();

		Pearly::Renderer::BeginScene(m_CameraController.GetCamera());

		glm::vec4 redColor(0.8f, 0.2f, 0.3f, 1.0f);
		glm::vec4 blueColor(0.2f, 0.3f, 0.8f, 1.0f);

		Pearly::Renderer::DrawQuad({ 0.0f, 0.0f }, { 1.0f, 1.0f }, m_Color);
		Pearly::Renderer::DrawQuad({ 1.2f, 0.0f }, { 1.0f, 2.0f }, redColor);
		Pearly::Renderer::DrawQuad({ -1.2f, 1.3f }, { 1.0f, 2.0f }, blueColor);
		Pearly::Renderer::DrawQuad({ -0.5f, 0.0f, -0.1f }, { 5.0f, 5.0f }, m_Texture);

		Pearly::Renderer::EndScene();
	}

	virtual void OnEvent(Pearly::Event& event)
	{
		m_CameraController.OnEvent(event);
	}

	virtual void OnImGuiRender() 
	{
		ImGui::Begin("Settings");
		ImGui::ColorEdit4("Square Color", glm::value_ptr(m_Color));
		ImGui::End();
	}

private:
	Pearly::Ref<Pearly::Texture2D> m_Texture;
	Pearly::Ref<Pearly::VertexArray> m_VertexArray;
	Pearly::Ref<Pearly::Shader> m_Shader;

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