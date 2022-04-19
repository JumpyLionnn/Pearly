#include "prpch.h"
#include "OpenGLVertexArray.h"

#include "glad/glad.h"

namespace Pearly {

	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
			case Pearly::ShaderDataType::Float: return GL_FLOAT;
			case Pearly::ShaderDataType::Vec2f: return GL_FLOAT;
			case Pearly::ShaderDataType::Vec3f: return GL_FLOAT;
			case Pearly::ShaderDataType::Vec4f: return GL_FLOAT;
			case Pearly::ShaderDataType::Mat3: return GL_FLOAT;
			case Pearly::ShaderDataType::Mat4: return GL_FLOAT;
			case Pearly::ShaderDataType::Int: return GL_INT;
			case Pearly::ShaderDataType::Vec2i: return GL_INT;
			case Pearly::ShaderDataType::Vec3i: return GL_INT;
			case Pearly::ShaderDataType::Vec4i: return GL_INT;
			case Pearly::ShaderDataType::Bool: return GL_BOOL;
		}
		PR_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		glCreateVertexArrays(1, &m_RendererID);
	}
	OpenGLVertexArray::~OpenGLVertexArray() 
	{
		glDeleteVertexArrays(m_RendererID);
	}

	void OpenGLVertexArray::Bind() const
	{
		glBindVertexArray(m_RendererID);
	}

	void OpenGLVertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}


	void OpenGLVertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)
	{
		PR_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "VertexBuffer has no layout!");
		glBindVertexArray(m_RendererID);
		vertexBuffer->Bind();
		unsigned int index = 0;
		const BufferLayout& layout = vertexBuffer->GetLayout();
		for (const BufferElement& element : layout)
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(
				index,
				element.GetComponentCount(),
				ShaderDataTypeToOpenGLBaseType(element.Type),
				element.Normalized ? GL_TRUE : GL_FALSE,
				layout.GetStride(),
				(const void*)element.Offset
			);
			index++;
		}
		m_VertexBuffers.push_back(vertexBuffer);
	}

	void OpenGLVertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
	{
		glBindVertexArray(m_RendererID);
		indexBuffer->Bind();
		m_IndexBuffer = indexBuffer;
	}
}