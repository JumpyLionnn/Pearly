#include "prpch.h"
#include "OpenGLTexture.h"

#include <stb_image.h>


namespace Pearly {
	OpenGLTexture2D::OpenGLTexture2D(uint32 width, uint32 height)
		: m_Width(width), m_Height(height), m_Path("")
	{
		PR_PROFILE_FUNCTION();
		m_InternalFormat = GL_RGBA8;
		m_DataFormat = GL_RGBA;
		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, GL_RGBA8, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
		: m_Path(path)
	{
		PR_PROFILE_FUNCTION();
		int width;
		int height;
		int channels;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = nullptr;
		{
			PR_PROFILE_SCOPE("stbi_load - OpenGLTexture2D::OpenGLTexture2D(const std::string&)");
			data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		}
		PR_CORE_ASSERT(data, "Faild to load image '{0}'!", path);
		m_Width = width;
		m_Height = height;

		GLenum internalFormat = 0;
		GLenum dataFormat = 0;
		if (channels == 4)
		{
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
		}
		else if (channels == 3)
		{
			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
		}
		m_InternalFormat = internalFormat;
		m_DataFormat = dataFormat;

		PR_CORE_ASSERT(internalFormat & dataFormat, "Format not supported for image: '{0}'", path);

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, internalFormat, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		PR_PROFILE_FUNCTION();
		glDeleteTextures(1, &m_RendererID);
	}

	void OpenGLTexture2D::SetData(void* data, uint32 size)
	{
		PR_PROFILE_FUNCTION();
		uint32 bytesPerPixel = m_DataFormat == GL_RGBA ? 4 : 3;
		PR_CORE_ASSERT(size == m_Width * m_Height * bytesPerPixel, "Data must be the extire texture!");
		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
	}

	void OpenGLTexture2D::Bind(uint32 slot) const
	{
		PR_PROFILE_FUNCTION();
		glBindTextureUnit(slot, m_RendererID);
	}

	void* OpenGLTexture2D::GetRendererID() const
	{
		return (void*)(uint64)m_RendererID;
	}
}