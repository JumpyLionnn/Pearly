#include "prpch.h"
#include "WindowsWindow.h"

namespace Pearly {
	static bool s_GLFWInitialized = false;

	Window* Window::Create(const WindowProperties& properties)
	{
		return new WindowsWindow(properties);
	}

	WindowsWindow::WindowsWindow(const WindowProperties& properties)
	{
		Init(properties);
	}

	WindowsWindow::~WindowsWindow()
	{
		Shutdown();
	}

	void WindowsWindow::Init(const WindowProperties& properties)
	{
		m_Data.Title = properties.Title;
		m_Data.Width = properties.Width;
		m_Data.Height = properties.Height;

		PR_CORE_INFO("Creating a window {0}, ({1}, {2})", properties.Title, properties.Width, properties.Height);

		if (!s_GLFWInitialized)
		{
			// TODO: glfwTerminate on system shutdown
			int success = glfwInit();
			PR_CORE_ASSERT(success, "Could bot initialize glfw!");
			s_GLFWInitialized = true;
		}

		m_Window = glfwCreateWindow((int)properties.Width, (int)properties.Height, properties.Title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_Window);
		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(true);
	}

	void WindowsWindow::Shutdown()
	{
		glfwDestroyWindow(m_Window);
	}

	void WindowsWindow::OnUpdate() const
	{
		glfwPollEvents();
		glfwSwapBuffers(m_Window);
	}

	void WindowsWindow::SetEventCallback(const EventCallbackFn& callback)
	{
		m_Data.EventCallback = callback;
	}

	void WindowsWindow::SetVSync(bool enabled)
	{
		if (enabled)
		{
			glfwSwapInterval(1);
		}
		else
		{
			glfwSwapInterval(0);
		}
		m_Data.VSync = enabled;
	}

	bool WindowsWindow::IsVSync() const
	{
		return m_Data.VSync;
	}
}