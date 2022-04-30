#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Pearly/Renderer/Camera.h"
#include "Pearly/Core/Timestep.h"
#include "SceneCamera.h"
#include "ScriptableEntity.h"

namespace Pearly {

	struct TransformComponent
	{
		glm::vec2 Position = {0.0f, 0.0f};
		float Rotation = 0.0f;
		glm::vec2 Scale = { 1.0f, 1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::vec2& position)
			: Position(position)
		{}

		glm::mat4 GetTransform() const
		{
			// TODO: add z-indexing in the rendering components
			return glm::translate(glm::mat4(1.0f), { Position.x , Position.y, 0.0f }) * 
				glm::rotate(glm::mat4(1.0f), glm::radians(Rotation), { 0.0f, 0.0f, 1.0f }) * 
				glm::scale(glm::mat4(1.0f), { Scale.x , Scale.y, 1.0f });
		}
	};

	struct TagComponent
	{
		std::string Tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag)
			: Tag(tag)
		{}
	};

	struct SpriteRendererComponent
	{
		glm::vec4 Color = glm::vec4(1.0f);

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const glm::vec4& color)
			: Color(color)
		{}
	};

	struct CameraComponent
	{
		SceneCamera Camera;
		bool Primary = false;
		bool FixedAspectRatio = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
	};

	struct NativeScriptComponent
	{
		ScriptableEntity* Instance = nullptr;

		ScriptableEntity*(*InstantiateScript)();
		void(*DestroyScript)(NativeScriptComponent*);



		template<typename T>
		void Bind()
		{
			PR_PROFILE_FUNCTION();
			InstantiateScript = []() { return (ScriptableEntity*)(new T()); };
			DestroyScript = [](NativeScriptComponent* script) { delete script->Instance; script->Instance = nullptr; };
		}
	};
}