#include "Image.h"

namespace Pearly {
	Image::Image(const std::string& path)
	{
		m_Texture = Texture2D::Create(path);
	}
	Ref<Image> Image::Load(const std::string& path)
	{
		return CreateRef<Image>(path);
	}


	void* Image::GetTextureID() const
	{
		return m_Texture->GetRendererID();
	}
}