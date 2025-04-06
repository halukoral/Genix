#pragma once
#include "Genix/Common/PrimitiveTypes.h"

// Non-owning raw buffer class
struct Buffer
{
	uint8* Data = nullptr;
	uint64 Size = 0;

	Buffer() = default;

	Buffer(uint64 size)
	{
		Allocate(size);
	}

	Buffer(const Buffer&) = default;

	static Buffer Copy(Buffer other)
	{
		Buffer result(other.Size);
		memcpy(result.Data, other.Data, other.Size);
		return result;
	}

	void Allocate(uint64 size)
	{
		Release();

		Data = new uint8[size];
		Size = size;
	}

	void Release()
	{
		delete[] Data;
		Data = nullptr;
		Size = 0;
	}

	template<typename T>
	T* As()
	{
		return (T*)Data;
	}

	operator bool() const
	{
		return (bool)Data;
	}

};

struct ScopedBuffer
{
	ScopedBuffer(Buffer buffer)
		: m_Buffer(buffer)
	{
	}

	ScopedBuffer(uint64 size)
		: m_Buffer(size)
	{
	}

	~ScopedBuffer()
	{
		m_Buffer.Release();
	}

	uint8* Data() { return m_Buffer.Data; }
	uint64 Size() { return m_Buffer.Size; }

	template<typename T>
	T* As()
	{
		return m_Buffer.As<T>();
	}

	operator bool() const { return m_Buffer; }
private:
	Buffer m_Buffer;
};
