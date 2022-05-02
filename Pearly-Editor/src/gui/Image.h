#pragma once
#include <Pearly/Renderer/Texture.h>

namespace Pearly {

	class Image
	{
	public:
		Image(const std::string& path);
		static Ref<Image> Load(const std::string& path);

		void* GetTextureID() const;
	private:
		Ref<Texture2D> m_Texture;
	};

}
