#pragma once

class Shader
{
public:
	virtual ~Shader() = default;

	virtual void Bind() const = 0;
	virtual void Unbind() const = 0;

	// utility OpenGL uniform functions
	// ------------------------------------------------------------------------
	virtual void GLSetUniform_Bool(const std::string& name, bool value) const = 0;
	virtual void GLSetUniform_Int(const std::string& name, int value) const = 0;
	virtual void GLSetUniform_Float(const std::string& name, float value) const = 0;
	virtual void GLSetUniform_Vec2(const std::string& name, glm::vec2& value) const = 0;
	virtual void GLSetUniform_Vec2(const std::string& name, const float x, const float y) const = 0;
	virtual void GLSetUniform_Vec3(const std::string& name, glm::vec3& value) const = 0;
	virtual void GLSetUniform_Vec3(const std::string& name, const float x, const float y, const float z) const = 0;
	virtual void GLSetUniform_Vec4(const std::string& name, glm::vec4& value) const = 0;
	virtual void GLSetUniform_Vec4(const std::string& name, const float x, const float y, const float z, const float w) const = 0;
	virtual void GLSetUniform_Mat2(const std::string& name, glm::mat2& matrix) const = 0;
	virtual void GLSetUniform_Mat3(const std::string& name, glm::mat3& matrix) const = 0;
	virtual void GLSetUniform_Mat4(const std::string& name, glm::mat4& matrix) const = 0;

	static Ref<Shader> Create(const std::string& vertexSrc, const std::string& fragmentSrc);
};
