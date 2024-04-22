#include "gxpch.h"
#include "Genix/Platform/OpenGL/OpenGLShader.h"
#include "Genix/Renderer/Renderer.h"

#include <glm/gtc/type_ptr.hpp>
#include <shaderc/shaderc.hpp>
#include <spirv_cross/spirv_cross.hpp>
#include <spirv_cross/spirv_glsl.hpp>

namespace Utils
{
	static GLenum ShaderTypeFromString(const std::string& type)
	{
		if (type == "vertex")
			return GL_VERTEX_SHADER;
		if (type == "fragment" || type == "pixel")
			return GL_FRAGMENT_SHADER;

		ASSERT_CORE(false, "Unknown shader type!");
		return 0;
	}

	static shaderc_shader_kind GLShaderStageToShaderC(GLenum stage)
	{
		switch (stage)
		{
		case GL_VERTEX_SHADER:   return shaderc_glsl_vertex_shader;
		case GL_FRAGMENT_SHADER: return shaderc_glsl_fragment_shader;
		}
		ASSERT_CORE(false);
		return (shaderc_shader_kind)0;
	}

	static const char* GLShaderStageToString(GLenum stage)
	{
		switch (stage)
		{
		case GL_VERTEX_SHADER:   return "GL_VERTEX_SHADER";
		case GL_FRAGMENT_SHADER: return "GL_FRAGMENT_SHADER";
		}
		ASSERT_CORE(false);
		return nullptr;
	}

	static const char* GetCacheDirectory()
	{
		return "Assets/Cache/Shader/Opengl";
	}

	static void CreateCacheDirectoryIfNeeded()
	{
		std::string cacheDirectory = GetCacheDirectory();
		if (!std::filesystem::exists(cacheDirectory))
		{
			std::filesystem::create_directories(cacheDirectory);
		}
	}

	static const char* GLShaderStageCachedOpenGLFileExtension(uint32_t stage)
	{
		switch (stage)
		{
		case GL_VERTEX_SHADER:    return ".cached_opengl.vert";
		case GL_FRAGMENT_SHADER:  return ".cached_opengl.frag";
		}
		ASSERT_CORE(false);
		return "";
	}

	static const char* GLShaderStageCachedVulkanFileExtension(uint32_t stage)
	{
		switch (stage)
		{
		case GL_VERTEX_SHADER:    return ".cached_vulkan.vert";
		case GL_FRAGMENT_SHADER:  return ".cached_vulkan.frag";
		}
		ASSERT_CORE(false);
		return "";
	}

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
}


OpenGLShader::OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc)
{
	GX_PROFILE_FUNCTION();

	// Extract name from filepath
	auto lastSlash = vertexSrc.find_last_of("/\\");
	lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
	const auto lastDot = vertexSrc.rfind('.');
	const auto count = lastDot == std::string::npos ? vertexSrc.size() - lastSlash : lastDot - lastSlash;
	m_Name = vertexSrc.substr(lastSlash, count);
	
	std::unordered_map<GLenum, std::string> sources;
	sources[GL_VERTEX_SHADER] = Utils::ReadFileAsString(vertexSrc);
	sources[GL_FRAGMENT_SHADER] = Utils::ReadFileAsString(fragmentSrc);
	Compile(sources);
}

OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
	: m_Name(name)
{
	GX_PROFILE_FUNCTION();

	Utils::CreateCacheDirectoryIfNeeded();
	
	m_FilePaths[GL_VERTEX_SHADER] = vertexSrc;
	m_FilePaths[GL_FRAGMENT_SHADER] = fragmentSrc;

	std::unordered_map<GLenum, std::string> sources;
	sources[GL_VERTEX_SHADER] = Utils::ReadFileAsString(vertexSrc);
	sources[GL_FRAGMENT_SHADER] = Utils::ReadFileAsString(fragmentSrc);
	Compile(sources);
}

OpenGLShader::~OpenGLShader()
{
	GX_PROFILE_FUNCTION();

	glDeleteProgram(m_ID);
}

void OpenGLShader::Bind() const
{
	GX_PROFILE_FUNCTION();

	glUseProgram(m_ID);
}

void OpenGLShader::Unbind() const
{
	GX_PROFILE_FUNCTION();

	glUseProgram(0);
}

void OpenGLShader::GLSetUniform_Bool(const std::string& name, const bool value) const
{
	GX_PROFILE_FUNCTION();

	CheckOpenGLRendererAPI();
	glUniform1i(glGetUniformLocation(m_ID, name.c_str()), (int)value);
}

void OpenGLShader::GLSetUniform_Int(const std::string& name, const int value) const
{
	GX_PROFILE_FUNCTION();

	CheckOpenGLRendererAPI();
	glUniform1i(glGetUniformLocation(m_ID, name.c_str()), value);
}

