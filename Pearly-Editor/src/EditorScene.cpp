#include "EditorScene.h"


namespace Pearly {

	EditorScene::EditorScene()
		: m_Camera(1.667f, -1.0f, 1.0f)
	{}

	void EditorScene::OnUpdate(Timestep ts)
	{
		m_Camera.OnUpdate(ts);
		Renderer::BeginScene(m_Camera, m_Camera.GetView());
		auto view = m_Registry.view<SpriteRendererComponent, TransformComponent>();
		for (entt::entity entity : view)
		{
			auto [sprite, transform] = view.get<SpriteRendererComponent, TransformComponent>(entity);

			glm::mat4 transformMatrix = glm::translate(glm::mat4(1.0f), { transform.Position.x , transform.Position.y, sprite.ZIndex })*
				glm::rotate(glm::mat4(1.0f), glm::radians(transform.Rotation), { 0.0f, 0.0f, 1.0f })*
				glm::scale(glm::mat4(1.0f), { transform.Scale.x , transform.Scale.y, 1.0f });
			
			Renderer::DrawSprite(transformMatrix, sprite, (int)entity);
		}
		Renderer::EndScene();
	}

	void EditorScene::OnEvent(Event& event)
	{
		m_Camera.OnEvent(event);
	}

	void EditorScene::OnViewportResize(uint32 width, uint32 height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;

		m_Camera.SetViewportSize((float)width, (float)height);
	}
}