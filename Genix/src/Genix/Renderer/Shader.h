#pragma once

#include "Genix/Common/PrimitiveTypes.h"

typedef unsigned int GLenum;

class Shader
{
public:
	Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
	Shader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
	virtual ~Shader();

	void Bind() const;
	void Unbind() const;
	const std::string& GetName() const { return m_Name; }
	
	// utility OpenGL uniform functions
	// ------------------------------------------------------------------------
	void GLSetUniform_Bool(const std::string& name, bool value) const;
	void GLSetUniform_Int(const std::string& name, int value) const;
	void GLSetUniform_Float(const std::string& name, float value) const;
	void GLSetUniform_Vec2(const std::string& name, const glm::vec2& value) const;
	void GLSetUniform_Vec2(const std::string& name, const float x, const float y) const;
	void GLSetUniform_Vec3(const std::string& name, const glm::vec3& value) const;
	void GLSetUniform_Vec3(const std::string& name, const float x, const float y, const float z) const;
	void GLSetUniform_Vec4(const std::string& name, const glm::vec4& value) const;
	void GLSetUniform_Vec4(const std::string& name, const float x, const float y, const float z, const float w) const;
	void GLSetUniform_Mat2(const std::string& name, const glm::mat2& matrix) const;
	void GLSetUniform_Mat3(const std::string& name, const glm::mat3& matrix) const;
	void GLSetUniform_Mat4(const std::string& name, const glm::mat4& matrix) const;

	static Ref<Shader> Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
	static Ref<Shader> Create(const std::string& vertexSrc, const std::string& fragmentSrc);
private:
	void Compile(std::unordered_map<GLenum, std::string> shaderSources);
	bool CheckCompileErrors(unsigned int shader, const std::string& type);
	static bool CheckOpenGLRendererAPI();

	uint32 m_ID;
	std::string m_Name;
	std::unordered_map<GLenum, std::string> m_FilePaths;
};

class ShaderLibrary
{
public:
	void Add(const std::string& name, const Ref<Shader>& shader);
	void Add(const Ref<Shader>& shader);
	Ref<Shader> Load(const std::string& vertexSrc, const std::string& fragmentSrc);
	Ref<Shader> Load(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);

	Ref<Shader> Get(const std::string& name);

	bool Exists(const std::string& name) const;
private:
	std::unordered_map<std::string, Ref<Shader>> m_Shaders;
};