#pragma once
#include "OrthographicCamera.h"
#include "Camera.h"
#include "Pearly/Core/Timestep.h"

#include "Pearly/Events/MouseEvents.h"
#include "Pearly/Events/WindowEvents.h"

namespace Pearly {

	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(float aspectRatio, bool rotation = false);

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		void OnResize(float width, float height);

		void SetZoomLevel(float zoomLevel) { m_ZoomLevel = zoomLevel; CalculateView(); }
		float GetZoomLevel() { return m_ZoomLevel; }

		inline OrthographicCameraBounds& GetBounds() { return m_Bounds; }
		inline OrthographicCamera& GetCamera() { return m_Camera; };
	public:
		float CameraMovementSpeed = 1.0f;
		float CameraRotationSpeed = 135.0f;
	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);

		void CalculateView();
	private:
		float m_AspectRatio;
		float m_ZoomLevel = 1.0f;
		OrthographicCameraBounds m_Bounds;
		OrthographicCamera m_Camera;

		bool m_RotationEnabled;
		glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
		float m_CameraRotation = 0.0f;
	};
}