#include "prpch.h"
#include "OrthographicCameraController.h"
#include "Pearly/Core/Input.h"
#include "Pearly/Core/KeyCodes.h"

namespace Pearly {

	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
		: m_AspectRatio(aspectRatio), m_Bounds({ -m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel }), m_Camera(m_Bounds.Left, m_Bounds.Right, m_Bounds.Bottom, m_Bounds.Top), m_RotationEnabled(rotation)
	{}

	void OrthographicCameraController::OnUpdate(Timestep ts)
	{
		PR_PROFILE_FUNCTION();
		if (Input::IsKeyPressed(Pearly::KeyCode::A))
			m_CameraPosition.x -= CameraMovementSpeed * m_ZoomLevel * ts;
		if (Input::IsKeyPressed(Pearly::KeyCode::D))
			m_CameraPosition.x += CameraMovementSpeed * m_ZoomLevel * ts;
		if (Input::IsKeyPressed(Pearly::KeyCode::W))
			m_CameraPosition.y += CameraMovementSpeed * m_ZoomLevel * ts;
		if (Input::IsKeyPressed(Pearly::KeyCode::S))
			m_CameraPosition.y -= CameraMovementSpeed * m_ZoomLevel * ts;

		if (m_RotationEnabled)
		{
			if (Pearly::Input::IsKeyPressed(Pearly::KeyCode::Q))
				m_CameraRotation += CameraRotationSpeed * ts;
			if (Pearly::Input::IsKeyPressed(Pearly::KeyCode::E))
				m_CameraRotation -= CameraRotationSpeed * ts;
			m_Camera.SetRotation(m_CameraRotation);
		}

		m_Camera.SetPosition(m_CameraPosition);
	}

	void OrthographicCameraController::OnResize(float width, float height)
	{
		PR_PROFILE_FUNCTION();
		m_AspectRatio = width / height;
		CalculateView();
	}

	void OrthographicCameraController::OnEvent(Event& e)
	{
		PR_PROFILE_FUNCTION();
		EventDispacher dispacher(e);
		dispacher.Dispatch<MouseScrolledEvent>(PR_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
		dispacher.Dispatch<WindowResizeEvent>(PR_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
	}

	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		PR_PROFILE_FUNCTION();
		m_ZoomLevel -= e.GetYOffset() * 0.25f;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.1f);
		CalculateView();
		return false;
	}

	bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& e)
	{
		PR_PROFILE_FUNCTION();
		OnResize((float)e.GetWidth(), (float)e.GetHeight());
		return false;
	}
	void OrthographicCameraController::CalculateView()
	{
		PR_PROFILE_FUNCTION();
		m_Bounds = { -m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel };
		m_Camera.SetProjection(m_Bounds.Left, m_Bounds.Right, m_Bounds.Bottom, m_Bounds.Top);
	}

}