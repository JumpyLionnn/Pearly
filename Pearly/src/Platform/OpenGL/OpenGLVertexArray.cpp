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
		PR_PROFILE_FUNCTION();
		glCreateVertexArrays(1, &m_RendererID);
	}
	OpenGLVertexArray::~OpenGLVertexArray() 
	{
		PR_PROFILE_FUNCTION();
		glDeleteVertexArrays(1, &m_RendererID);
	}

	void OpenGLVertexArray::Bind() const
	{
		PR_PROFILE_FUNCTION();
		glBindVertexArray(m_RendererID);
	}

	void OpenGLVertexArray::Unbind() const
	{
		PR_PROFILE_FUNCTION();
		glBindVertexArray(0);
	}


	void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)
	{
		PR_PROFILE_FUNCTION();
		PR_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "VertexBuffer has no layout!");
		glBindVertexArray(m_RendererID);
		vertexBuffer->Bind();
		const BufferLayout& layout = vertexBuffer->GetLayout();
		for (const BufferElement& element : layout)
		{
			glEnableVertexAttribArray(m_VertexBufferIndex);
			switch (element.Type)
			{
				case ShaderDataType::Float:
				case ShaderDataType::Vec2f:
				case ShaderDataType::Vec3f:
				case ShaderDataType::Vec4f:
				{
					glVertexAttribPointer(m_VertexBufferIndex,
						element.GetComponentCount(),
						ShaderDataTypeToOpenGLBaseType(element.Type),
						element.Normalized ? GL_TRUE : GL_FALSE,
						layout.GetStride(),
						(const void*)(uint64)element.Offset);
					m_VertexBufferIndex++;
					break;
				}
				case ShaderDataType::Int:
				case ShaderDataType::Vec2i:
				case ShaderDataType::Vec3i:
				case ShaderDataType::Vec4i:
				case ShaderDataType::Bool:
				{
					glVertexAttribIPointer(m_VertexBufferIndex,
						element.GetComponentCount(),
						ShaderDataTypeToOpenGLBaseType(element.Type),
						layout.GetStride(),
						(const void*)(uint64)element.Offset);
					m_VertexBufferIndex++;
					break;
				}
				case ShaderDataType::Mat3:
				case ShaderDataType::Mat4:
				{
					uint8_t count = element.GetComponentCount();
					for (uint8_t i = 0; i < count; i++)
					{
						glVertexAttribPointer(m_VertexBufferIndex,
							count,
							ShaderDataTypeToOpenGLBaseType(element.Type),
							element.Normalized ? GL_TRUE : GL_FALSE,
							layout.GetStride(),
							(const void*)(element.Offset + sizeof(float) * count * i));
						glVertexAttribDivisor(m_VertexBufferIndex, 1);
						m_VertexBufferIndex++;
					}
					break;
				}
				default:
					PR_CORE_ASSERT(false, "Unknown ShaderDataType!");
			}
		}
		m_VertexBuffers.push_back(vertexBuffer);
	}

	void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
	{
		PR_PROFILE_FUNCTION();
		glBindVertexArray(m_RendererID);
		indexBuffer->Bind();
		m_IndexBuffer = indexBuffer;
	}

	const Ref<IndexBuffer>& OpenGLVertexArray::GetIndexBuffer() const
	{
		return m_IndexBuffer;
	}
}