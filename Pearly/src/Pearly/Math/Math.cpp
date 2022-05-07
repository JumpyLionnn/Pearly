#include "prpch.h"
#include "Math.h"

namespace Pearly {
	bool Math::DecomposeTransform(const glm::mat4& transform, glm::vec3& position, float& rotation, glm::vec2& scale)
	{
		glm::mat4 localMatrix(transform);

		// Normalize the matrix.
		if (glm::epsilonEqual(localMatrix[3][3], static_cast<float>(0), glm::epsilon<float>()))
			return false;

		// First, isolate perspective.  floathis is the messiest.
		if (
			glm::epsilonNotEqual(localMatrix[0][3], static_cast<float>(0), glm::epsilon<float>()) ||
			glm::epsilonNotEqual(localMatrix[1][3], static_cast<float>(0), glm::epsilon<float>()) ||
			glm::epsilonNotEqual(localMatrix[2][3], static_cast<float>(0), glm::epsilon<float>()))
		{
			// Clear the perspective partition
			localMatrix[0][3] = localMatrix[1][3] = localMatrix[2][3] = static_cast<float>(0);
			localMatrix[3][3] = static_cast<float>(1);
		}

		// Next take care of translation (easy).
		position = glm::vec3(localMatrix[3]);
		localMatrix[3] = glm::vec4(0, 0, 0, localMatrix[3].w);

		glm::vec3 row[3];

		// Now get scale and shear.
		for (glm::length_t i = 0; i < 3; ++i)
			for (glm::length_t j = 0; j < 3; ++j)
				row[i][j] = localMatrix[i][j];

		// Compute X scale factor and normalize first row.
		scale.x = glm::length(row[0]);
		row[0] = glm::detail::scale(row[0], static_cast<float>(1));
		scale.y = glm::length(row[1]);
		row[1] = glm::detail::scale(row[1], static_cast<float>(1));

		rotation = atan2(row[0][1], row[0][0]);

		return true;
	}

}