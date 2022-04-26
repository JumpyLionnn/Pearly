#pragma once
#include "Pearly/Renderer/FrameBuffer.h"

namespace Pearly {
	class OpenGLFrameBuffer : public FrameBuffer
	{
	public:
		OpenGLFrameBuffer(const FrameBufferSpecification& spec);
		virtual ~OpenGLFrameBuffer();

		void Resize();

		virtual void Bind() override;
		virtual void Unbind() override;

		virtual uint32 GetColorAttachmentRendererID() const override { return m_ColorAttachment; }

		virtual const FrameBufferSpecification& GetSpecification() const override { return m_Specification; }
	private:
		FrameBufferSpecification m_Specification;
		uint32 m_RendererID;
		uint32 m_ColorAttachment, m_DepthAttachment;
	};
}