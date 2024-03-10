#pragma once

enum class RendererAPI
{
	None	= 0,
	OpenGL	= 1,
	DirectX = 2
};

class GENIX_API Renderer
{
public:
	static RendererAPI GetAPI() { return s_RendererAPI; }

private:
	static RendererAPI s_RendererAPI;
};
