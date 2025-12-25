// Swift.h - Swift C++ interoperability macros for SwiftUSD
// Defines SWIFT_* annotations for seamless Swift/C++ interop

#pragma once

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
