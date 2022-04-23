#include "prpch.h"
#include "OpenGLContext.h"

#include "Pearly/Core/Core.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Pearly {
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		PR_CORE_ASSERT(windowHandle, "Handle is null!");
	}

	void OpenGLContext::Init()
	{
		PR_PROFILE_FUNCTION();
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		PR_CORE_ASSERT(status, "Faild to initialize Glad!");

		PR_CORE_INFO("OpenGL Renderer:");
		PR_CORE_INFO("  Vendor: {0}", glGetString(GL_VENDOR));
		PR_CORE_INFO("  Renderer: {0}", glGetString(GL_RENDERER));
		PR_CORE_INFO("  Version: {0}", glGetString(GL_VERSION));
	}
	void OpenGLContext::SwapBuffers()
	{
		PR_PROFILE_FUNCTION();
		glfwSwapBuffers(m_WindowHandle);
	}
}