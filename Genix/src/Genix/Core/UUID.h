#pragma once
#include "Genix/Common/PrimitiveTypes.h"

class UUID
{
public:
	UUID();
	UUID(uint64 uuid);
	UUID(const UUID&) = default;

	operator uint64() const { return m_UUID; }
private:
	uint64 m_UUID;
};

namespace std {
	template <typename T> struct hash;
	
	template<>
	struct hash<UUID>
	{
		std::size_t operator()(const UUID& uuid) const
		{
			return uuid;
		}
	};

}