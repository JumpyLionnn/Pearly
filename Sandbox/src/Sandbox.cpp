#include <Pearly.h>
#include "Platform/OpenGL/OpenGLShader.h"

#include <imgui/imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class ExampleLayer : public Pearly::Layer
{
public:
	ExampleLayer()
		: Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f)
	{
		m_VertexArray.reset(Pearly::VertexArray::Create());
		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
			0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
			0.0f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		};
		m_VertexBuffer.reset(Pearly::VertexBuffer::Create(vertices, sizeof(vertices)));
		Pearly::BufferLayout layout = {
			{Pearly::ShaderDataType::Vec3f, "a_Position"},
			{Pearly::ShaderDataType::Vec4f, "a_Color"},
		};
		m_VertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(m_VertexBuffer);

		unsigned int indices[3] = { 0, 1, 2 };
		m_IndexBuffer.reset(Pearly::IndexBuffer::Create(indices, sizeof(indices) / sizeof(unsigned int)));
		m_VertexArray->SetIndexBuffer(m_IndexBuffer);


		m_SquareVertexArray.reset(Pearly::VertexArray::Create());

		float squareVertices[3 * 4] = {
			-0.5f, -0.5f, 0.0f, 
			0.5f, -0.5f, 0.0f, 
			-0.5f, 0.5f, 0.0f, 
			0.5f, 0.5f, 0.0f, 
		};
		m_SquareVertexBuffer.reset(Pearly::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		Pearly::BufferLayout squareLayout = {
			{Pearly::ShaderDataType::Vec3f, "a_Position"}
		};
		m_SquareVertexBuffer->SetLayout(squareLayout);
		m_SquareVertexArray->AddVertexBuffer(m_SquareVertexBuffer);

		unsigned int squareIndices[6] = { 0, 1, 2, 1, 2, 3 };
		m_SquareIndexBuffer.reset(Pearly::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(unsigned int)));
		m_SquareVertexArray->SetIndexBuffer(m_SquareIndexBuffer);

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

		m_Shader.reset(Pearly::Shader::Create(vertexSource, fragmentSource));

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

		m_FlatColorShader.reset(Pearly::Shader::Create(flatColorVertexSource, flatColorFragmentSource));
	}

	void OnUpdate(Pearly::Timestep ts) override
	{
		PR_TRACE("Delta time: {0}, ({1}ms)", ts, ts.GetMiliseconds());

		if (Pearly::Input::IsKeyPressed(PR_KEY_A))
			m_CameraPosition.x -= m_CameraMovementSpeed * ts;
		if (Pearly::Input::IsKeyPressed(PR_KEY_D))
			m_CameraPosition.x += m_CameraMovementSpeed * ts;
		if (Pearly::Input::IsKeyPressed(PR_KEY_W))
			m_CameraPosition.y += m_CameraMovementSpeed * ts;
		if (Pearly::Input::IsKeyPressed(PR_KEY_S))
			m_CameraPosition.y -= m_CameraMovementSpeed * ts;

		if (Pearly::Input::IsKeyPressed(PR_KEY_LEFT))
			m_CameraRotation += m_CameraRotationSpeed * ts;
		if (Pearly::Input::IsKeyPressed(PR_KEY_RIGHT))
			m_CameraRotation -= m_CameraRotationSpeed * ts;


		Pearly::RenderCommand::SetClearColor(glm::vec4(0.32f, 0.42f, 0.52f, 1.0f));
		Pearly::RenderCommand::Clear();

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);

		Pearly::Renderer::BeginScene(m_Camera);

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

		Pearly::Renderer::Submit(m_VertexArray, m_Shader);

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
		
	}
private:
	Pearly::Ref<Pearly::VertexArray> m_VertexArray;
	Pearly::Ref<Pearly::VertexBuffer> m_VertexBuffer;
	Pearly::Ref<Pearly::IndexBuffer> m_IndexBuffer;
	Pearly::Ref<Pearly::Shader> m_Shader;
	Pearly::Ref<Pearly::Shader> m_FlatColorShader;

	Pearly::Ref<Pearly::VertexArray> m_SquareVertexArray;
	Pearly::Ref<Pearly::VertexBuffer> m_SquareVertexBuffer;
	Pearly::Ref<Pearly::IndexBuffer> m_SquareIndexBuffer;

	Pearly::OrthographicCamera m_Camera;
	glm::vec3 m_CameraPosition = {0.0f, 0.0f, 0.0f};
	float m_CameraRotation = 0.0f;

	float m_CameraMovementSpeed = 1.0f;
	float m_CameraRotationSpeed = 135.0f;

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