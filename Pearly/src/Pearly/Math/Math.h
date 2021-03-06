#pragma once


#include <glm/glm.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>

namespace Pearly {
	class Math
	{
	public:
		static bool DecomposeTransform(const glm::mat4& transform, glm::vec3& translation, float& rotation, glm::vec2& scale);
	
		static float Signum(float value) 
		{
			return (float)((0.0f < value) - (value < 0.0f));
		}
	};
}