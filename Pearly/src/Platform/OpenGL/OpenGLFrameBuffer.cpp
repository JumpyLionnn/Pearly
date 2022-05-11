#include "prpch.h"
#include "OpenGLFrameBuffer.h"

#include <glad/glad.h>

namespace Pearly {

	static const uint32 s_MaxFrameBufferSize = 8192; // TODO: use RenderCapabilities

	static bool IsDepthFormat(FrameBufferTextureFormat format)
	{
		switch (format)
		{
			case FrameBufferTextureFormat::DEPTH24STENCIL8:
				return true;
			default:
				return false;
		}
	}

	static GLenum TextureTarget(bool multisampled)
	{
		return multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
	}

	static void BindTexture(bool multisampled, uint32 id)
	{
		glBindTexture(TextureTarget(multisampled), id);
	}

	static void CreateTextures(bool multisampled, uint32* outID, uint32 count)
	{
		glCreateTextures(TextureTarget(multisampled), count, outID);
	}

	static void AttachColorTexture(uint32 id, int samples, GLenum internalFormat, GLenum format, uint32 width, uint32 height, int index)
	{
		bool multisampled = samples > 1;
		if (multisampled)
		{
			glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internalFormat, width, height, GL_FALSE);
		}
		else
		{
			glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr);

			glTextureParameteri(id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTextureParameteri(id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glTextureParameteri(id, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			glTextureParameteri(id, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTextureParameteri(id, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, TextureTarget(multisampled), id, 0);
	}

	static void AttachDepthTexture(uint32 id, int samples, GLenum format, GLenum attachmentType, uint32 width, uint32 height)
	{
		bool multisampled = samples > 1;
		if (multisampled)
		{
			glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
		}
		else
		{
			glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);

			glTextureParameteri(id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTextureParameteri(id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glTextureParameteri(id, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			glTextureParameteri(id, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTextureParameteri(id, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}

		glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, TextureTarget(multisampled), id, 0);
	}


	static GLenum FrameBufferTextureFormatToGl(FrameBufferTextureFormat format)
	{
		switch (format)
		{
			case Pearly::FrameBufferTextureFormat::RGBA8:
				return GL_RGBA8;
			case Pearly::FrameBufferTextureFormat::RED_INTEGER:
				return GL_RED_INTEGER;
		}
		PR_CORE_ASSERT(false, "Texture format isnt supported!");
		return 0;
	}

	OpenGLFrameBuffer::OpenGLFrameBuffer(const FrameBufferSpecification& spec)
		: m_Specification(spec)
	{
		for (auto format : m_Specification.Attachments.Attachments)
		{
			if (!IsDepthFormat(format.TextureFormat))
				m_ColorAttachmentSpecifications.emplace_back(format);
			else
				m_DepthAttachmentSpecification = format.TextureFormat;
		}

		Invalidate();
	}

	OpenGLFrameBuffer::~OpenGLFrameBuffer()
	{
		glDeleteFramebuffers(1, &m_RendererID);
		glDeleteTextures((GLsizei)m_ColorAttachments.size(), m_ColorAttachments.data());
		glDeleteTextures(1, &m_DepthAttachment);
	}

	void OpenGLFrameBuffer::Resize(uint32 width, uint32 height)
	{
		if (width == 0 || height == 0 || width > s_MaxFrameBufferSize || height > s_MaxFrameBufferSize)
		{
			PR_CORE_WARN("Cannot resize the frame buffer to {0}, {1}", width, height);
			return;
		}
		m_Specification.Width = width;
		m_Specification.Height = height;
		Invalidate();
	}

	int OpenGLFrameBuffer::ReadPixel(uint32 index, int x, int y)
	{
		PR_CORE_ASSERT(index < m_ColorAttachments.size(), "The color attachment does not exists!");
		glReadBuffer(GL_COLOR_ATTACHMENT0 + index);
		int pixelData;
		glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);
		return pixelData;
	}

	void OpenGLFrameBuffer::ClearColorAtachment(uint32 index, int value)
	{
		PR_CORE_ASSERT(index < m_ColorAttachments.size(), "The color attachment does not exists!");

		FrameBufferTextureSpecification spec = m_ColorAttachmentSpecifications[index];

		glClearTexImage(m_ColorAttachments[index], 0, FrameBufferTextureFormatToGl(spec.TextureFormat), GL_INT, &value);
	}

	void OpenGLFrameBuffer::Invalidate()
	{
		if (m_RendererID)
		{
			glDeleteFramebuffers(1, &m_RendererID);
			glDeleteTextures((GLsizei)m_ColorAttachments.size(), m_ColorAttachments.data());
			glDeleteTextures(1, &m_DepthAttachment);

			m_ColorAttachments.clear();
			m_DepthAttachment = 0;
		}

		glCreateFramebuffers(1, &m_RendererID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

		bool multisample = m_Specification.Samples > 1;

		if (!m_ColorAttachmentSpecifications.empty())
		{
			m_ColorAttachments.resize(m_ColorAttachmentSpecifications.size());
			CreateTextures(multisample, m_ColorAttachments.data(), (uint32)m_ColorAttachments.size());
			for (int i = 0; i < m_ColorAttachments.size(); i++)
			{
				BindTexture(multisample, m_ColorAttachments[i]);
				switch (m_ColorAttachmentSpecifications[i].TextureFormat)
				{
				case FrameBufferTextureFormat::RGBA8:
					AttachColorTexture(m_ColorAttachments[i], m_Specification.Samples, GL_RGBA8, GL_RGBA, m_Specification.Width, m_Specification.Height, i);
					break;
				case FrameBufferTextureFormat::RED_INTEGER:
					AttachColorTexture(m_ColorAttachments[i], m_Specification.Samples, GL_R32I, GL_RED_INTEGER, m_Specification.Width, m_Specification.Height, i);
					break;
				}
			}
		}
		if (m_DepthAttachmentSpecification.TextureFormat != FrameBufferTextureFormat::None)
		{
			CreateTextures(multisample, &m_DepthAttachment, 1);
			BindTexture(multisample, m_DepthAttachment);
			switch (m_DepthAttachmentSpecification.TextureFormat)
			{
			case FrameBufferTextureFormat::DEPTH24STENCIL8:
				AttachDepthTexture(m_DepthAttachment, m_Specification.Samples, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, m_Specification.Width, m_Specification.Height);
				break;
			}
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_DepthAttachment, 0);
		}

		if (m_ColorAttachments.size() > 1)
		{
			PR_CORE_ASSERT(m_ColorAttachments.size() <= 4, "more than 4 color attachments are not allowed");
			GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
			glDrawBuffers((GLsizei)m_ColorAttachments.size(), buffers);
		}
		else if (m_ColorAttachments.empty())
		{
			glDrawBuffer(GL_NONE);
		}

		PR_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFrameBuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
		glViewport(0, 0, m_Specification.Width, m_Specification.Height);
	}

	void OpenGLFrameBuffer::Unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}