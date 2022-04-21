#pragma once

#include "Pearly/Renderer/Shader.h"
#include "Pearly/Core.h"

#include <glm/glm.hpp>

namespace Pearly {

	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& filepath);
		OpenGLShader(const std::string& vertexSource, const std::string& fragmentSource);
		~OpenGLShader();

		void Bind() const;
		void Unbind() const;

		void UploadUnifromInt(const std::string& name, int value);
		void UploadUnifromFloat(const std::string& name, float value);
		void UploadUnifromVec2f(const std::string& name, const glm::vec2& vector);
		void UploadUnifromVec3f(const std::string& name, const glm::vec3& vector);
		void UploadUnifromVec4f(const std::string& name, const glm::vec4& vector);

		void UploadUnifromMat3(const std::string& name, const glm::mat3& matrix);
		void UploadUnifromMat4(const std::string& name, const glm::mat4& matrix);
	private:
		std::string ReadFile(const std::string& filepath);
		std::unordered_map<uint32, std::string> PreProccess(const std::string& source);
		void Compile(std::unordered_map<uint32, std::string> shaderSources);
		int GetUniformLocation(const std::string& name);
	private:
		uint32 m_RendererID = 0;
		std::unordered_map<std::string, int> m_UniformLocationCache;
	};
}