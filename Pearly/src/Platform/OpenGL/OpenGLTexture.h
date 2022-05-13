#pragma once
#include "Pearly/Renderer/Texture.h"

#include <glad/glad.h>


namespace Pearly {

	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(uint32 width, uint32 height);
		OpenGLTexture2D(const std::string& path);
		virtual ~OpenGLTexture2D() override;

		virtual uint32 GetWidth() const override { return m_Width; }
		virtual uint32 GetHeight() const override { return m_Height; }
		virtual const std::string& GetPath() const override { return m_Path; }

		virtual void SetData(void* data, uint32 size) override;

		virtual void Bind(uint32 slot = 0) const override;

		virtual void* GetRendererID() const override;

		virtual bool operator==(const Texture& other) const override { return m_RendererID == ((OpenGLTexture2D&)other).m_RendererID; }
	private:
		std::string m_Path;
		uint32 m_RendererID;
		uint32 m_Width;
		uint32 m_Height;
		GLenum m_InternalFormat;
		GLenum m_DataFormat;
	};

}