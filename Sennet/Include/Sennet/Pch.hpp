#pragma once

#include <algorithm>
#include <chrono>
#include <cstdint>
#include <iosfwd>
#include <iostream>
#include <stdexcept>

#include <array>
#include <atomic>
#include <fstream>
#include <functional>
#include <map>
#include <memory>
#include <mutex>
#include <queue>
#include <sstream>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

#include "Sennet/Core/Log.hpp"
#include "Sennet/Debug/Instrumentor.hpp"

#ifdef SENNET_PLATFORM_WINDOWS
#include <Windows.h>
#endif