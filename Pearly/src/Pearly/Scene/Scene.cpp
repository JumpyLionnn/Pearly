#include "prpch.h"
#include "Scene.h"

#include "Entity.h"
#include "Components.h"
#include "Pearly/Renderer/Renderer.h"

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

namespace Pearly {
	Scene::~Scene()
	{

	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity = { m_Registry.create(), this };
		entity.AddComponent<TransformComponent>();
		entity.AddComponent<TagComponent>(name);
		return entity;
	}

	void Scene::DestroyEntity(Entity entity)
	{
		m_Registry.destroy(entity);
	}

	void Scene::OnUpdate(Timestep ts)
	{
		PR_PROFILE_FUNCTION();
		// script updating
		/////////////////////
		{
			m_Registry.view<NativeScriptComponent>().each([=](entt::entity entity, NativeScriptComponent& script) 
			{
					if (!script.Instance)
					{
						script.Instance = script.InstantiateScript();
						script.Instance->m_Entity = Entity{ entity, this };
						script.Instance->OnStart();
					}

					script.Instance->OnUpdate(ts);	
			});
		}

		// Rendering
		/////////////////////
		Camera* primaryCamera = nullptr;
		glm::mat4 primaryCameraTransform;
		{
			auto view = m_Registry.view<CameraComponent, TransformComponent>();
			for (entt::entity entity : view)
			{
				auto[camera, transform] = view.get<CameraComponent, TransformComponent>(entity);
				if (camera.Primary)
				{
					primaryCamera = &camera.Camera;
					primaryCameraTransform = transform.GetTransform();
					break;
				}
			}
			
		}
		
		if (primaryCamera)
		{
			Renderer::BeginScene(*primaryCamera, glm::inverse(primaryCameraTransform));
			auto view = m_Registry.view<SpriteRendererComponent, TransformComponent>();
			for (entt::entity entity : view)
			{
				auto [sprite, transform] = view.get<SpriteRendererComponent, TransformComponent>(entity);

				Renderer::DrawQuad(transform.GetTransform(), sprite.Color);
			}
			Renderer::EndScene();
		}
		
	}

	void Scene::OnViewportResize(uint32 width, uint32 height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;

		auto view = m_Registry.view<CameraComponent>();
		for (entt::entity entity : view)
		{
			CameraComponent& camera = view.get<CameraComponent>(entity);
			if (!camera.FixedAspectRatio)
			{
				camera.Camera.SetViewportSize(width, height);
			}
		}
	}

	Entity Scene::GetPrimaryCameraEntity()
	{
		auto view = m_Registry.view<CameraComponent>();
		for (entt::entity entity : view)
		{
			auto& camera = view.get<CameraComponent>(entity);
			if (camera.Primary)
			{
				return Entity(entity, this);
			}
		}
		return {};
	}

	template<typename T>
	void Scene::OnComponentAdded(Entity entity, T& component)
	{
		static_assert(false);
	}

	template<>
	void Scene::OnComponentAdded<TagComponent>(Entity entity, TagComponent& component)
	{}
	template<>
	void Scene::OnComponentAdded<TransformComponent>(Entity entity, TransformComponent& component)
	{}
	template<>
	void Scene::OnComponentAdded<CameraComponent>(Entity entity, CameraComponent& component)
	{
		component.Camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);
	}
	template<>
	void Scene::OnComponentAdded<SpriteRendererComponent>(Entity entity, SpriteRendererComponent& component)
	{}
	template<>
	void Scene::OnComponentAdded<NativeScriptComponent>(Entity entity, NativeScriptComponent& component)
	{}

}