#pragma once
#include "Pearly/Window.h"

#include <GLFW/glfw3.h>

namespace Pearly {
	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProperties& poroperties);
		virtual ~WindowsWindow() override;

		virtual void OnUpdate() const override;

		virtual unsigned int GetWidth() const override { return m_Data.Width; }
		virtual unsigned int GetHeight() const override { return m_Data.Height; }

		virtual void SetEventCallback(const EventCallbackFn& callback) override;
		virtual void SetVSync(bool enabled) override;
		virtual bool IsVSync() const override;
	private:
		void Init(const WindowProperties& poroperties);
		void Shutdown();
	private:
		GLFWwindow* m_Window;

		struct WindowData 
		{
			std::string Title;
			unsigned int Width;
			unsigned int Height;
			bool VSync;

			EventCallbackFn EventCallback;
		};

		WindowData m_Data;
	};
}

