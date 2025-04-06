#pragma once

#ifndef NOMINMAX
	// See github.com/skypjack/entt/wiki/Frequently-Asked-Questions#warning-c4003-the-min-the-max-and-the-macro
	#define NOMINMAX
#endif

#include <algorithm>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <memory>
#include <optional>
#include <random>
#include <string>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <thread>
#include <type_traits>
#include <vector>

#include "Genix/Core/Log.h"

#ifdef GX_PLATFORM_WINDOWS
#include <Windows.h>
#endif
