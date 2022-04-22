#pragma once
#include "RenderCommand.h"
#include "OrthographicCamera.h"
#include "Shader.h"
#include "Texture.h"

namespace Pearly {

	class Renderer
	{
	public:
		static void Init();
		static void Shutdown();
		static void OnWindowResize(uint32 width, uint32 height);

		static void BeginScene(const OrthographicCamera& camera); 
		static void EndScene();

		static void DrawQuad(const glm::vec2& position, const glm::vec2& scale, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& scale, const glm::vec4& color);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& scale, Ref<Texture2D> texture);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& scale, Ref<Texture2D> texture);

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); };
	};

}