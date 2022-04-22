#include <Pearly.h>
#include "Platform/OpenGL/OpenGLShader.h"
#include "Pearly/Renderer/Shader.h"

#include <imgui/imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class ExampleLayer : public Pearly::Layer
{
public:
	ExampleLayer()
		: Layer("Example"), m_CameraController(1280.0f/720.0f, true)
	{
		m_VertexArray.reset(Pearly::VertexArray::Create());
		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
			0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
			0.0f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		};
		Pearly::Ref<Pearly::VertexBuffer> vertexBuffer = Pearly::VertexBuffer::Create(vertices, sizeof(vertices));
		Pearly::BufferLayout layout = {
			{Pearly::ShaderDataType::Vec3f, "a_Position"},
			{Pearly::ShaderDataType::Vec4f, "a_Color"},
		};
		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		unsigned int indices[3] = { 0, 1, 2 };
		Pearly::Ref<Pearly::IndexBuffer> indexBuffer = Pearly::IndexBuffer::Create(indices, sizeof(indices) / sizeof(unsigned int));
		m_VertexArray->SetIndexBuffer(indexBuffer);


		m_SquareVertexArray.reset(Pearly::VertexArray::Create());

		float squareVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f, 0.5f, 0.0f, 0.0f, 1.0f
		};
		Pearly::Ref<Pearly::VertexBuffer> squareVertexBuffer = Pearly::VertexBuffer::Create(squareVertices, sizeof(squareVertices));
		Pearly::BufferLayout squareLayout = {
			{Pearly::ShaderDataType::Vec3f, "a_Position"},
			{Pearly::ShaderDataType::Vec2f, "a_TexCoord"},
		};
		squareVertexBuffer->SetLayout(squareLayout);
		m_SquareVertexArray->AddVertexBuffer(squareVertexBuffer);

		unsigned int squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		Pearly::Ref<Pearly::IndexBuffer> squareIndexBuffer = Pearly::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(unsigned int));
		m_SquareVertexArray->SetIndexBuffer(squareIndexBuffer);

		std::string vertexSource = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Postion;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec4 v_Color;
			
			void main()
			{
				v_Color = a_Color;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Postion, 1.0);
			}

		)";
		std::string fragmentSource = R"(
			#version 330 core

			layout(location = 0) out vec4 Color;
			
			in vec4 v_Color;

			void main()
			{
				Color = v_Color;
			}
		)";

		m_Shader = Pearly::Shader::Create("triangle", vertexSource, fragmentSource);

		std::string flatColorVertexSource = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Postion;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;
			
			void main()
			{
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Postion, 1.0);
			}

		)";
		std::string flatColorFragmentSource = R"(
			#version 330 core

			uniform vec3 u_Color;

			layout(location = 0) out vec4 Color;

			void main()
			{
				Color = vec4(u_Color, 1.0f);
			}
		)";

		m_FlatColorShader = Pearly::Shader::Create("flatColorShader", flatColorVertexSource, flatColorFragmentSource);


		Pearly::Ref<Pearly::Shader> textureShader = m_ShaderLibrary.Load("assets/shaders/texture.glsl");

		m_CrateTexture = Pearly::Texture2D::Create("assets/textures/Checkerboard.png");
		m_JumpyLionLogoTexture = Pearly::Texture2D::Create("assets/textures/jumpylion.png");

		std::dynamic_pointer_cast<Pearly::OpenGLShader>(textureShader)->Bind();
		std::dynamic_pointer_cast<Pearly::OpenGLShader>(textureShader)->UploadUnifromInt("u_Texture", 0);
	}

	void OnUpdate(Pearly::Timestep ts) override
	{
		Pearly::RenderCommand::SetClearColor(glm::vec4(0.32f, 0.42f, 0.52f, 1.0f));
		Pearly::RenderCommand::Clear();

		m_CameraController.OnUpdate(ts);

		Pearly::Renderer::BeginScene(m_CameraController.GetCamera());

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		glm::vec4 redColor(0.8f, 0.2f, 0.3f, 1.0f);
		glm::vec4 blueColor(0.3f, 0.2f, 0.8f, 1.0f);

		std::dynamic_pointer_cast<Pearly::OpenGLShader>(m_FlatColorShader)->Bind();
		std::dynamic_pointer_cast<Pearly::OpenGLShader>(m_FlatColorShader)->UploadUnifromVec3f("u_Color", m_Color);
		
		for (int y = 0; y < 20; y++)
		{
			for (int x = 0; x < 20; x++)
			{
				glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				Pearly::Renderer::Submit(m_SquareVertexArray, m_FlatColorShader, transform);
			}
		}


		Pearly::Ref<Pearly::Shader> textureShader = m_ShaderLibrary.Get("texture");

		m_CrateTexture->Bind();
		Pearly::Renderer::Submit(m_SquareVertexArray, textureShader, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
		m_JumpyLionLogoTexture->Bind();
		Pearly::Renderer::Submit(m_SquareVertexArray, textureShader, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));


		//Pearly::Renderer::Submit(m_VertexArray, m_Shader);

		Pearly::Renderer::EndScene();
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Square Color", glm::value_ptr(m_Color));
		ImGui::End();
	}	

	virtual void OnEvent(Pearly::Event& event) override
	{
		Pearly::EventDispacher dispacher(event);
		m_CameraController.OnEvent(event);
	}
private:
	Pearly::ShaderLibrary m_ShaderLibrary;
	Pearly::Ref<Pearly::VertexArray> m_VertexArray;
	Pearly::Ref<Pearly::Shader> m_Shader;
	Pearly::Ref<Pearly::Shader> m_FlatColorShader;

	Pearly::Ref<Pearly::VertexArray> m_SquareVertexArray;

	Pearly::Ref<Pearly::Texture2D> m_CrateTexture;
	Pearly::Ref<Pearly::Texture2D> m_JumpyLionLogoTexture;

	Pearly::OrthographicCameraController m_CameraController;

	glm::vec3 m_Color = {0.2f, 0.3f, 0.8f};
};





class Sandbox : public Pearly::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
	}
	~Sandbox()
	{

	}

private:

};
Pearly::Application* Pearly::CreateApplication()
{
	return new Sandbox();
}