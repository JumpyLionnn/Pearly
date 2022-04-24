#include "prpch.h"
#include "Buffer.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Pearly {
	Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32 size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None: PR_CORE_ASSERT(false, "Renderer API None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL: return CreateRef<OpenGLVertexBuffer>(vertices, size);
		}

		PR_CORE_ASSERT(false, "Unknown Renderer API!");
		return nullptr;
	}
	
	Ref<IndexBuffer> IndexBuffer::Create(uint32* indices, uint32 count)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None: PR_CORE_ASSERT(false, "Renderer API None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL: return CreateRef<OpenGLIndexBuffer>(indices, count);
		}

		PR_CORE_ASSERT(false, "Unknown Renderer API!");
		return nullptr;
	}
}