#include "prpch.h"
#include "Renderer.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Pearly {

	struct QuadVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;
		float TexIndex;
		float TilingFactor;
	};

	struct RendererData
	{
		static const uint32 MaxQuads = 10000;
		static const uint32 MaxVertices = MaxQuads * 4;
		static const uint32 MaxIndices = MaxQuads * 6;
		static const uint32 MaxTextureSlots = 32; //TODO: query the gpu for its max texture slots using RenderCapabilities

		Ref<VertexArray> QuadVertexArray;
		Ref<VertexBuffer> QuadVertexBuffer;
		Ref<Shader> TextureShader;
		Ref<Texture2D> WhiteTexture;

		uint32 QuadIndexCount = 0;
		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;

		std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;
		uint32 TextureSlotIndex = 1; // slot 0 is a white texture

		glm::vec4 QuadVertexPositions[4];

		#if PR_ENABLE_RENDERER_STATS
		Renderer::Statistics Stats;
		#endif
	};

	static RendererData s_Data;

	void Renderer::Init()
	{
		PR_PROFILE_FUNCTION();
		RenderCommand::Init();
		RenderCommand::Init();

		s_Data.QuadVertexArray = VertexArray::Create();

		s_Data.QuadVertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(QuadVertex));
		s_Data.QuadVertexBuffer->SetLayout({
			{ShaderDataType::Vec3f, "a_Position"},
			{ShaderDataType::Vec4f, "a_Color"},
			{ShaderDataType::Vec2f, "a_TexCoord"},
			{ShaderDataType::Float, "a_TexIndex"},
			{ShaderDataType::Float, "a_TilingFactor"},
			});
		s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);

		s_Data.QuadVertexBufferBase = new QuadVertex[s_Data.MaxVertices];

		uint32* quadIndices = new uint32[s_Data.MaxIndices];

		uint32 offset = 0;
		for (uint32 i = 0; i < s_Data.MaxIndices; i += 6)
		{
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;

			offset += 4;
		}

		Ref<IndexBuffer> quadIndexBuffer = IndexBuffer::Create(quadIndices, s_Data.MaxIndices);
		s_Data.QuadVertexArray->SetIndexBuffer(quadIndexBuffer);
		delete[] quadIndices;

		s_Data.WhiteTexture = Texture2D::Create(1, 1);
		uint32 texData = 0xFFFFFFFF;
		s_Data.WhiteTexture->SetData(&texData, sizeof(texData));

		s_Data.TextureSlots[0] = s_Data.WhiteTexture;

		int samplers[s_Data.MaxTextureSlots];
		for (int i = 0; i < s_Data.MaxTextureSlots; i++)
		{
			samplers[i] = i;
		}
		
		s_Data.TextureShader = Shader::Create("assets/shaders/Texture.glsl");
		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetIntArray("u_Textures", samplers, s_Data.MaxTextureSlots);

		

		s_Data.QuadVertexPositions[0] = { -0.5f, -0.5, 0.0f, 1.0f };
		s_Data.QuadVertexPositions[1] = { 0.5f, -0.5, 0.0f, 1.0f };
		s_Data.QuadVertexPositions[2] = { 0.5f, 0.5, 0.0f, 1.0f };
		s_Data.QuadVertexPositions[3] = { -0.5f, 0.5, 0.0f, 1.0f };
	}
	void Renderer::Shutdown()
	{
		PR_PROFILE_FUNCTION();
		delete[] s_Data.QuadVertexBufferBase;
	}

	void Renderer::OnWindowResize(uint32 width, uint32 height)
	{
		RenderCommand::SetViewport(0, 0, width, height);
	}


	void Renderer::BeginScene(const OrthographicCamera& camera)
	{
		PR_PROFILE_FUNCTION();
		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

		s_Data.QuadIndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

		s_Data.TextureSlotIndex = 1;
	}

	void Renderer::EndScene()
	{
		PR_PROFILE_FUNCTION();

		uint32 dataSize = (uint8*)s_Data.QuadVertexBufferPtr - (uint8*)s_Data.QuadVertexBufferBase;
		s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase, dataSize);

		Flush();
	}

	void Renderer::Flush()
	{
		PR_PROFILE_FUNCTION();
		if (s_Data.QuadIndexCount == 0)
			return;

		for (uint32 i = 0; i < s_Data.TextureSlotIndex; i++)
		{
			s_Data.TextureSlots[i]->Bind(i);
		}
		RenderCommand::DrawIndexed(s_Data.QuadVertexArray, s_Data.QuadIndexCount);

		#if PR_ENABLE_RENDERER_STATS
		s_Data.Stats.DrawCalls++;
		#endif
	}

	void Renderer::FlushAndReset()
	{
		PR_PROFILE_FUNCTION();
		EndScene();

		s_Data.QuadIndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

		s_Data.TextureSlotIndex = 1;
	}

	void Renderer::DrawQuad(const TransformProperties& transformProperties, const glm::vec4& color)
	{
		PR_PROFILE_FUNCTION();

		constexpr float textureIndex = 0.0f; // white texture
		SubmitQuad(transformProperties, textureIndex, color, 1.0f);
	}

	void Renderer::DrawQuad(const TransformProperties& transformProperties, Ref<Texture2D> texture, const glm::vec4& tint, float tilingFactor)
	{
		PR_PROFILE_FUNCTION();

		float textureIndex = GetTextureIndex(texture);
		SubmitQuad(transformProperties, textureIndex, tint, tilingFactor);
	}

	void Renderer::DrawQuad(const TransformProperties& transformProperties, Ref<SubTexture2D> subTexture, const glm::vec4& tint, float tilingFactor)
	{
		PR_PROFILE_FUNCTION();

		float textureIndex = GetTextureIndex(subTexture->GetTexture());
		SubmitQuad(transformProperties, textureIndex, tint, tilingFactor, subTexture->GetTexCoords());
	}

	void Renderer::ResetStats()
	{
		#if PR_ENABLE_RENDERER_STATS
		PR_PROFILE_FUNCTION();
		memset(&s_Data.Stats, 0, sizeof(Statistics));
		#endif
	}
	Renderer::Statistics Renderer::GetStats()
	{
		#if PR_ENABLE_RENDERER_STATS
		return s_Data.Stats;
		#else
		return {};
		#endif
	}

	float Renderer::GetTextureIndex(const Ref<Texture2D>& texture)
	{
		float textureIndex = 0.0f;

		for (uint32 i = 1; i < s_Data.TextureSlotIndex; i++)
		{
			if (*s_Data.TextureSlots[i].get() == *texture.get())
			{
				textureIndex = (float)i;
			}
		}

		if (textureIndex == 0.0f)
		{
			if (s_Data.TextureSlotIndex >= RendererData::MaxTextureSlots)
				FlushAndReset();
			textureIndex = (float)s_Data.TextureSlotIndex;
			s_Data.TextureSlots[s_Data.TextureSlotIndex++] = texture;
		}
		return textureIndex;
	}

	void Renderer::SubmitQuad(const TransformProperties& transformProperties, uint32 textureIndex, const glm::vec4& color, float tilingFactor, const std::array<glm::vec2, 4>& textureCoords)
	{
		PR_PROFILE_FUNCTION();
		if (s_Data.QuadIndexCount >= RendererData::MaxIndices)
			FlushAndReset();

		glm::mat4 transform = CanculateTransformMatrix(transformProperties);

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[0];
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[0];
		s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[1];
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[1];
		s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[2];
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[2];
		s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[3];
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[3];
		s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadIndexCount += 6;

		#if PR_ENABLE_RENDERER_STATS
		s_Data.Stats.QuadCount++;
		#endif
	}
	
	glm::mat4 Renderer::CanculateTransformMatrix(const TransformProperties& transformProperties)
	{
		PR_PROFILE_FUNCTION();
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), transformProperties.Position);
		if (transformProperties.Rotation != 0)
		{
			transform = glm::rotate(transform, transformProperties.Rotation, { 0.0f, 0.0f, 1.0f });
		}
		return glm::scale(transform, { transformProperties.Scale.x, transformProperties.Scale.y, 1.0f });
	}
	
}