#include "prpch.h"
#include "OpenGLShader.h"

#include "Pearly/Core.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace Pearly {
	OpenGLShader::OpenGLShader(const std::string& vertexSource, const std::string& fragmentSource)
	{
		// Create an empty vertex shader handle
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

		// Send the vertex shader source code to GL
		// Note that std::string's .c_str is NULL character terminated.
		const GLchar* source = (const GLchar*)vertexSource.c_str();
		glShaderSource(vertexShader, 1, &source, 0);

		// Compile the vertex shader
		glCompileShader(vertexShader);

		GLint isCompiled = 0;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);

			// We don't need the shader anymore.
			glDeleteShader(vertexShader);

			PR_CORE_ERROR("{0}", infoLog.data());
			PR_CORE_ASSERT(false, "Vertex shader compilation failure!");

			// In this simple program, we'll just leave
			return;
		}

		// Create an empty fragment shader handle
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		// Send the fragment shader source code to GL
		// Note that std::string's .c_str is NULL character terminated.
		source = (const GLchar*)fragmentSource.c_str();
		glShaderSource(fragmentShader, 1, &source, 0);

		// Compile the fragment shader
		glCompileShader(fragmentShader);

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);

			// We don't need the shader anymore.
			glDeleteShader(fragmentShader);
			// Either of them. Don't leak shaders.
			glDeleteShader(vertexShader);

			PR_CORE_ERROR("{0}", infoLog.data());
			PR_CORE_ASSERT(false, "Fragment shader compilation failure!");

			// In this simple program, we'll just leave
			return;
		}

		// Vertex and fragment shaders are successfully compiled.
		// Now time to link them together into a program.
		// Get a program object.
		m_RendererID = glCreateProgram();

		// Attach our shaders to our program
		glAttachShader(m_RendererID, vertexShader);
		glAttachShader(m_RendererID, fragmentShader);

		// Link our program
		glLinkProgram(m_RendererID);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(m_RendererID, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(m_RendererID, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(m_RendererID, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(m_RendererID);
			// Don't leak shaders either.
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

			PR_CORE_ERROR("{0}", infoLog.data());
			PR_CORE_ASSERT(false, "Shader link failure!");

			// In this simple program, we'll just leave
			return;
		}

		// Always detach shaders after a successful link.
		glDetachShader(m_RendererID, vertexShader);
		glDetachShader(m_RendererID, fragmentShader);
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(m_RendererID);
	}

	void OpenGLShader::Bind() const
	{
		glUseProgram(m_RendererID);
	}

	void OpenGLShader::Unbind() const
	{
		glUseProgram(0);
	}


	void OpenGLShader::UploadUnifromInt(const std::string& name, int value)
	{
		int location = GetUniformLocation(name);
		glUniform1i(location, value);
	}

	void OpenGLShader::UploadUnifromFloat(const std::string& name, float value)
	{
		int location = GetUniformLocation(name);
		glUniform1f(location, value);
	}

	void OpenGLShader::UploadUnifromVec2f(const std::string& name, const glm::vec2& vector)
	{
		int location = GetUniformLocation(name);
		glUniform2f(location, vector.x, vector.y);
	}

	void OpenGLShader::UploadUnifromVec3f(const std::string& name, const glm::vec3& vector)
	{
		int location = GetUniformLocation(name);
		glUniform3f(location, vector.x, vector.y, vector.z);
	}


	void OpenGLShader::UploadUnifromVec4f(const std::string& name, const glm::vec4& vector)
	{
		int location = GetUniformLocation(name);
		glUniform4f(location, vector.x, vector.y, vector.z, vector.w);
	}

	void OpenGLShader::UploadUnifromMat3(const std::string& name, const glm::mat3& matrix)
	{
		int location = GetUniformLocation(name);
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::UploadUnifromMat4(const std::string& name, const glm::mat4& matrix)
	{
		int location = GetUniformLocation(name);
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	int OpenGLShader::GetUniformLocation(const std::string& name)
	{
		if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
			return m_UniformLocationCache.at(name);

		int location = glGetUniformLocation(m_RendererID, name.c_str());
		m_UniformLocationCache[name] = location;
		return location;
	}
}