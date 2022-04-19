#include "prpch.h"
#include "Application.h"

#include "Pearly/Events/WindowEvents.h"

#include "Input.h"

#include <Glad/glad.h>
#include "Renderer/Renderer.h"

namespace Pearly {

	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		PR_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(PR_BIND_EVENT_FN(Application::OnEvent));

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

		m_VertexArray.reset(VertexArray::Create());

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
			0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
			0.0f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		};

		m_VertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

		BufferLayout layout = {
			{ShaderDataType::Vec3f, "a_Position"},
			{ShaderDataType::Vec4f, "a_Color"},
		};
		m_VertexBuffer->SetLayout(layout);

		m_VertexArray->AddVertexBuffer(m_VertexBuffer);


		unsigned int indices[3] = { 0, 1, 2 };
		m_IndexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(unsigned int)));

		m_VertexArray->SetIndexBuffer(m_IndexBuffer);

		std::string vertexSource = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Postion;
			layout(location = 1) in vec4 a_Color;

			out vec4 v_Color;
			
			void main()
			{
				v_Color = a_Color;
				gl_Position = vec4(a_Postion, 1.0);
			}

		)";
		std::string fragmentSource = R"(
			#version 330 core

			layout(location = 0) out vec4 Color;
			
			in vec4 v_Color;

			void main()
			{
				Color = v_Color;
			}
		)";

		m_Shader.reset(new Shader(vertexSource, fragmentSource));

	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		while (m_Running)
		{
			RenderCommand::SetClearColor(glm::vec4(0.32f, 0.42f, 0.52f, 1.0f));
			RenderCommand::Clear();

			Renderer::BeginScene();

			m_Shader->Bind();
			Renderer::Submit(m_VertexArray);

			Renderer::EndScene();
			//glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();

			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();
			m_ImGuiLayer->End();

			m_Window->OnUpdate();
		}
	}

	void Application::OnEvent(Event& e)
	{
		EventDispacher dispacher(e);
		dispacher.Dispatch<WindowCloseEvent>(PR_BIND_EVENT_FN(Application::OnWindowClose));

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
	}
	void Application::PushOverlay(Layer* overlay)
	{
		m_LayerStack.PushOverlay(overlay);
	}
	void Application::PopLayer(Layer* layer)
	{
		m_LayerStack.PopLayer(layer);
	}
	void Application::PopOverlay(Layer* overlay)
	{
		m_LayerStack.PopOverlay(overlay);
	}

	bool Application::OnWindowClose(WindowCloseEvent& event)
	{
		m_Running = false;
		return true;
	}
}