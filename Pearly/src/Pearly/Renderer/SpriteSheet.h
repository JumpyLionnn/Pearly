#pragma once

#include <glm/glm.hpp>

#include "Texture.h"
#include "SubTexture.h"

namespace Pearly {

	class SpriteSheet
	{
	public:
		SpriteSheet(Ref<Texture2D> texture, const glm::vec2& cellSize);
		SpriteSheet(const std::string& texturePath, const glm::vec2& cellSize);

		Ref<SubTexture2D> CreateSubTexture(const glm::vec2& coords, const glm::vec2& spriteSize = { 1, 1 }) const;
	private:
		Ref<Texture2D> m_Texture;
		glm::vec2 m_CellSize;
	};

}