﻿#pragma once
#include "Genix/Renderer/RendererAPI.h"

class OpenGLRendererAPI : public RendererAPI
{
public:
	void Init() override;
	void SetViewport(uint32 x, uint32 y, uint32 width, uint32 height) override;
	void SetClearColor(const glm::vec4& color) override;
	void SetActiveTexture(const uint8 slot) override;
	void Clear() override;
	void ResetFrameBuffer() override;
	void SetBlendFunc(BlendFuncArgument arg1, BlendFuncArgument arg2) override;
	
	void DrawIndexed(const Ref<VertexArray>& vertexArray) override;

	void RenderCube() override; 
	void RenderSphere() override;
	void RenderQuad() override; 
};