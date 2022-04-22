#pragma once
#include "Pearly/Core/Window.h"
#include "Pearly/Renderer/GraphicsContext.h"

#include <GLFW/glfw3.h>

namespace Pearly {
	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProperties& poroperties);
		virtual ~WindowsWindow() override;

		virtual void OnUpdate() const override;

		virtual uint32 GetWidth() const override { return m_Data.Width; }
		virtual uint32 GetHeight() const override { return m_Data.Height; }

		virtual void SetEventCallback(const EventCallbackFn& callback) override;
		virtual void SetVSync(bool enabled) override;
		virtual bool IsVSync() const override;

		virtual void* GetNativeWindow() const override;
	private:
		void Init(const WindowProperties& poroperties);
		void Shutdown();
	private:
		GLFWwindow* m_Window;
		GraphicsContext* m_Context;

		struct WindowData 
		{
			std::string Title;
			uint32 Width;
			uint32 Height;
			bool VSync;

			EventCallbackFn EventCallback;
		};

		WindowData m_Data;
	};
}

