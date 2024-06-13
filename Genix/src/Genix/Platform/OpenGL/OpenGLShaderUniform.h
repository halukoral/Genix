#pragma once
#include "Genix/Renderer/ShaderUniform.h"

class OpenGLShaderResourceDeclaration : public ShaderResourceDeclaration
{
public:
	enum class Type
	{
		NONE, TEXTURE2D, TEXTURECUBE
	};
	
private:
	friend class OpenGLShader;
	
private:
	std::string m_Name;
	uint32 m_Register = 0;
	uint32 m_Count;
	Type m_Type;
	
public:
	OpenGLShaderResourceDeclaration(Type type, const std::string& name, uint32 count);

	const std::string& GetName() const override { return m_Name; }
	uint32 GetRegister() const override { return m_Register; }
	uint32 GetCount() const override { return m_Count; }

	Type GetType() const { return m_Type; }
	
public:
	static Type StringToType(const std::string& type);
	static std::string TypeToString(Type type);
};

class OpenGLShaderUniformDeclaration : public ShaderUniformDeclaration
{
private:
	friend class OpenGLShader;
	friend class OpenGLShaderUniformBufferDeclaration;
	
public:
	enum class Type
	{
		NONE, FLOAT32, VEC2, VEC3, VEC4, MAT3, MAT4, INT32, STRUCT
	};
	
private:
	std::string m_Name;
	uint32 m_Size;
	uint32 m_Count;
	uint32 m_Offset;
	ShaderDomain m_Domain;

	Type m_Type;
	ShaderStruct* m_Struct;
	mutable int32 m_Location;
	
public:
	OpenGLShaderUniformDeclaration(ShaderDomain domain, Type type, const std::string& name, uint32 count = 1);
	OpenGLShaderUniformDeclaration(ShaderDomain domain, ShaderStruct* uniformStruct, const std::string& name, uint32 count = 1);

	const std::string& GetName() const override { return m_Name; }
	uint32 GetSize() const override { return m_Size; }
	uint32 GetCount() const override { return m_Count; }
	uint32 GetOffset() const override { return m_Offset; }
	uint32 GetAbsoluteOffset() const { return m_Struct ? m_Struct->GetOffset() + m_Offset : m_Offset; }
	ShaderDomain GetDomain() const { return m_Domain; }

	int32 GetLocation() const { return m_Location; }
	Type GetType() const { return m_Type; }
	bool IsArray() const { return m_Count > 1; }
	const ShaderStruct& GetShaderUniformStruct() const { ASSERT_CORE(m_Struct, ""); return *m_Struct; }
protected:
	void SetOffset(uint32 offset) override;
public:
	static uint32 SizeOfUniformType(Type type);
	static Type StringToType(const std::string& type);
	static std::string TypeToString(Type type);
};

struct GLShaderUniformField
{
	OpenGLShaderUniformDeclaration::Type type;
	std::string name;
	uint32 count;
	mutable uint32 size;
	mutable int32 location;
};

class OpenGLShaderUniformBufferDeclaration : public ShaderUniformBufferDeclaration
{
private:
	friend class Shader;
	
private:
	std::string m_Name;
	ShaderUniformList m_Uniforms;
	uint32 m_Register;
	uint32 m_Size;
	ShaderDomain m_Domain;
	
public:
	OpenGLShaderUniformBufferDeclaration(const std::string& name, ShaderDomain domain);

	void PushUniform(OpenGLShaderUniformDeclaration* uniform);

	const std::string& GetName() const override { return m_Name; }
	uint32 GetRegister() const override { return m_Register; }
	uint32 GetSize() const override { return m_Size; }
	virtual ShaderDomain GetDomain() const { return m_Domain; }
	const ShaderUniformList& GetUniformDeclarations() const override { return m_Uniforms; }

	ShaderUniformDeclaration* FindUniform(const std::string& name);
};