#pragma once
#include "Renderer/OrthographicCamera.h"
#include "Core/Timestep.h"

#include "Events/MouseEvents.h"
#include "Events/WindowEvents.h"

namespace Pearly {


	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(float aspectRatio, bool rotation = false);

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		inline OrthographicCamera& GetCamera() { return m_Camera; };
	public:
		float CameraMovementSpeed = 1.0f;
		float CameraRotationSpeed = 135.0f;
	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);
	private:
		float m_AspectRatio;
		float m_ZoomLevel = 1.0f;
		OrthographicCamera m_Camera;

		bool m_RotationEnabled;
		glm::vec3 m_CameraPosition = { 1.0f, 1.0f, 1.0f };
		float m_CameraRotation = 0.0f;
	};
}