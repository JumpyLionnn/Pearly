#include "EditorCamera.h"
#include "imgui/imgui.h"

namespace Pearly {

	static constexpr float s_MinZoomSpeed = 0.1f;
	static constexpr float s_MaxZoomSpeed = 100.0f;

	EditorCamera::EditorCamera(float aspectRatio, float nearClip, float farClip)
		: Camera(glm::ortho(-aspectRatio, aspectRatio, nearClip, farClip, m_NearClip, m_FarClip)), m_AspectRatio(aspectRatio), m_NearClip(nearClip), m_FarClip(farClip)
	{
		UpdateView();
	}

	void EditorCamera::OnUpdate(Timestep ts)
	{
		if (Input::IsMouseButtonPressed(PR_MOUSE_BUTTON_RIGHT))
		{
			// TODO: improve precision of the movment to be perfect on all window sizes
			glm::vec2 mousePosition{ Input::GetMouseX(), Input::GetMouseY() };
			glm::vec2 delta = (mousePosition - m_InitialMousePosition) * 0.015f;
			m_InitialMousePosition = mousePosition;

			float x = std::min(m_ViewportWidth / 1000.0f, 2.4f); // max = 2.4f
			float xFactor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;

			float y = std::min(m_ViewportHeight / 1000.0f, 2.4f); // max = 2.4f
			float yFactor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;

			m_Position += glm::vec2(delta.x * xFactor, delta.y * yFactor) * glm::vec2(-1.0f, 1.0f) * m_Distance;
			UpdateView();
		}

	}

	void EditorCamera::OnEvent(Event& event)
	{
		EventDispacher dispacher(event);
		dispacher.Dispatch<MouseScrolledEvent>(PR_BIND_EVENT_FN(EditorCamera::OnMouseScrolled));
		dispacher.Dispatch<MouseButtonPressedEvent>(PR_BIND_EVENT_FN(EditorCamera::OnMouseButtonPressed));
	}

	void EditorCamera::UpdateProjection()
	{
		m_AspectRatio = m_ViewportWidth / m_ViewportHeight;
		m_Projection = glm::ortho(-m_AspectRatio * m_Distance, m_AspectRatio * m_Distance, -m_Distance, m_Distance, m_NearClip, m_FarClip);
	}

	void EditorCamera::UpdateView()
	{
		m_View = glm::translate(glm::mat4(1.0f), glm::vec3(m_Position, 0.0f));
		m_View = glm::inverse(m_View);
	}

	bool EditorCamera::OnMouseScrolled(MouseScrolledEvent& event)
	{
		m_Distance -= event.GetYOffset() * ZoomSpeed();
		m_Distance = std::max(m_Distance, 0.1f);
		UpdateProjection();
		return false;
	}

	bool EditorCamera::OnMouseButtonPressed(MouseButtonPressedEvent& event)
	{
		m_InitialMousePosition = { Input::GetMouseX(), Input::GetMouseY() };
		return false;
	}

	float EditorCamera::ZoomSpeed() const
	{
		return std::max(std::min(m_Distance * 0.08f, s_MaxZoomSpeed), s_MinZoomSpeed);
	}
}