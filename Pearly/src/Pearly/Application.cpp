#include "prpch.h"
#include "Application.h"

#include "Pearly/Events/WindowEvents.h"

#include "GLFW/glfw3.h"

namespace Pearly {

	Application::Application()
	{
		m_Window = std::unique_ptr<Window>(Window::Create());
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
}