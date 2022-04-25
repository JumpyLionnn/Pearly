#include "prpch.h"
#include "SpriteSheet.h"

namespace Pearly {

	SpriteSheet::SpriteSheet(Ref<Texture2D> texture, const glm::vec2& cellSize)
		: m_Texture(texture), m_CellSize(cellSize)
	{}

	SpriteSheet::SpriteSheet(const std::string& texturePath, const glm::vec2& cellSize)
		: m_Texture(Texture2D::Create(texturePath)), m_CellSize(cellSize)
	{}

	Ref<SubTexture2D> SpriteSheet::CreateSubTexture(const glm::vec2& coords, const glm::vec2& spriteSize) const
	{
		glm::vec2 min = { coords.x * m_CellSize.x / m_Texture->GetWidth(), coords.y * m_CellSize.y / m_Texture->GetHeight() };
		glm::vec2 max = {(coords.x + spriteSize.x) * m_CellSize.x / m_Texture->GetWidth(), (coords.y + spriteSize.y) * m_CellSize.y / m_Texture->GetHeight() };
		return CreateRef<SubTexture2D>(m_Texture, min, max);
	}

}