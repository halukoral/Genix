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
	uint32	 		Offset;
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

class VertexBuffer
{
public:
	VertexBuffer() = default;
	virtual ~VertexBuffer() = default;

	virtual void Bind() const = 0;
	virtual void Unbind() const = 0;

	virtual const BufferLayout& GetLayout() const = 0;
	virtual void SetLayout(const BufferLayout& layout) = 0;
	
	static Ref<VertexBuffer> Create(float* vertices, uint32 size);
};

class IndexBuffer
{
public:
	IndexBuffer() = default;
	virtual ~IndexBuffer() = default;

	virtual void Bind() const = 0;
	virtual void Unbind() const = 0;

	virtual uint32 GetCount() const = 0;

	static Ref<IndexBuffer> Create(uint32* indices, uint32 size);
};
