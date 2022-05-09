#pragma once
#include <glm/glm.hpp>

namespace Pearly {

	struct OrthographicCameraBounds
	{
		float Left;
		float Right;
		float Bottom;
		float Top;

		OrthographicCameraBounds(float left, float right, float bottom, float top)
			: Left(left), Right(right), Bottom(bottom), Top(top)
		{}

		inline float GetWidth() { return Right - Left; }
		inline float GetHeight() { return Top - Bottom; }
	};

	class Camera
	{
	public:
		Camera() = default;
		Camera(const glm::mat4& projection)
			: m_Projection(projection)
		{}
		virtual ~Camera() = default;

		inline const glm::mat4& GetProjection() const { return m_Projection; }
	protected:
		glm::mat4 m_Projection = glm::mat4(1.0f);
	};

}