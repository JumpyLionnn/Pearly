#include "prpch.h"
#include "Shader.h"
#include "Pearly/Core.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Pearly {
	Shader* Shader::Create(const std::string& vertexSource, const std::string& fragmentSource)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None: PR_CORE_ASSERT(false, "Renderer API None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL: return new OpenGLShader(vertexSource, fragmentSource);
		}

		PR_CORE_ASSERT(false, "Unknown Renderer API!");
		return nullptr;
	}
}