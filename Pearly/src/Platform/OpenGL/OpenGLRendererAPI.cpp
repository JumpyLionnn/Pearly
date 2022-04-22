#include "prpch.h"
#include "OpenGLRendererAPI.h"
#include "Pearly/Core/Core.h"

#include <glad/glad.h>

namespace Pearly {
	#ifdef PR_ENABLE_RENDERER_LOG
	void OpenGLMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
	{ 
		switch (severity) 
		{ 
			case GL_DEBUG_SEVERITY_HIGH: 
				#if PR_ENABLE_RENDERER_LOG_LEVEL <= 4 
					PR_CORE_CRITICAL(message); 
				#endif 
				return; 
			case GL_DEBUG_SEVERITY_MEDIUM: 
				#if PR_ENABLE_RENDERER_LOG_LEVEL <= 3 
					PR_CORE_ERROR(message); 
				#endif 
				return; 
			case GL_DEBUG_SEVERITY_LOW: 
				#if PR_ENABLE_RENDERER_LOG_LEVEL <= 2 
					PR_CORE_WARN(message); 
				#endif 
				return; 
			case GL_DEBUG_SEVERITY_NOTIFICATION: 
				#if PR_ENABLE_RENDERER_LOG_LEVEL <= 1 
					PR_CORE_TRACE(message); 
				#endif 
				return; 
		}
		PR_CORE_ASSERT(false, "Unknown severity level!");
	} 
	#endif

	void OpenGLRendererAPI::Init()
	{
		#ifdef PR_ENABLE_RENDERER_LOG
			glEnable(GL_DEBUG_OUTPUT);
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
			glDebugMessageCallback(OpenGLMessageCallback, nullptr);

			glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
		#endif

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_DEPTH_TEST);
	}
	void OpenGLRendererAPI::SetViewport(uint32 x, uint32 y, uint32 width, uint32 height)
	{
		glViewport(x, y, width, height);
	}

	void OpenGLRendererAPI::SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}
	void OpenGLRendererAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray)
	{
		glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
	}
}