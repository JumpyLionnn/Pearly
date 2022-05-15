#include "EditorCamera.h"
#include "imgui/imgui.h"

namespace Pearly {

	static constexpr float s_MinZoomSpeed = 0.1f;
	static constexpr float s_MaxZoomSpeed = 100.0f;

	EditorCamera::EditorCamera(float aspectRatio, float nearClip, float farClip)
		: Camera(glm::ortho(-aspectRatio * m_Distance, aspectRatio* m_Distance, -m_Distance, m_Distance, nearClip, farClip)), 
			m_Bounds(-aspectRatio * m_Distance, aspectRatio * m_Distance, -m_Distance, m_Distance),
			m_AspectRatio(aspectRatio),
			m_NearClip(nearClip),
			m_FarClip(farClip)
	{
		UpdateView();
	}

	void EditorCamera::OnUpdate(Timestep ts)
	{
		if (Input::IsMouseButtonPressed(MouseCode::ButtonRight))
		{
			glm::vec2 mousePosition{ Input::GetMouseX(), Input::GetMouseY() };
			glm::vec2 delta = (mousePosition - m_InitialMousePosition);
			m_InitialMousePosition = mousePosition;

			// getting the ratio between the screen pixels and the scene units
			float xAxisUnitsPerScreenPixel = m_Bounds.GetWidth() / m_ViewportWidth;
			float yAxisUnitsPerScreenPixel = m_Bounds.GetHeight() / m_ViewportHeight;
			// calculating the amout of units the camera needs to move
			m_Position.x -= xAxisUnitsPerScreenPixel * delta.x;
			m_Position.y += yAxisUnitsPerScreenPixel * delta.y;

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
		m_Bounds = { -m_AspectRatio * m_Distance, m_AspectRatio * m_Distance, -m_Distance, m_Distance };
		m_Projection = glm::ortho(m_Bounds.Left, m_Bounds.Right, m_Bounds.Bottom, m_Bounds.Top, m_NearClip, m_FarClip);
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