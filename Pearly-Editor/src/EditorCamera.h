#pragma once
#include <Pearly.h>

namespace Pearly {

	class EditorCamera : public Camera
	{
	public:
		EditorCamera() = default;
		EditorCamera(float aspectRatio, float nearClip, float farClip);

		glm::mat4 GetView() const { return m_View; }

		inline void SetViewportSize(float width, float height) { m_ViewportWidth = width; m_ViewportHeight = height; UpdateProjection(); }

		void OnUpdate(Timestep ts);

		void OnEvent(Event& event);
	private:
		void UpdateProjection();
		void UpdateView();
		bool OnMouseScrolled(MouseScrolledEvent& event);
		bool OnMouseButtonPressed(MouseButtonPressedEvent& event);

		float ZoomSpeed() const;
	private:
		OrthographicCameraBounds m_Bounds;
		glm::mat4 m_View = glm::mat4(1.0f);

		glm::vec2 m_Position = glm::vec2(0.0f);
		float m_Distance = 4.0f;

		glm::vec2 m_InitialMousePosition = glm::vec2(0.0f);

		float m_ViewportWidth = 1280;
		float m_ViewportHeight = 720;

		float m_AspectRatio = 1.667f; 
		float m_NearClip = -1.0f;
		float m_FarClip = 1.0f;
	};

}