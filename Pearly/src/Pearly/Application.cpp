#include "prpch.h"
#include "Application.h"

#include "Pearly/Events/WindowEvents.h"

#include "GLFW/glfw3.h"

namespace Pearly {

#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

	Application::Application()
	{
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		while (m_Running)
		{
			glClearColor(0.32, 0.42, 0.52, 1);
			glClear(GL_COLOR_BUFFER_BIT);
			m_Window->OnUpdate();
		}
	}

	void Application::OnEvent(Event& e)
	{
		PR_CORE_TRACE(e);
		EventDispacher dispacher(e);
		dispacher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));
	}

	bool Application::OnWindowClose(WindowCloseEvent& event)
	{
		m_Running = false;
		return true;
	}
}