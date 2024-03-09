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
	
	uint32_t m_RendererID;
};
