#pragma once

#include <glm/glm.hpp>

#include "Texture.h"

namespace Pearly {

	class SubTexture2D
	{
	public:
		SubTexture2D(const Ref<Texture2D> texture, const glm::vec2& min, const glm::vec2& max);

		inline const Ref<Texture2D> GetTexture() const { return m_Texture; }
		inline const std::array<glm::vec2, 4>& GetTexCoords() const { return m_TexCoords; }
	private:
		Ref<Texture2D> m_Texture;

		std::array<glm::vec2, 4> m_TexCoords;
	};

}