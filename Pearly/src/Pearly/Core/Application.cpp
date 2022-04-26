#include "prpch.h"
#include "Application.h"

#include "Pearly/Events/WindowEvents.h"

#include "Input.h"

#include "Pearly/Renderer/Renderer.h"
#include <GLFW/glfw3.h>

namespace Pearly {

	Application* Application::s_Instance = nullptr;

	Application::Application(const std::string& name)
	{
		PR_PROFILE_FUNCTION();
		PR_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create(WindowProperties(name)));
		m_Window->SetEventCallback(PR_BIND_EVENT_FN(Application::OnEvent));

		Renderer::Init();

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
	}

	void Application::Run()
	{
		PR_PROFILE_FUNCTION();
		while (m_Running)
		{
			PR_PROFILE_SCOPE("Frame");
			float time = (float)glfwGetTime(); // TODO: Platform::GetTime();
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			if (!m_Minimized)
			{
				{
					PR_PROFILE_SCOPE("Layers update");
					for (Layer* layer : m_LayerStack)
						layer->OnUpdate(timestep);
				}
			}
			

			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();
			m_ImGuiLayer->End();

			m_Window->OnUpdate();
		}
	}

	void Application::OnEvent(Event& e)
	{
		PR_PROFILE_FUNCTION();
		EventDispacher dispacher(e);
		dispacher.Dispatch<WindowCloseEvent>(PR_BIND_EVENT_FN(Application::OnWindowClose));
		dispacher.Dispatch<WindowResizeEvent>(PR_BIND_EVENT_FN(Application::OnWindowResize));

		for (std::vector<Layer*>::reverse_iterator it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
		{
			(*it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		PR_PROFILE_FUNCTION();
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}
	void Application::PushOverlay(Layer* overlay)
	{
		PR_PROFILE_FUNCTION();
		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}
	void Application::PopLayer(Layer* layer)
	{
		PR_PROFILE_FUNCTION();
		m_LayerStack.PopLayer(layer);
		layer->OnDetach();
	}
	void Application::PopOverlay(Layer* overlay)
	{
		PR_PROFILE_FUNCTION();
		m_LayerStack.PopOverlay(overlay);
		overlay->OnDetach();
	}

	void Application::Close()
	{
		m_Running = false;
	}

	bool Application::OnWindowClose(WindowCloseEvent& event)
	{
		m_Running = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& event)
	{
		PR_PROFILE_FUNCTION();
		if (event.GetWidth() == 0 || event.GetHeight() == 0)
		{
			m_Minimized = true;
			return false;
		}
		m_Minimized = false;
		Renderer::OnWindowResize(event.GetWidth(), event.GetHeight());
		return false;
	}
}