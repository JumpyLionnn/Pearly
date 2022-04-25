#pragma once
#include "RenderCommand.h"
#include "OrthographicCamera.h"
#include "Shader.h"
#include "Texture.h"
#include "SubTexture.h"

namespace Pearly {

	class Renderer
	{
	public:

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

		static void Init();
		static void Shutdown();
		static void OnWindowResize(uint32 width, uint32 height);

		static void BeginScene(const OrthographicCamera& camera); 
		static void EndScene();
		static void Flush();

		static void DrawQuad(const TransformProperties& transformProperties, const glm::vec4& color);
		static void DrawQuad(const TransformProperties& transformProperties, Ref<Texture2D> texture, const glm::vec4& tint = glm::vec4(1.0f), float tilingFactor = 1.0f);
		static void DrawQuad(const TransformProperties& transformProperties, Ref<SubTexture2D> subTexture, const glm::vec4& tint = glm::vec4(1.0f), float tilingFactor = 1.0f);

		// stats
		struct Statistics
		{
			uint32 DrawCalls = 0;
			uint32 QuadCount = 0;

			uint32 GetTotalVertexCount() { return QuadCount * 4; }
			uint32 GetTotalIndexCount() { return QuadCount * 6; }
		};

		static void ResetStats();
		static Statistics GetStats();

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); };
	private:
		static float GetTextureIndex(const Ref<Texture2D>& texture);

		static constexpr std::array<glm::vec2, 4> defaultTextureCoords{ glm::vec2( 0.0f, 0.0f), glm::vec2(1.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec2(0.0f, 1.0f) };
		static void SubmitQuad(const TransformProperties& transformProperties, uint32 textureIndex, const glm::vec4& color, float tilingFactor, const std::array<glm::vec2, 4>& textureCoords = defaultTextureCoords);

		static glm::mat4 CanculateTransformMatrix(const TransformProperties& transformProperties);

		static void FlushAndReset();
	};

}