﻿#pragma once
#include "Genix/Common/PrimitiveTypes.h"

enum class ShaderDomain
{
	None = 0, Vertex = 0, Pixel = 1
};

class ShaderUniformDeclaration
{
private:
	friend class Shader;
	friend class OpenGLShader;
	friend class ShaderStruct;
	
public:
	virtual const std::string& GetName() const = 0;
	virtual uint32 GetSize() const = 0;
	virtual uint32 GetCount() const = 0;
	virtual uint32 GetOffset() const = 0;
	virtual ShaderDomain GetDomain() const = 0;
	
protected:
	virtual void SetOffset(uint32 offset) = 0;
};

typedef std::vector<ShaderUniformDeclaration*> ShaderUniformList;

class ShaderUniformBufferDeclaration
{
public:
	virtual const std::string& GetName() const = 0;
	virtual uint32 GetRegister() const = 0;
	virtual uint32 GetSize() const = 0;
	virtual const ShaderUniformList& GetUniformDeclarations() const = 0;

	virtual ShaderUniformDeclaration* FindUniform(const std::string& name) = 0;
};

typedef std::vector<ShaderUniformBufferDeclaration*> ShaderUniformBufferList;

class ShaderStruct
{
private:
	friend class Shader;
	
private:
	std::string m_Name;
	std::vector<ShaderUniformDeclaration*> m_Fields;
	uint32 m_Size;
	uint32 m_Offset;
	
public:
	ShaderStruct(const std::string& name)
		: m_Name(name), m_Size(0), m_Offset(0)
	{
	}

	void AddField(ShaderUniformDeclaration* field)
	{
		m_Size += field->GetSize();
		uint32 offset = 0;
		if (m_Fields.size())
		{
			ShaderUniformDeclaration* previous = m_Fields.back();
			offset = previous->GetOffset() + previous->GetSize();
		}
		field->SetOffset(offset);
		m_Fields.push_back(field);
	}

	void SetOffset(uint32 offset) { m_Offset = offset; }

	const std::string& GetName() const { return m_Name; }
	uint32 GetSize() const { return m_Size; }
	uint32 GetOffset() const { return m_Offset; }
	const std::vector<ShaderUniformDeclaration*>& GetFields() const { return m_Fields; }
};

typedef std::vector<ShaderStruct*> ShaderStructList;

class ShaderResourceDeclaration
{
public:
	virtual const std::string& GetName() const = 0;
	virtual uint32 GetRegister() const = 0;
	virtual uint32 GetCount() const = 0;
};

typedef std::vector<ShaderResourceDeclaration*> ShaderResourceList;