void OpenGLShader::GLSetUniform_Float(const std::string& name, float value) const
{
	GX_PROFILE_FUNCTION();

	CheckOpenGLRendererAPI();
	glUniform1f(glGetUniformLocation(m_ID, name.c_str()), value);
}

void OpenGLShader::GLSetUniform_Vec2(const std::string& name, const glm::vec2& value) const
{
	GX_PROFILE_FUNCTION();

	CheckOpenGLRendererAPI();
	glUniform2fv(glGetUniformLocation(m_ID, name.c_str()), 1, glm::value_ptr(value));
}

void OpenGLShader::GLSetUniform_Vec2(const std::string& name, const float x, const float y) const
{
	GX_PROFILE_FUNCTION();

	CheckOpenGLRendererAPI();
	glUniform2f(glGetUniformLocation(m_ID, name.c_str()), x, y);
}

void OpenGLShader::GLSetUniform_Vec3(const std::string& name, const glm::vec3& value) const
{
	GX_PROFILE_FUNCTION();

	CheckOpenGLRendererAPI();
	glUniform3fv(glGetUniformLocation(m_ID, name.c_str()), 1, glm::value_ptr(value));
}

void OpenGLShader::GLSetUniform_Vec3(const std::string& name, const float x, const float y, const float z) const
{
	GX_PROFILE_FUNCTION();

	CheckOpenGLRendererAPI();
	glUniform3f(glGetUniformLocation(m_ID, name.c_str()), x, y, z);
}

void OpenGLShader::GLSetUniform_Vec4(const std::string& name, const glm::vec4& value) const
{
	GX_PROFILE_FUNCTION();

	CheckOpenGLRendererAPI();
	glUniform4fv(glGetUniformLocation(m_ID, name.c_str()), 1, glm::value_ptr(value));
}

void OpenGLShader::GLSetUniform_Vec4(const std::string& name, const float x, const float y, const float z, const float w) const
{
	GX_PROFILE_FUNCTION();

	CheckOpenGLRendererAPI();
	glUniform4f(glGetUniformLocation(m_ID, name.c_str()), x, y, z, w);
}

void OpenGLShader::GLSetUniform_Mat2(const std::string& name, const glm::mat2& matrix) const
{
	GX_PROFILE_FUNCTION();

	CheckOpenGLRendererAPI();
	glUniformMatrix2fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
}

void OpenGLShader::GLSetUniform_Mat3(const std::string& name, const glm::mat3& matrix) const
{
	GX_PROFILE_FUNCTION();

	CheckOpenGLRendererAPI();
	glUniformMatrix3fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
}

void OpenGLShader::GLSetUniform_Mat4(const std::string& name, const glm::mat4& matrix) const
{
	GX_PROFILE_FUNCTION();

	CheckOpenGLRendererAPI();
	glUniformMatrix4fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
}

void OpenGLShader::Compile(std::unordered_map<GLenum, std::string> shaderSources)
{
	GX_PROFILE_FUNCTION();

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

void OpenGLShader::CompileOrGetVulkanBinaries(const std::unordered_map<GLenum, std::string>& shaderSources)
{
	GX_PROFILE_FUNCTION();

	GLuint program = glCreateProgram();

	shaderc::Compiler compiler;
	shaderc::CompileOptions options;
	options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_2);
	const bool optimize = true;
	if (optimize)
		options.SetOptimizationLevel(shaderc_optimization_level_performance);

	std::filesystem::path cacheDirectory = Utils::GetCacheDirectory();

	auto& shaderData = m_VulkanSPIRV;
	shaderData.clear();
	for (auto&& [stage, source] : shaderSources)
	{
		std::filesystem::path shaderFilePath = m_FilePaths[stage];
		std::filesystem::path cachedPath = cacheDirectory / (shaderFilePath.filename().string() + Utils::GLShaderStageCachedVulkanFileExtension(stage));

		std::ifstream in(cachedPath, std::ios::in | std::ios::binary);
		if (in.is_open())
		{
			in.seekg(0, std::ios::end);
			auto size = in.tellg();
			in.seekg(0, std::ios::beg);

			auto& data = shaderData[stage];
			data.resize(size / sizeof(uint32_t));
			in.read((char*)data.data(), size);
		}
		else
		{
			shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(source, Utils::GLShaderStageToShaderC(stage), m_FilePaths[stage].c_str(), options);
			if (module.GetCompilationStatus() != shaderc_compilation_status_success)
			{
				LOG_CORE_ERROR(module.GetErrorMessage());
				ASSERT_CORE(false);
			}

			shaderData[stage] = std::vector<uint32_t>(module.cbegin(), module.cend());

			std::ofstream out(cachedPath, std::ios::out | std::ios::binary);
			if (out.is_open())
			{
				auto& data = shaderData[stage];
				out.write((char*)data.data(), data.size() * sizeof(uint32_t));
				out.flush();
				out.close();
			}
		}
	}

	for (auto&& [stage, data]: shaderData)
		Reflect(stage, data);
}

