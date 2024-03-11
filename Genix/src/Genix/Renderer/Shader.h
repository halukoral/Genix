#pragma once

#include <glad/glad.h>
#include "Genix/Common/PrimitiveTypes.h"

class GENIX_API Shader
{
public:
	Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
	~Shader();

	void Bind() const;
	void Unbind() const;

private:
	void Compile(std::unordered_map<GLenum, std::string> shaderSources);
	bool CheckCompileErrors(unsigned int shader, const std::string& type);
	
	uint32 m_ID;
};
