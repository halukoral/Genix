#include "gxpch.h"
#include "GUUID.h"

static std::random_device s_RandomDevice;
static std::mt19937_64 s_Engine(s_RandomDevice());
static std::uniform_int_distribution<uint64> s_UniformDistribution;

GUUID::GUUID() : m_UUID(s_UniformDistribution(s_Engine)) { }

GUUID::GUUID(uint64 uuid) : m_UUID(uuid) { }
