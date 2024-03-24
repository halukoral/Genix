#pragma once
#include "Genix/Common/PrimitiveTypes.h"

// From Cryengine 5.7 (GUID.h)
struct GUID
{
	uint64 hipart;
	uint64 lopart;

	constexpr GUID() : hipart(0),lopart(0) {}

	constexpr GUID(const GUID& rhs) : hipart(rhs.hipart),lopart(rhs.lopart) {}

	constexpr GUID(const uint64& hipart, const uint64& lopart) : hipart(hipart),lopart(lopart) {}

	constexpr static GUID Construct(const uint64& hipart, const uint64& lopart)
	{
		return GUID{ hipart, lopart };
	}
	
	constexpr static GUID Construct(uint32 d1, uint16 d2, uint16 d3,
		uint8 d4_0, uint8 d4_1, uint8 d4_2, uint8 d4_3, uint8 d4_4, uint8 d4_5, uint8 d4_6, uint8 d4_7)
	{
		return GUID(
			(((uint64)d3) << 48) | (((uint64)d2) << 32) | ((uint64)d1),  //high part
			(((uint64)d4_7) << 56) | (((uint64)d4_6) << 48) | (((uint64)d4_5) << 40) | (((uint64)d4_4) << 32) | (((uint64)d4_3) << 24) | (((uint64)d4_2) << 16) | (((uint64)d4_1) << 8) | (uint64)d4_0   //low part
		);
	}

	constexpr static GUID Null()
	{
		return Construct( 0, 0 );
	}

	constexpr bool IsNull() const { return hipart == 0 && lopart == 0; }

	constexpr bool operator==(const GUID& rhs) const { return hipart == rhs.hipart && lopart == rhs.lopart; }
	constexpr bool operator!=(const GUID& rhs) const { return hipart != rhs.hipart || lopart != rhs.lopart; }
	constexpr bool operator<(const GUID& rhs) const  { return hipart < rhs.hipart || ((hipart == rhs.hipart) && lopart < rhs.lopart); }
	constexpr bool operator>(const GUID& rhs) const  { return hipart > rhs.hipart || ((hipart == rhs.hipart) && lopart > rhs.lopart); }
	constexpr bool operator<=(const GUID& rhs) const { return hipart < rhs.hipart || ((hipart == rhs.hipart) && lopart <= rhs.lopart); }
	constexpr bool operator>=(const GUID& rhs) const { return hipart > rhs.hipart || ((hipart == rhs.hipart) && lopart >= rhs.lopart); }

	struct StringUtils
	{
		// Convert hexadecimal character to unsigned char.
		constexpr static uint8 HexCharToUInt8(char x) 
		{
			return  x >= '0' && x <= '9' ? x - '0' :
					x >= 'a' && x <= 'f' ? x - 'a' + 10 :
					x >= 'A' && x <= 'F' ? x - 'A' + 10 : 0;
		}

		// Convert hexadecimal character to unsigned short.
		constexpr static uint16 HexCharToUInt16(char x)
		{
			return HexCharToUInt8(x);
		}

		// Convert hexadecimal character to unsigned long.
		constexpr static uint32 HexCharToUInt32(char x)
		{
			return HexCharToUInt8(x);
		}

		// Convert hexadecimal string to unsigned char.
		constexpr static uint8 HexStringToUInt8(const char* szInput)
		{
			return (HexCharToUInt8(szInput[0]) << 4) + HexCharToUInt8(szInput[1]);
		}

		// Convert hexadecimal string to unsigned short.
		constexpr static uint16 HexStringToUInt16(const char* szInput)
		{
			return  (HexCharToUInt16(szInput[0]) << 12) +
					(HexCharToUInt16(szInput[1]) << 8) +
					(HexCharToUInt16(szInput[2]) << 4) +
					HexCharToUInt16(szInput[3]);
		}

		// Convert hexadecimal string to unsigned long.
		constexpr static uint32 HexStringToUInt32(const char* szInput)
		{
			return  (HexCharToUInt32(szInput[0]) << 28) +
					(HexCharToUInt32(szInput[1]) << 24) +
					(HexCharToUInt32(szInput[2]) << 20) +
					(HexCharToUInt32(szInput[3]) << 16) +
					(HexCharToUInt32(szInput[4]) << 12) +
					(HexCharToUInt32(szInput[5]) << 8) +
					(HexCharToUInt32(szInput[6]) << 4) +
					HexCharToUInt32(szInput[7]);
		}
	};
	
	// Create GUID from string.
	// Ex: "296708CE-F570-4263-B067-C6D8B15990BD"
	constexpr static GUID FromStringInternal(const char* szInput)
	{
		return Construct(
			StringUtils::HexStringToUInt32(szInput),        // uint32 data1
			StringUtils::HexStringToUInt16(szInput + 9),   // uint16 data2
			StringUtils::HexStringToUInt16(szInput + 14),  // uint16 data3
			StringUtils::HexStringToUInt8(szInput + 19),   // uint8  data4[8]
			StringUtils::HexStringToUInt8(szInput + 21),
			StringUtils::HexStringToUInt8(szInput + 24),
			StringUtils::HexStringToUInt8(szInput + 26),
			StringUtils::HexStringToUInt8(szInput + 28),
			StringUtils::HexStringToUInt8(szInput + 30),
			StringUtils::HexStringToUInt8(szInput + 32),
			StringUtils::HexStringToUInt8(szInput + 34)
		);
	}

	// Create GUID from string.
	// Ex: "296708CE-F570-4263-B067-C6D8B15990BD"
	// Ex: "{296708CE-F570-4263-B067-C6D8B15990BD}"
	static GUID FromString(const char* szGuidAsString)
	{
		ASSERT_CORE(szGuidAsString == nullptr, "GUID string is invalid!");
		GUID guid;
		if (szGuidAsString)
		{
			const size_t len = strlen(szGuidAsString);
			if (szGuidAsString[0] == '{' && len >= m_StringArrayCount - 1)
			{
				guid = FromStringInternal(szGuidAsString + 1);
			}
			else if (szGuidAsString[0] != '{' && len >= m_StringArrayCount - 1)
			{
				guid = FromStringInternal(szGuidAsString);
			}
			else if (len <= sizeof(uint64) * 2 && std::all_of(szGuidAsString, szGuidAsString + len, ::isxdigit)) // Convert if it still coming from the old 64bit GUID system.
			{
				guid.hipart = std::strtoull(szGuidAsString, 0, 16);
			}
			else
			{
				ASSERT_CORE(false, "GUID string is invalid: {0}", szGuidAsString);
			}
		}
		return guid;
	}

private:
	static constexpr uint8 m_StringArrayCount = 36; // XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX
};
