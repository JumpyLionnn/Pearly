#pragma once
#include "Pearly/Core/Core.h"

namespace Pearly {

	struct FrameBufferSpecification
	{
		uint32 Width;
		uint32 Height;
		uint32 Samples = 1;

		bool SwapChainTarget = false;
	};

	class FrameBuffer
	{
	public:
		virtual ~FrameBuffer() = default;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual uint32 GetColorAttachmentRendererID() const = 0;

		virtual const FrameBufferSpecification& GetSpecification() const = 0;

		static Ref<FrameBuffer> Create(const FrameBufferSpecification& spec);
	};

}