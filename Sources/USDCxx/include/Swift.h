// Swift.h - Swift C++ interoperability macros for SwiftUSD
// Defines SWIFT_* annotations for seamless Swift/C++ interop

#pragma once

// ============================================================================
// MARK: - System Header Prelude (for Clang module compatibility)
// ============================================================================
// Force-include ALL system headers USD might use BEFORE namespace opens.
// This satisfies Clang modules by ensuring they're imported at global scope.
// Required because USD headers include <limits.h> etc inside PXR_NAMESPACE_OPEN_SCOPE.

// C standard library
#include <cassert>
#include <cctype>
#include <cerrno>
#include <cfloat>
#include <climits>
#include <clocale>
#include <cmath>
#include <csetjmp>
#include <csignal>
#include <cstdarg>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <cwchar>
#include <cwctype>

// C headers (needed by some USD code paths)
#include <limits.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

// C++ STL containers
#include <array>
#include <bitset>
#include <deque>
#include <forward_list>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <vector>

// C++ STL utilities
#include <algorithm>
#include <chrono>
#include <functional>
#include <initializer_list>
#include <iterator>
#include <limits>
#include <memory>
#include <new>
#include <numeric>
#include <random>
#include <ratio>
#include <tuple>
#include <type_traits>
#include <typeindex>
#include <typeinfo>
#include <utility>

// C++ STL strings & streams
#include <fstream>
#include <iomanip>
#include <ios>
#include <iosfwd>
#include <iostream>
#include <istream>
#include <ostream>
#include <sstream>
#include <streambuf>
#include <string>
#include <string_view>

// C++ STL threading & atomics
#include <atomic>
#include <condition_variable>
#include <future>
#include <mutex>
#include <thread>

// C++ STL other
#include <exception>
#include <optional>
#include <stdexcept>
#include <system_error>
#include <variant>

// Platform-specific headers
#ifdef __APPLE__
#include <dispatch/dispatch.h>
#include <os/lock.h>
#include <mach/mach_time.h>
#include <TargetConditionals.h>
#endif

#ifdef __linux__
#include <pthread.h>
#include <dlfcn.h>
#endif

// ============================================================================
// MARK: - USD Mode Configuration
// ============================================================================
// USE_PIXAR_USD is set via Package.swift's swiftSettings:
//   .unsafeFlags(["-Xcc", "-DUSE_PIXAR_USD=1", ...])
//
// When USE_PIXAR_USD is defined:
//   - Real Pixar USD types and Hydra rendering are used
//   - Requires USD libraries in Vendor/USD/darwin/
//
// When USE_PIXAR_USD is NOT defined:
//   - Standalone fallback implementations are used
//   - No external USD library required

#if __has_include(<swift/bridging>)
#include <swift/bridging>
#else
// Fallback definitions when not building with Swift
#define SWIFT_SHARED_REFERENCE(retain, release)
#define SWIFT_IMMORTAL_REFERENCE
#define SWIFT_SELF_CONTAINED
#define SWIFT_UNSAFE_REFERENCE
#define SWIFT_RETURNS_INDEPENDENT_VALUE
#define SWIFT_COMPUTED_PROPERTY
#define SWIFT_NAME(name)
#endif

// Cross-platform noexcept macro
#define SWIFTUSD_NOEXCEPT noexcept

// Platform detection macros
#if defined(__APPLE__)
    #define SWIFTUSD_PLATFORM_APPLE 1
    #define SWIFTUSD_HGI_METAL 1
#elif defined(__linux__)
    #define SWIFTUSD_PLATFORM_LINUX 1
    #define SWIFTUSD_HGI_VULKAN 1
#elif defined(_WIN32)
    #define SWIFTUSD_PLATFORM_WINDOWS 1
    #define SWIFTUSD_HGI_VULKAN 1
#endif

// Architecture detection
#if defined(__x86_64__) || defined(_M_X64)
    #define SWIFTUSD_ARCH_X64 1
#elif defined(__aarch64__) || defined(_M_ARM64)
    #define SWIFTUSD_ARCH_ARM64 1
#endif
