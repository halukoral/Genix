#include "gxpch.h"
#include "OpenGLMesh.h"
#include "OpenGLShader.h"

#include "Genix/Renderer/RenderCommand.h"
#include "Genix/Renderer/Texture.h"
#include "Genix/Renderer/VertexArray.h"

OpenGLMesh::OpenGLMesh(const std::vector<VertexData>& vertices, std::vector<uint32>& indices, const std::vector<Ref<Texture>>& textures)
{
	m_Vertices = vertices;
	m_Indices = indices;
	m_Textures = textures;

	// now that we have all the required data, set the vertex buffers and its attribute pointers.
	SetupMesh();
}

void OpenGLMesh::Draw(const Ref<Shader>& shader)
{
	// bind appropriate textures
	uint8 DiffuseNr  = 1;
	uint8 SpecularNr = 1;
	uint8 NormalNr   = 1;
	uint8 HeightNr   = 1;
	
	for(uint8 i = 0; i < m_Textures.size(); i++)
	{
		RenderCommand::SetActiveTexture(i);
		// retrieve texture number (the N in diffuse_textureN)
		std::string Name;
		switch (m_Textures[i]->GetTextureType())
		{
		case TextureType::Diffuse:
			Name = "texture_diffuse" + std::to_string(DiffuseNr++);
			break;
		case TextureType::Specular:
			Name = "texture_specular" + std::to_string(SpecularNr++);
			break;
		case TextureType::Normal:
			Name = "texture_normal" + std::to_string(NormalNr++);
			break;
		case TextureType::Height:
			Name = "texture_height" + std::to_string(HeightNr++);
			break;
		}

		const Ref<OpenGLShader> openGLShader = std::dynamic_pointer_cast<OpenGLShader>(shader);
		ASSERT_CORE(openGLShader, "openGLShader is nullptr!")
		
		// now set the sampler to the correct texture unit
		openGLShader->GLSetUniform_Int(Name,i);
		// and finally bind the texture
		m_Textures[i]->Bind(i);
	}

	m_VertexArray->Bind();
	RenderCommand::DrawIndexed(m_VertexArray);

	// always good practice to set everything back to defaults once configured.
	RenderCommand::SetActiveTexture(0);
}

void OpenGLMesh::SetupMesh()
{
	// VAO
	m_VertexArray = VertexArray::Create();

	// VBO
	m_VertexBuffer = VertexBuffer::Create(m_Vertices);
	m_VertexBuffer->SetLayout(
	{
		{ ShaderDataType::Float3, "a_Position" },
		{ ShaderDataType::Float3, "a_Normal" },
		{ ShaderDataType::Float2, "a_TexCoord" },
		{ ShaderDataType::Float3, "a_Tangent" },
		{ ShaderDataType::Float3, "a_Bitangent" },
		{ ShaderDataType::Int4, "a_BoneId" },
		{ ShaderDataType::Float4, "a_Weight" }
	});	
	m_VertexArray->AddVertexBuffer(m_VertexBuffer);

	// EBO
	m_IndexBuffer = IndexBuffer::Create(m_Indices);
	m_VertexArray->SetIndexBuffer(m_IndexBuffer);
	
	m_VertexArray->Unbind();
}
