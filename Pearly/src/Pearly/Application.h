#pragma once
#include "Window.h"

#include "Events/WindowEvents.h"

namespace Pearly {
	class Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);
	private:
		bool OnWindowClose(WindowCloseEvent& event);
	private:
		bool m_Running = true;
		std::unique_ptr<Window> m_Window;
	};
	
	// to be defined in a client
	Application* CreateApplication();
}

