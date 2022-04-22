#pragma once

#include <glm/glm.hpp>

namespace Pearly {

	class OrthographicCamera
	{
	public:
		OrthographicCamera(float left, float right, float bottom, float top, float zNear = -1.0f, float zFar = 1.0f);

		void SetProjection(float left, float right, float bottom, float top, float zNear = -1.0f, float zFar = 1.0f);

		inline const glm::vec3& GetPosition(const glm::vec3& position) const { return m_Position; }
		inline void SetPosition(const glm::vec3& position) 
		{ 
			m_Position = position; 
			RecalculateViewMatrix();
		}

		inline float GetRotation() const { return m_Rotation; }
		inline void SetRotation(float rotation) 
		{
			m_Rotation = rotation;
			RecalculateViewMatrix();
		}

		inline const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		inline const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		inline const glm::mat4& GetViewProjectionMatrix() const { return m_ViewPorjectionMatrix; }
	private:
		void RecalculateViewMatrix();
	private: 
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewPorjectionMatrix;

		glm::vec3 m_Position = {0.0f, 0.0f, 0.0f};
		float m_Rotation = 0.0f;
	};

}