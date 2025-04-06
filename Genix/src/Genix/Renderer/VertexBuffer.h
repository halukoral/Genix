#pragma once
#include "Genix/Common/PrimitiveTypes.h"

enum class ShaderDataType
{
	None = 0, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
};

static uint32 ShaderDataTypeSize(const ShaderDataType type)
{
	switch (type)
	{
	case ShaderDataType::Float:    return 4;
	case ShaderDataType::Float2:   return 4 * 2;
	case ShaderDataType::Float3:   return 4 * 3;
	case ShaderDataType::Float4:   return 4 * 4;
	case ShaderDataType::Mat3:     return 4 * 3 * 3;
	case ShaderDataType::Mat4:     return 4 * 4 * 4;
	case ShaderDataType::Int:      return 4;
	case ShaderDataType::Int2:     return 4 * 2;
	case ShaderDataType::Int3:     return 4 * 3;
	case ShaderDataType::Int4:     return 4 * 4;
	case ShaderDataType::Bool:     return 1;
	}

	ASSERT_CORE(false, "Unknown ShaderDataType!")
	return 0;
}

struct BufferElement
{
	BufferElement(): Type(), Size(0), Offset(0), Normalized(false) { }

	BufferElement(const ShaderDataType type, std::string name, const bool normalized = false)
		: Name(std::move(name)), Type(type), Size(ShaderDataTypeSize(type)), Offset(0), Normalized(normalized)
	{
	}

	uint32 GetComponentCount() const;

	std::string		Name;
	ShaderDataType	Type;
	uint32 			Size;
	size_t	 		Offset;
	bool			Normalized;
};

class BufferLayout
{
public:
	BufferLayout() = default;
	BufferLayout(const std::initializer_list<BufferElement>& elements);

	uint32 GetStride() const { return m_Stride; }
	const std::vector<BufferElement>& GetElements() const { return m_Elements; }

	std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
	std::vector<BufferElement>::iterator end() { return m_Elements.end(); }
	std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
	std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }
	
private:
	void CalculateOffsetsAndStride();

	std::vector<BufferElement> m_Elements;
	uint32 m_Stride = 0;
};

#define MAX_BONE_INFLUENCE 4

struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	glm::vec3 Tangent;
	glm::vec3 Bitangent;

	int BoneIDs[MAX_BONE_INFLUENCE];
	float Weights[MAX_BONE_INFLUENCE];

	// Editor-only
	int EntityID;
};

class VertexBuffer
{
public:
	VertexBuffer(const float* vertices, uint32 size);
	VertexBuffer(const std::vector<Vertex>& vertices);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;

	const BufferLayout& GetLayout() const { return m_Layout; }
	void SetLayout(const BufferLayout& layout) { m_Layout = layout; }

	static Ref<VertexBuffer> Create(float* vertices, uint32 size);
	static Ref<VertexBuffer> Create(std::vector<Vertex> vertices);
	
private:
	uint32 m_Id;
	BufferLayout m_Layout;
};

class IndexBuffer
{
public:
	IndexBuffer(uint32* indices, uint32 count);
	IndexBuffer(const std::vector<uint32>& indices);
	~IndexBuffer();

	void Bind() const;
	void Unbind() const;

	uint32 GetCount() const { return m_Count; }

	static Ref<IndexBuffer> Create(uint32* indices, uint32 size);
	static Ref<IndexBuffer> Create(std::vector<uint32> indices);
	
private:
	uint32 m_Id;
	uint32 m_Count;
};
