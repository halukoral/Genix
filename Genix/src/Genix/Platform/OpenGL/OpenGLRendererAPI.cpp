#include "gxpch.h"
#include "OpenGLRendererAPI.h"

#include <glad/glad.h>

void OpenGLMessageCallback(
	unsigned source,
	unsigned type,
	unsigned id,
	unsigned severity,
	int length,
	const char* message,
	const void* userParam)
{
	switch (severity)
	{
	case GL_DEBUG_SEVERITY_HIGH:         LOG_CORE_CRITICAL(message); return;
	case GL_DEBUG_SEVERITY_MEDIUM:       LOG_CORE_ERROR(message); return;
	case GL_DEBUG_SEVERITY_LOW:          LOG_CORE_WARN(message); return;
	case GL_DEBUG_SEVERITY_NOTIFICATION: LOG_CORE_TRACE(message); return;
	}

	ASSERT_CORE(false, "Unknown severity level!");
}

void OpenGLRendererAPI::Init()
{
#ifdef GX_DEBUG
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(OpenGLMessageCallback, nullptr);

	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
#endif
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void OpenGLRendererAPI::SetViewport(uint32 x, uint32 y, uint32 width, uint32 height)
{
	glViewport(x, y, width, height);
}

void OpenGLRendererAPI::SetClearColor(const glm::vec4& color)
{
	glClearColor(color.r, color.g, color.b, color.a);
}

void OpenGLRendererAPI::SetActiveTexture(const uint8 slot)
{
	glActiveTexture(GL_TEXTURE0 + slot);
}

void OpenGLRendererAPI::Clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray)
{
	glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
}
