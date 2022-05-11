#pragma once
#include "Pearly/Core/Core.h"

namespace Pearly {

	enum class FrameBufferTextureFormat
	{
		None,

		RGBA8,
		RED_INTEGER,

		DEPTH24STENCIL8,

		// defaults
		Color = RGBA8,
		Depth = DEPTH24STENCIL8
	};

	struct FrameBufferTextureSpecification
	{
		FrameBufferTextureFormat TextureFormat = FrameBufferTextureFormat::None;

		FrameBufferTextureSpecification() = default;
		FrameBufferTextureSpecification(FrameBufferTextureFormat format)
			: TextureFormat(format)
		{}
	};

	struct FrameBufferAttachmentSpecification
	{
		std::vector<FrameBufferTextureSpecification> Attachments;
		FrameBufferAttachmentSpecification() = default;
		FrameBufferAttachmentSpecification(std::initializer_list<FrameBufferTextureSpecification> attachments)
			: Attachments(attachments)
		{}
	};

	struct FrameBufferSpecification
	{
		uint32 Width = 0;
		uint32 Height = 0;
		uint32 Samples = 1;
		FrameBufferAttachmentSpecification Attachments;

		bool SwapChainTarget = false;
	};

	class FrameBuffer
	{
	public:
		virtual ~FrameBuffer() = default;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual void Resize(uint32 width, uint32 height) = 0;
		virtual int ReadPixel(uint32 index, int x, int y) = 0;

		virtual void ClearColorAtachment(uint32 index, int value) = 0;

		virtual uint32 GetColorAttachmentRendererID(uint32 index = 0) const = 0;

		virtual const FrameBufferSpecification& GetSpecification() const = 0;

		static Ref<FrameBuffer> Create(const FrameBufferSpecification& spec);
	};

}