void OpenGLShader::CompileOrGetOpenGLBinaries()
{
	GX_PROFILE_FUNCTION();

	auto& shaderData = m_OpenGLSPIRV;

	shaderc::Compiler compiler;
	shaderc::CompileOptions options;
	options.SetTargetEnvironment(shaderc_target_env_opengl, shaderc_env_version_opengl_4_5);
	const bool optimize = false;
	if (optimize)
		options.SetOptimizationLevel(shaderc_optimization_level_performance);

	std::filesystem::path cacheDirectory = Utils::GetCacheDirectory();

	shaderData.clear();
	m_OpenGLSourceCode.clear();
	for (auto&& [stage, spirv] : m_VulkanSPIRV)
	{
		std::filesystem::path shaderFilePath = m_FilePaths[stage];
		std::filesystem::path cachedPath = cacheDirectory / (shaderFilePath.filename().string() + Utils::GLShaderStageCachedOpenGLFileExtension(stage));

		std::ifstream in(cachedPath, std::ios::in | std::ios::binary);
		if (in.is_open())
		{
			in.seekg(0, std::ios::end);
			auto size = in.tellg();
			in.seekg(0, std::ios::beg);

			auto& data = shaderData[stage];
			data.resize(size / sizeof(uint32_t));
			in.read((char*)data.data(), size);
		}
		else
		{
			spirv_cross::CompilerGLSL glslCompiler(spirv);
			m_OpenGLSourceCode[stage] = glslCompiler.compile();
			auto& source = m_OpenGLSourceCode[stage];

			shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(source, Utils::GLShaderStageToShaderC(stage), m_FilePaths[stage].c_str());
			if (module.GetCompilationStatus() != shaderc_compilation_status_success)
			{
				LOG_CORE_ERROR(module.GetErrorMessage());
				ASSERT_CORE(false);
			}

			shaderData[stage] = std::vector<uint32_t>(module.cbegin(), module.cend());

			std::ofstream out(cachedPath, std::ios::out | std::ios::binary);
			if (out.is_open())
			{
				auto& data = shaderData[stage];
				out.write((char*)data.data(), data.size() * sizeof(uint32_t));
				out.flush();
				out.close();
			}
		}
	}
}

void OpenGLShader::CreateProgram()
{
	GX_PROFILE_FUNCTION();

	GLuint program = glCreateProgram();

	std::vector<GLuint> shaderIDs;
	for (auto&& [stage, spirv] : m_OpenGLSPIRV)
	{
		GLuint shaderID = shaderIDs.emplace_back(glCreateShader(stage));
		glShaderBinary(1, &shaderID, GL_SHADER_BINARY_FORMAT_SPIR_V, spirv.data(), spirv.size() * sizeof(uint32_t));
		glSpecializeShader(shaderID, "main", 0, nullptr, nullptr);
		glAttachShader(program, shaderID);
	}

	glLinkProgram(program);

	GLint isLinked;
	glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
	if (isLinked == GL_FALSE)
	{
		GLint maxLength;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

		std::vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(program, maxLength, &maxLength, infoLog.data());
		LOG_CORE_ERROR("Shader linking failed ({0}):\n{1}", m_FilePaths[0], infoLog.data());

		glDeleteProgram(program);

		for (auto id : shaderIDs)
			glDeleteShader(id);
	}

	for (auto id : shaderIDs)
	{
		glDetachShader(program, id);
		glDeleteShader(id);
	}

	m_ID = program;
}

void OpenGLShader::Reflect(GLenum stage, const std::vector<uint32>& shaderData)
{
	GX_PROFILE_FUNCTION();

	spirv_cross::Compiler compiler(shaderData);
	spirv_cross::ShaderResources resources = compiler.get_shader_resources();

	LOG_CORE_TRACE("OpenGLShader::Reflect - {0} {1}", Utils::GLShaderStageToString(stage), m_FilePaths[0]);
	LOG_CORE_TRACE("    {0} uniform buffers", resources.uniform_buffers.size());
	LOG_CORE_TRACE("    {0} resources", resources.sampled_images.size());

	LOG_CORE_TRACE("Uniform buffers:");
	for (const auto& resource : resources.uniform_buffers)
	{
		const auto& bufferType = compiler.get_type(resource.base_type_id);
		uint32 bufferSize = compiler.get_declared_struct_size(bufferType);
		uint32 binding = compiler.get_decoration(resource.id, spv::DecorationBinding);
		int memberCount = bufferType.member_types.size();

		LOG_CORE_TRACE("  {0}", resource.name);
		LOG_CORE_TRACE("    Size = {0}", bufferSize);
		LOG_CORE_TRACE("    Binding = {0}", binding);
		LOG_CORE_TRACE("    Members = {0}", memberCount);
	}
}

