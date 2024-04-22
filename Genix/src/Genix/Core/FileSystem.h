#pragma once
#include "Genix/Core/Buffer.h"

class FileSystem
{
public:
	static Buffer ReadFileBinary(const std::filesystem::path& filepath);
};
