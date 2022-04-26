#pragma once
#include <Pearly.h>

namespace Pearly {

	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate(Timestep ts) override;
		virtual void OnEvent(Event& event) override;
		virtual void OnImGuiRender() override;
	private:
		SpriteSheet m_SpriteSheet;
		Ref<Texture2D> m_CheckerboardTexture;

		Ref<SubTexture2D> m_LilyPadTexture;
		Ref<SubTexture2D> m_BigTreeTexture;

		Ref<FrameBuffer> m_FrameBuffer;
		glm::vec2 m_ViewportSize = { 0, 0 };

		OrthographicCameraController m_CameraController;

		glm::vec4 m_Color = { 0.2f, 0.3f, 0.8f, 1.0f };
	};

}