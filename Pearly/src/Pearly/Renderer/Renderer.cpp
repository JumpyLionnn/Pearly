#include "prpch.h"
#include "Renderer.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Pearly {

	struct RendererStorage
	{
		Ref<VertexArray> QuadVertexArray;
		Ref<Shader> TextureShader;
		Ref<Texture2D> WhiteTexture;
	};

	static Scope<RendererStorage> s_Data;

	void Renderer::Init()
	{
		PR_PROFILE_FUNCTION();
		s_Data = CreateScope<RendererStorage>();
		RenderCommand::Init();

		s_Data->QuadVertexArray = VertexArray::Create();

		float squareVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
		};
		Ref<VertexBuffer> squareVertexBuffer = VertexBuffer::Create(squareVertices, sizeof(squareVertices));
		BufferLayout squareLayout = {
			{ShaderDataType::Vec3f, "a_Position"},
			{ShaderDataType::Vec2f, "a_TexCoord"},
		};
		squareVertexBuffer->SetLayout(squareLayout);
		s_Data->QuadVertexArray->AddVertexBuffer(squareVertexBuffer);

		unsigned int squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		Ref<IndexBuffer> squareIndexBuffer = IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(unsigned int));
		s_Data->QuadVertexArray->SetIndexBuffer(squareIndexBuffer);

		s_Data->WhiteTexture = Texture2D::Create(1, 1);
		uint32 texData = 0xFFFFFFFF;
		s_Data->WhiteTexture->SetData(&texData, sizeof(texData));
		
		s_Data->TextureShader = Shader::Create("assets/shaders/Texture.glsl");
		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetInt("u_Texture", 0);
	}
	void Renderer::Shutdown()
	{
		PR_PROFILE_FUNCTION();
		delete s_Data.release();
	}

	void Renderer::OnWindowResize(uint32 width, uint32 height)
	{
		RenderCommand::SetViewport(0, 0, width, height);
	}


	void Renderer::BeginScene(const OrthographicCamera& camera)
	{
		PR_PROFILE_FUNCTION();
		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
	}

	void Renderer::EndScene()
	{

	}

	void Renderer::DrawQuad(const TransformProperties& transformProperties, const glm::vec4& color)
	{
		DrawQuad(transformProperties, s_Data->WhiteTexture, color, 1.0f);
	}

	void Renderer::DrawQuad(const TransformProperties& transformProperties, Ref<Texture2D> texture, const glm::vec4& tint, float tilingFactor)
	{
		PR_PROFILE_FUNCTION();
		s_Data->TextureShader->SetVec4f("u_Color", tint);
		s_Data->TextureShader->SetFloat("u_TilingFactor", tilingFactor);
		glm::mat4 transform = CanculateTransformMatrix(transformProperties);
		s_Data->TextureShader->SetMat4("u_Transform", transform);

		texture->Bind();

		s_Data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
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