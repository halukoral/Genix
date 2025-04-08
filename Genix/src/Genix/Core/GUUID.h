#pragma once
#include "Genix/Common/PrimitiveTypes.h"

class GUUID
{
public:
	GUUID();
	GUUID(uint64 uuid);
	GUUID(const GUUID&) = default;

	operator uint64() const { return m_UUID; }
private:
	uint64 m_UUID;
};
