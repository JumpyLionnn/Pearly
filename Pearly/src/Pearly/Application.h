#pragma once
#include "Window.h"
#include "Events/WindowEvents.h"
#include "LayerStack.h"

namespace Pearly {
	class Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PopLayer(Layer* layer);
	private:
		bool OnWindowClose(WindowCloseEvent& event);
	private:
		bool m_Running = true;
		std::unique_ptr<Window> m_Window;
		LayerStack m_LayerStack;
	};
	
	// to be defined in a client
	Application* CreateApplication();
}

