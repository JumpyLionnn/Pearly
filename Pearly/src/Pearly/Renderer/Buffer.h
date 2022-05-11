#pragma once
#include "Pearly/Core/Core.h"

namespace Pearly {

	enum class ShaderDataType : uint8
	{
		None,
		Float,
		Vec2f,
		Vec3f,
		Vec4f,
		Mat3,
		Mat4,
		Int,
		Vec2i,
		Vec3i,
		Vec4i,
		Bool
	};
	static uint32 ShaderDataTypeSize(ShaderDataType type)
	{
		switch (type)
		{
			case Pearly::ShaderDataType::Float: return 4;
			case Pearly::ShaderDataType::Vec2f: return 4 * 2;
			case Pearly::ShaderDataType::Vec3f: return 4 * 3;
			case Pearly::ShaderDataType::Vec4f: return 4 * 4;
			case Pearly::ShaderDataType::Mat3: return 4 * 3 * 3;
			case Pearly::ShaderDataType::Mat4: return 4 * 4 * 4;
			case Pearly::ShaderDataType::Int: return 4;
			case Pearly::ShaderDataType::Vec2i: return 4 * 2;
			case Pearly::ShaderDataType::Vec3i: return 4 * 3;
			case Pearly::ShaderDataType::Vec4i: return 4 * 4;
			case Pearly::ShaderDataType::Bool: return 1;
		}
		PR_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	struct BufferElement
	{
		ShaderDataType Type;
		std::string Name;
		bool Normalized;
		uint32 Offset;
		uint32 Size;

		BufferElement(ShaderDataType type, const std::string& name, bool normalized = false)
			: Type(type), Name(name), Normalized(normalized), Size(ShaderDataTypeSize(type)), Offset(0)
		{}

		unsigned int GetComponentCount() const
		{
			switch (Type)
			{
				case Pearly::ShaderDataType::Float: return 1;
				case Pearly::ShaderDataType::Vec2f: return 2;
				case Pearly::ShaderDataType::Vec3f: return 3;
				case Pearly::ShaderDataType::Vec4f: return 4;
				case Pearly::ShaderDataType::Mat3: return 3 * 3;
				case Pearly::ShaderDataType::Mat4: return 4 * 4;
				case Pearly::ShaderDataType::Int: return 1;
				case Pearly::ShaderDataType::Vec2i: return 2;
				case Pearly::ShaderDataType::Vec3i: return 3;
				case Pearly::ShaderDataType::Vec4i: return 4;
				case Pearly::ShaderDataType::Bool: return 1;
			}
			PR_CORE_ASSERT(false, "Unknown ShaderDataType!");
			return 0;
		}
	};

	class BufferLayout
	{
	public:
		BufferLayout() {}
		BufferLayout(const std::initializer_list<BufferElement>& elements)
			: m_Elements(elements)
		{
			CalculateOffsetAndStride();
		}

		inline const std::vector<BufferElement>& GetElements() const { return m_Elements; }
		inline uint32 GetStride() const { return m_Stride; }

		std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
		std::vector<BufferElement>::iterator end() { return m_Elements.end(); }
		std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
		std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }
	private:
		void CalculateOffsetAndStride()
		{
			uint32 offset = 0;
			for (BufferElement& element : m_Elements)
			{
				element.Offset = offset;
				offset += element.Size;
				m_Stride += element.Size;
			}
		}
	private:
		std::vector<BufferElement> m_Elements;
		uint32 m_Stride = 0;
	};

	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetLayout(const BufferLayout& layout) = 0;
		virtual const BufferLayout& GetLayout() const = 0;

		virtual void SetData(const void* data, uint32 size) = 0;

		static Ref<VertexBuffer> Create(uint32 size);
		static Ref<VertexBuffer> Create(float* vertices, uint32 size);
	};

	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual uint32 GetCount() const = 0;

		static Ref<IndexBuffer> Create(uint32* indices, uint32 count);
	};
}