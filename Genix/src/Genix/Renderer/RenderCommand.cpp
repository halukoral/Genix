#include "gxpch.h"
#include "RenderCommand.h"

#include "Genix/Platform/OpenGL/OpenGLRendererAPI.h"

RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;
