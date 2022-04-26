#pragma once
#include "Window.h"
#include "Pearly/Events/WindowEvents.h"
#include "Timestep.h"
#include "LayerStack.h"
#include "Pearly/Debug/ImGuiLayer.h"

int main(int argc, char** argv);

namespace Pearly {
	class Application
	{
	friend int ::main(int argc, char** argv);
	public:
		Application(const std::string& name = "Pearly Application");
		virtual ~Application() = default;


		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
		void PopLayer(Layer* layer);
		void PopOverlay(Layer* overlay);

		inline static Application& Get() { return *s_Instance; }
		inline Window& GetWindow() { return *m_Window; }

		void Close();
	private:
		void Run();
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

