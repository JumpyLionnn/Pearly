#pragma once

#include "Pearly/Renderer/Shader.h"
#include "Pearly/Core.h"

#include <glm/glm.hpp>

namespace Pearly {

	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& filepath);
		OpenGLShader(const std::string& name, const std::string& vertexSource, const std::string& fragmentSource);
		virtual ~OpenGLShader() override;

		virtual void Bind() const override;
		virtual void Unbind() const override;

		inline virtual const std::string& GetName() const override { return m_Name; }

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
		std::string m_Name;
		std::unordered_map<std::string, int> m_UniformLocationCache;
	};
}