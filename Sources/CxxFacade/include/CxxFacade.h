// CxxFacade.h - Swift/C++ interop prelude
#pragma once

#if __has_include(<swift/bridging>)
#include <swift/bridging>
#else
#define SWIFT_SHARED_REFERENCE(retain, release)
#define SWIFT_IMMORTAL_REFERENCE
#define SWIFT_SELF_CONTAINED
#define SWIFT_UNSAFE_REFERENCE
#define SWIFT_RETURNS_RETAINED
#define SWIFT_RETURNS_INDEPENDENT_VALUE
#endif

#include <atomic>
#include <string>
#include <cstdint>
#include <vector>
#include <memory>

// Platform detection
#if defined(__APPLE__)
    #define CXXFACADE_PLATFORM_APPLE 1
#elif defined(__linux__)
    #define CXXFACADE_PLATFORM_LINUX 1
#elif defined(_WIN32)
    #define CXXFACADE_PLATFORM_WINDOWS 1
#endif

// Namespace
namespace cxxfacade {

/// Get the CxxFacade library version
const char* GetVersion();

/// Get build information (platform)
const char* GetBuildInfo();

} // namespace cxxfacade
