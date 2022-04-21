#include "prpch.h"
#include "OpenGLShader.h"

#include "Pearly/Core.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace Pearly {

	static GLenum ShaderTypeFromString(const std::string& type)
	{
		if (type == "vertex" || type == "vert") return GL_VERTEX_SHADER;
		if (type == "fragment" || type == "frag" || type == "pixel") return GL_FRAGMENT_SHADER;

		PR_CORE_ASSERT(false, "Unknown shader type '{0}'!", type);
		return 0;
	}

	static std::string ShaderTypeToString(GLenum& type)
	{
		switch (type)
		{
			case GL_VERTEX_SHADER: return "Vertex";
			case GL_FRAGMENT_SHADER: return "Fragment";
		}

		PR_CORE_ASSERT(false, "Unknown shader type '{0}'!", type);
		return "Unknown";
	}

	OpenGLShader::OpenGLShader(const std::string& filepath)
	{
		std::string source = ReadFile(filepath);
		std::unordered_map<GLenum, std::string> shaderSources = PreProccess(source);
		Compile(shaderSources);

		int lastSlash = filepath.find_last_of("\\/");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		int lastDot = filepath.rfind(".");
		int count = lastDot == std::string::npos ? filepath.size() - lastSlash : lastDot - lastSlash;
		m_Name = filepath.substr(lastSlash, count);
	}

	OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSource, const std::string& fragmentSource)
		: m_Name(name)
	{
		std::unordered_map<GLenum, std::string> sources;
		sources[GL_VERTEX_SHADER] = vertexSource;
		sources[GL_FRAGMENT_SHADER] = fragmentSource;
		Compile(sources);
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

	std::string OpenGLShader::ReadFile(const std::string& filepath)
	{
		std::string result;
		std::ifstream in(filepath, std::ios::in | std::ios::binary);
		if (in)
		{
			in.seekg(0, std::ios::end);
			result.resize(in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(&result[0], result.size());
			in.close();
		}
		else
		{
			PR_CORE_ERROR("Could not open file '{0}'", filepath);
		}
		return result;
	}

	std::unordered_map<GLenum, std::string> OpenGLShader::PreProccess(const std::string& source)
	{
		std::unordered_map<GLenum, std::string> shaderSources;

		const char* typeToken = "#type";
		int typeTokenLength = strlen(typeToken);
		int	pos = source.find(typeToken, 0);
		while (pos != std::string::npos)
		{
			int endOfLine = source.find_first_of("\r\n", pos);
			PR_CORE_ASSERT(endOfLine != std::string::npos, "Syntax error!");
			int begin = pos + typeTokenLength + 1;
			std::string type = source.substr(begin, (int)(endOfLine - begin));
			PR_CORE_ASSERT(ShaderTypeFromString(type), "Invalid shader type specified '{0}'!", type);

			int nextLinePos = source.find_first_not_of("\r\n", endOfLine);
			pos = source.find(typeToken, nextLinePos);
			shaderSources[ShaderTypeFromString(type)] = source.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos));
		}
		return shaderSources;
	}

	void OpenGLShader::Compile(std::unordered_map<GLenum, std::string> shaderSources)
	{
		GLuint program = glCreateProgram();
		PR_CORE_ASSERT(shaderSources.size() <= 2, "Only 2 shaders are supported now!");
		std::array<GLenum, 2> glShaderIDs;
		int shaderIdIndex = 0;
		for (std::pair<const GLenum, std::string>& kv : shaderSources)
		{
			GLenum type = kv.first;
			const std::string& source = kv.second;

			GLuint shader = glCreateShader(type);

			const GLchar* sourceCStr = (const GLchar*)source.c_str();
			glShaderSource(shader, 1, &sourceCStr, 0);

			glCompileShader(shader);

			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

				glDeleteShader(shader);

				PR_CORE_ERROR("{0}", infoLog.data());
				PR_CORE_ASSERT(false, "{0} shader compilation failure!", ShaderTypeToString(type));

				break;
			}
			glShaderIDs[shaderIdIndex++] = shader;
			glAttachShader(program, shader);
		}

		

		glLinkProgram(program);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(program);
			
			for (GLenum id : glShaderIDs)
			{
				glDeleteShader(id);
			}

			PR_CORE_ERROR("{0}", infoLog.data());
			PR_CORE_ASSERT(false, "Shader link failure!");
			return;
		}

		for (GLenum id : glShaderIDs)
		{
			glDetachShader(program, id);
		}

		m_RendererID = program;
	}
}