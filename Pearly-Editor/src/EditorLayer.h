#pragma once
#include <Pearly.h>
#include "Panels/SceneHierarchyPanel.h"

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
		bool OnKeyPressed(KeyPressedEvent& event);

		void NewScene();
		void OpenScene();
		void SaveScene();
		void SaveAsScene();
	private:
		SpriteSheet m_SpriteSheet;
		Ref<Texture2D> m_CheckerboardTexture;

		Ref<SubTexture2D> m_LilyPadTexture;
		Ref<SubTexture2D> m_BigTreeTexture;

		Ref<Scene> m_ActiveScene;
		std::string m_CurrentSceneFilePath;

		Entity m_SquareEntity;
		Entity m_CameraEntity;

		Ref<FrameBuffer> m_FrameBuffer;
		glm::vec2 m_ViewportSize = { 0, 0 };

		OrthographicCameraController m_CameraController;

		bool m_ViewportFocused = false;
		bool m_ViewportHovered = false;

		glm::vec4 m_Color = { 0.2f, 0.3f, 0.8f, 1.0f };

		SceneHierarchyPanel m_SceneHierarchyPanel;

	};

}