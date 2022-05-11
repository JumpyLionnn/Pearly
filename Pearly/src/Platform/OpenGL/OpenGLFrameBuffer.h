#pragma once
#include "Pearly/Renderer/FrameBuffer.h"

namespace Pearly {
	class OpenGLFrameBuffer : public FrameBuffer
	{
	public:
		OpenGLFrameBuffer(const FrameBufferSpecification& spec);
		virtual ~OpenGLFrameBuffer();

		virtual void Resize(uint32 width, uint32 height) override;
		virtual int ReadPixel(uint32 index, int x, int y) override;

		virtual void ClearColorAtachment(uint32 index, int value) override;

		void Invalidate();

		virtual void Bind() override;
		virtual void Unbind() override;

		virtual uint32 GetColorAttachmentRendererID(uint32 index) const override { PR_CORE_ASSERT(index < m_ColorAttachments.size(), "the color attachment does not exists!");  return m_ColorAttachments[index]; }

		virtual const FrameBufferSpecification& GetSpecification() const override { return m_Specification; }
	private:
		FrameBufferSpecification m_Specification;
		uint32 m_RendererID = 0;

		std::vector<FrameBufferTextureSpecification> m_ColorAttachmentSpecifications;
		FrameBufferTextureSpecification m_DepthAttachmentSpecification;

		std::vector<uint32> m_ColorAttachments;
		uint32 m_DepthAttachment = 0;
	};
}