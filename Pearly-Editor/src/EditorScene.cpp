#include "EditorScene.h"

namespace Pearly {
	EditorScene::EditorScene()
		: m_Camera(1.667, -1.0f, 1.0f)
	{}

	void EditorScene::OnUpdate(Timestep ts)
	{
		m_Camera.OnUpdate(ts);
		Renderer::BeginScene(m_Camera, m_Camera.GetView());
		auto view = m_Registry.view<SpriteRendererComponent, TransformComponent>();
		for (entt::entity entity : view)
		{
			auto [sprite, transform] = view.get<SpriteRendererComponent, TransformComponent>(entity);

			Renderer::DrawQuad(transform.GetTransform(), sprite.Color);
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

		m_Camera.SetViewportSize(width, height);
	}

}