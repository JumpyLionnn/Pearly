#include "prpch.h"
#include "Buffer.h"

#include "Pearly/Core.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Pearly {
	VertexBuffer* VertexBuffer::Create(float* vertices, unsigned int size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None: PR_CORE_ASSERT(false, "Renderer API None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL: return new OpenGLVertexBuffer(vertices, size);
		}

		PR_CORE_ASSERT(false, "Unknown Renderer API!");
		return nullptr;
	}
	
	IndexBuffer* IndexBuffer::Create(unsigned int* indices, unsigned int count)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None: PR_CORE_ASSERT(false, "Renderer API None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL: return new OpenGLIndexBuffer(indices, count);
		}

		PR_CORE_ASSERT(false, "Unknown Renderer API!");
		return nullptr;
	}
}