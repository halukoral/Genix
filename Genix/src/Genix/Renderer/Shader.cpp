#include "gxpch.h"
#include "Shader.h"

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

Shader::Shader(const std::string& vertexSrc, const std::string& fragmentSrc)
{
	std::unordered_map<GLenum, std::string> sources;
	sources[GL_VERTEX_SHADER] = ReadFileAsString(vertexSrc);
	sources[GL_FRAGMENT_SHADER] = ReadFileAsString(fragmentSrc);
	Compile(sources);
}

Shader::~Shader()
{
	glDeleteProgram(m_RendererID);
}

void Shader::Bind() const
{
	glUseProgram(m_RendererID);
}

void Shader::Unbind() const
{
	glUseProgram(0);
}

void Shader::Compile(std::unordered_map<GLenum, std::string> shaderSources)
{
	GLuint program = glCreateProgram();
	std::vector<GLenum> glShaderIDs(shaderSources.size());
	for (auto& kv : shaderSources)
	{
		GLenum type = kv.first;
		const std::string& source = kv.second;

		GLuint shader = glCreateShader(type);

		const GLchar* sourceCStr = source.c_str();
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

			LOG_CORE_ERROR("{0}", infoLog.data());
			GX_CORE_ASSERT(false, "Shader compilation failure!")
			break;
		}

		glAttachShader(program, shader);
		glShaderIDs.push_back(shader);
	}
		
	m_RendererID = program;

	// Link our program
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
			
		for (auto id : glShaderIDs)
		{
			glDeleteShader(id);
		}

		LOG_CORE_ERROR("{0}", infoLog.data());
		GX_CORE_ASSERT(false, "Shader link failure!")
		return;
	}

	for (auto id : glShaderIDs)
	{
		glDetachShader(program, id);
	}
}
