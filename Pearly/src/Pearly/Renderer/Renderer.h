#pragma once
#include "RenderCommand.h"
#include "OrthographicCamera.h"
#include "Shader.h"
#include "Texture.h"

namespace Pearly {

	struct TransformProperties
	{
		glm::vec3 Position;
		glm::vec2 Scale;
		float Rotation;
		TransformProperties(glm::vec3 position = glm::vec3(0.0f), glm::vec2 scale = glm::vec2(1.0f), float rotation = 0.0f)
			: Position(position), Scale(scale), Rotation(rotation)
		{}
		TransformProperties(glm::vec2 position = glm::vec2(0.0f), glm::vec2 scale = glm::vec2(1.0f), float rotation = 0.0f)
			: Position({ position.x, position.y, 0.0f }), Scale(scale), Rotation(rotation)
		{}
	};

	class Renderer
	{
	public:
		static void Init();
		static void Shutdown();
		static void OnWindowResize(uint32 width, uint32 height);

		static void BeginScene(const OrthographicCamera& camera); 
		static void EndScene();

		static void DrawQuad(const TransformProperties& transformProperties, const glm::vec4& color);
		static void DrawQuad(const TransformProperties& transformProperties, Ref<Texture2D> texture, const glm::vec4& tint = glm::vec4(1.0f), float tilingFactor = 1.0f);

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); };
	private:
		static glm::mat4 CanculateTransformMatrix(const TransformProperties& transformProperties);
	};

}