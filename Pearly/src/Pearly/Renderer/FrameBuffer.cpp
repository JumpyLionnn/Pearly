#include"prpch.h"
#include "FrameBuffer.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLFrameBuffer.h"

namespace Pearly {

	Ref<FrameBuffer> FrameBuffer::Create(const FrameBufferSpecification& spec)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None: PR_CORE_ASSERT(false, "Renderer API None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL: return CreateRef<OpenGLFrameBuffer>(spec);
		}

		PR_CORE_ASSERT(false, "Unknown Renderer API!");
		return nullptr;
	}
}