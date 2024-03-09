#pragma once

#include <glad/glad.h>

class Shader
{
public:
	Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
	~Shader();

	void Bind() const;
	void Unbind() const;

private:
	void Compile(std::unordered_map<GLenum, std::string> shaderSources);
	bool CheckCompileErrors(unsigned int shader, const std::string& type);
	
	uint32_t m_RendererID;
};
