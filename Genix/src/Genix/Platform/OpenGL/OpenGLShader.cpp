#include "gxpch.h"
#include "OpenGLShader.h"

#include "Genix/Renderer/Renderer.h"
#include "glm/gtc/type_ptr.hpp"

std::string ReadFileAsString(const std::string& filepath)
{
	std::string result;
	std::ifstream in(filepath, std::ios::in | std::ios::binary);
	if (in)
	{
		in.seekg(0, std::ios::end);
		result.resize((size_t)in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&result[0], result.size());
		in.close();
	}
	else
	{
		LOG_CORE_ERROR("Could not open file '{0}'", filepath);
	}

	return result;
}

OpenGLShader::OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc)
{
	std::unordered_map<GLenum, std::string> sources;
	sources[GL_VERTEX_SHADER] = ReadFileAsString(vertexSrc);
	sources[GL_FRAGMENT_SHADER] = ReadFileAsString(fragmentSrc);
	Compile(sources);
}

OpenGLShader::~OpenGLShader()
{
	glDeleteProgram(m_ID);
}

void OpenGLShader::Bind() const
{
	glUseProgram(m_ID);
}

void OpenGLShader::Unbind() const
{
	glUseProgram(0);
}

void OpenGLShader::GLSetUniform_Bool(const std::string& name, const bool value) const
{
	CheckOpenGLRendererAPI();
	glUniform1i(glGetUniformLocation(m_ID, name.c_str()), (int)value);
}

void OpenGLShader::GLSetUniform_Int(const std::string& name, const int value) const
{
	CheckOpenGLRendererAPI();
	glUniform1i(glGetUniformLocation(m_ID, name.c_str()), value);
}

void OpenGLShader::GLSetUniform_Float(const std::string& name, float value) const
{
	CheckOpenGLRendererAPI();
	glUniform1f(glGetUniformLocation(m_ID, name.c_str()), value);
}

void OpenGLShader::GLSetUniform_Vec2(const std::string& name, glm::vec2& value) const
{
	CheckOpenGLRendererAPI();
	glUniform2fv(glGetUniformLocation(m_ID, name.c_str()), 1, glm::value_ptr(value));
}

void OpenGLShader::GLSetUniform_Vec2(const std::string& name, const float x, const float y) const
{
	CheckOpenGLRendererAPI();
	glUniform2f(glGetUniformLocation(m_ID, name.c_str()), x, y);
}

void OpenGLShader::GLSetUniform_Vec3(const std::string& name, glm::vec3& value) const
{
	CheckOpenGLRendererAPI();
	glUniform3fv(glGetUniformLocation(m_ID, name.c_str()), 1, glm::value_ptr(value));
}

void OpenGLShader::GLSetUniform_Vec3(const std::string& name, const float x, const float y, const float z) const
{
	CheckOpenGLRendererAPI();
	glUniform3f(glGetUniformLocation(m_ID, name.c_str()), x, y, z);
}

void OpenGLShader::GLSetUniform_Vec4(const std::string& name, glm::vec4& value) const
{
	CheckOpenGLRendererAPI();
	glUniform4fv(glGetUniformLocation(m_ID, name.c_str()), 1, glm::value_ptr(value));
}

void OpenGLShader::GLSetUniform_Vec4(const std::string& name, const float x, const float y, const float z, const float w) const
{
	CheckOpenGLRendererAPI();
	glUniform4f(glGetUniformLocation(m_ID, name.c_str()), x, y, z, w);
}

void OpenGLShader::GLSetUniform_Mat2(const std::string& name, glm::mat2& matrix) const
{
	CheckOpenGLRendererAPI();
	glUniformMatrix2fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
}

void OpenGLShader::GLSetUniform_Mat3(const std::string& name, glm::mat3& matrix) const
{
	CheckOpenGLRendererAPI();
	glUniformMatrix3fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
}

void OpenGLShader::GLSetUniform_Mat4(const std::string& name, glm::mat4& matrix) const
{
	CheckOpenGLRendererAPI();
	glUniformMatrix4fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
}

void OpenGLShader::Compile(std::unordered_map<GLenum, std::string> shaderSources)
{
	GLuint program = glCreateProgram();
	std::vector<GLenum> glShaderIDs;
	for (auto& kv : shaderSources)
	{
		GLenum type = kv.first;
		const std::string& source = kv.second;

		GLuint shader = glCreateShader(type);

		const GLchar* sourceCStr = source.c_str();
		glShaderSource(shader, 1, &sourceCStr, 0);

		glCompileShader(shader);

		if (CheckCompileErrors(shader, std::to_string(type)))
		{
			return;
		}

		glAttachShader(program, shader);
		glShaderIDs.push_back(shader);
	}
		
	m_ID = program;

	// Link our program
	glLinkProgram(program);
	CheckCompileErrors(program, "PROGRAM");

	for (auto id : glShaderIDs)
	{
		glDetachShader(program, id);
		glDeleteShader(id);
	}
}

bool OpenGLShader::CheckCompileErrors(unsigned shader, const std::string& type)
{	
	int success;
	char infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
			LOG_CORE_ERROR("{0}", infoLog);
			ASSERT_CORE(false, "Shader compilation failure!")
			return true;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
			LOG_CORE_ERROR("{0}", infoLog);
			ASSERT_CORE(false, "Shader linking failure!")
			return true;
		}
	}
	return false;
}

bool OpenGLShader::CheckOpenGLRendererAPI()
{
	if (Renderer::GetAPI() == RendererAPI::API::OpenGL)
	{
		return true;
	}
	ASSERT_CORE(false, "Renderer API is not OpenGL!")
	return false;
}

