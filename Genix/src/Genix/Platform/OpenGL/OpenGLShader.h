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
	
	// utility uniform functions
	// ------------------------------------------------------------------------
	void UploadUniform_Bool(const std::string& name, bool value) const;
	void UploadUniform_Int(const std::string& name, int value) const;
	void UploadUniform_Float(const std::string& name, float value) const;
	void UploadUniform_Vec2(const std::string& name, glm::vec2& value) const;
	void UploadUniform_Vec2(const std::string& name, const float x, const float y) const;
	void UploadUniform_Vec3(const std::string& name, glm::vec3& value) const;
	void UploadUniform_Vec3(const std::string& name, const float x, const float y, const float z) const;
	void UploadUniform_Vec4(const std::string& name, glm::vec4& value) const;
	void UploadUniform_Vec4(const std::string& name, const float x, const float y, const float z, const float w) const;
	void UploadUniform_Mat2(const std::string& name, glm::mat2& matrix) const;
	void UploadUniform_Mat3(const std::string& name, glm::mat3& matrix) const;
	void UploadUniform_Mat4(const std::string& name, glm::mat4& matrix) const;
	
private:
	void Compile(std::unordered_map<GLenum, std::string> shaderSources);
	bool CheckCompileErrors(unsigned int shader, const std::string& type);
	
	uint32 m_ID;
};
