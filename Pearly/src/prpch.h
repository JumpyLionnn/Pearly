#pragma once

#include <memory>
#include <string>
#include <sstream>
#include <functional>
#include <vector>
#include <utility>
#include <initializer_list>
#include <unordered_map>
#include <fstream>
#include <array>
#include <algorithm>
#include <chrono>
#include <thread>
#include <mutex>
#include <fstream>

#include "Pearly/Core/Log.h"
#include "Pearly/Debug/Instrumentor.h"

#ifdef PR_PLATFORM_WINDOWS
	#include <Windows.h>
#endif

