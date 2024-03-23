#pragma once
#include "Genix/Renderer/Shader.h"
#include "Genix/Common/PrimitiveTypes.h"

#include <glad/glad.h>
#include <glm/glm.hpp>

class OpenGLShader : public Shader
{
public:
	OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc);
	~OpenGLShader() override;

	void Bind() const override;
	void Unbind() const override;

	uint32 GetId() const { return m_ID; }
	
	// utility OpenGL uniform functions
	// ------------------------------------------------------------------------
	void GLSetUniform_Bool(const std::string& name, bool value) const override;
	void GLSetUniform_Int(const std::string& name, int value) const override;
	void GLSetUniform_Float(const std::string& name, float value) const override;
	void GLSetUniform_Vec2(const std::string& name, glm::vec2& value) const override;
	void GLSetUniform_Vec2(const std::string& name, const float x, const float y) const override;
	void GLSetUniform_Vec3(const std::string& name, glm::vec3& value) const override;
	void GLSetUniform_Vec3(const std::string& name, const float x, const float y, const float z) const override;
	void GLSetUniform_Vec4(const std::string& name, glm::vec4& value) const override;
	void GLSetUniform_Vec4(const std::string& name, const float x, const float y, const float z, const float w) const override;
	void GLSetUniform_Mat2(const std::string& name, glm::mat2& matrix) const override;
	void GLSetUniform_Mat3(const std::string& name, glm::mat3& matrix) const override;
	void GLSetUniform_Mat4(const std::string& name, glm::mat4& matrix) const override;
	
private:
	void Compile(std::unordered_map<GLenum, std::string> shaderSources);
	bool CheckCompileErrors(unsigned int shader, const std::string& type);
	static bool CheckOpenGLRendererAPI();
	
	uint32 m_ID;
};
