#pragma once

#include "Pearly/Renderer/Buffer.h"

namespace Pearly {
	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(float* vertices, uint32 size);
		virtual ~OpenGLVertexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		inline virtual void SetLayout(const BufferLayout& layout) override { m_Layout = layout; }
		inline virtual const BufferLayout& GetLayout() const override { return m_Layout; }
	private:
		uint32 m_RendererID = 0;
		BufferLayout m_Layout;
	};

	
	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(uint32* indices, uint32 count);
		virtual ~OpenGLIndexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual uint32 GetCount() const override;
	private:
		uint32 m_RendererID = 0;
		uint32 m_Count;
	};
	
}