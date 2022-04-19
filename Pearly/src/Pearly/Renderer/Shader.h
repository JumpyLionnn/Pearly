#pragma once

namespace Pearly {

	class Shader
	{
	public:
		Shader(const std::string& vertexSource, const std::string& fragmentSource);
		~Shader();

		void Bind() const;
		void Unbind() const;
	private:
		unsigned int m_RendererID = 0;
	};
}