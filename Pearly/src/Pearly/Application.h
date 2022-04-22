#pragma once
#include "Window.h"
#include "Events/WindowEvents.h"
#include "Core/Timestep.h"
#include "LayerStack.h"
#include "Debug/ImGuiLayer.h"

namespace Pearly {
	class Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
		void PopLayer(Layer* layer);
		void PopOverlay(Layer* overlay);

		inline static Application& Get() { return *s_Instance; }
		inline Window& GetWindow() { return *m_Window; }
	private:
		bool OnWindowClose(WindowCloseEvent& event);
		bool OnWindowResize(WindowResizeEvent& event);
	private:
		bool m_Running = true;
		bool m_Minimized = false;
		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		LayerStack m_LayerStack;
		Timestep m_LastFrameTime;
	private:
		static Application* s_Instance;
	};
	
	// to be defined in a client
	Application* CreateApplication();
}

