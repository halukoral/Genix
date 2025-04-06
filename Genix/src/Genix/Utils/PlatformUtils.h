#pragma once

class FileDialogs
{
public:
	// These return empty strings if cancelled
	static std::string OpenFile(const char* filter);
	static std::string SaveFile(const char* filter);
};

class Time
{
public:
	static float GetTime();
};

class Rnd
{
public:
	static uint32_t GetRandValue();
};