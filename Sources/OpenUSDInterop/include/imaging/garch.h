// garch.h - Garch C wrapper
// Mirrors: pxr/imaging/garch/*.h (Graphics Architecture - GL context, platform detection)

#ifndef OPENUSDINTEROP_IMAGING_GARCH_H
#define OPENUSDINTEROP_IMAGING_GARCH_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#ifndef USD_INTEROP_API
#if defined(_WIN32)
    #if defined(OPENUSDINTEROP_EXPORTS)
        #define USD_INTEROP_API __declspec(dllexport)
    #else
        #define USD_INTEROP_API __declspec(dllimport)
    #endif
#else
    #define USD_INTEROP_API __attribute__((visibility("default")))
#endif
#endif

#ifndef USD_RESULT_DEFINED
#define USD_RESULT_DEFINED
typedef enum {
    USD_RESULT_SUCCESS = 0,
    USD_RESULT_ERROR = 1,
    USD_RESULT_INVALID_ARGUMENT = 2,
    USD_RESULT_NOT_FOUND = 3,
    USD_RESULT_ALREADY_EXISTS = 4,
    USD_RESULT_OUT_OF_MEMORY = 5,
    USD_RESULT_IO_ERROR = 6
} UsdResult;
#endif

#ifdef __cplusplus
extern "C" {
#endif

// ============================================================================
// MARK: - Opaque Handle Types
// ============================================================================

/// Opaque handle for GL platform context state
typedef struct GarchGLContextStateOpaque* GarchGLContextStateRef;

/// Opaque handle for GL platform debug context
typedef struct GarchGLDebugContextOpaque* GarchGLDebugContextRef;

// ============================================================================
// MARK: - Platform Detection
// ============================================================================

/// Graphics platform type
typedef enum {
    GARCH_PLATFORM_NONE = 0,      ///< No graphics platform available
    GARCH_PLATFORM_OPENGL = 1,    ///< OpenGL is available
    GARCH_PLATFORM_METAL = 2,     ///< Metal is available (macOS/iOS only)
    GARCH_PLATFORM_VULKAN = 3     ///< Vulkan is available
} GarchPlatformType;

/// Returns the primary graphics platform for this system.
USD_INTEROP_API GarchPlatformType Garch_GetPlatform(void);

/// Returns true if OpenGL is available on this system.
USD_INTEROP_API bool Garch_HasOpenGL(void);

/// Returns true if Metal is available on this system (macOS/iOS only).
USD_INTEROP_API bool Garch_HasMetal(void);

/// Returns true if Vulkan is available on this system.
USD_INTEROP_API bool Garch_HasVulkan(void);

// ============================================================================
// MARK: - GL Platform Context State
// ============================================================================

/// Creates a context state capturing the current GL context.
/// Returns NULL if no context is current or on error.
/// Caller must release with GarchGLContextState_Release.
USD_INTEROP_API GarchGLContextStateRef GarchGLContextState_GetCurrent(void);

/// Creates a null/empty context state.
/// Caller must release with GarchGLContextState_Release.
USD_INTEROP_API GarchGLContextStateRef GarchGLContextState_CreateNull(void);

/// Retains a context state reference.
USD_INTEROP_API GarchGLContextStateRef GarchGLContextState_Retain(GarchGLContextStateRef state);

/// Releases a context state reference.
USD_INTEROP_API void GarchGLContextState_Release(GarchGLContextStateRef state);

/// Returns true if the context state is valid (not null/empty).
USD_INTEROP_API bool GarchGLContextState_IsValid(GarchGLContextStateRef state);

/// Returns a hash value for the context state.
USD_INTEROP_API size_t GarchGLContextState_GetHash(GarchGLContextStateRef state);

/// Compares two context states for equality.
USD_INTEROP_API bool GarchGLContextState_Equals(GarchGLContextStateRef a, GarchGLContextStateRef b);

/// Makes this context state current.
USD_INTEROP_API UsdResult GarchGLContextState_MakeCurrent(GarchGLContextStateRef state);

/// Clears the current GL context (makes no context current).
USD_INTEROP_API void GarchGLContextState_DoneCurrent(void);

// ============================================================================
// MARK: - GL Platform Debug Context
// ============================================================================

/// Creates a new GL debug context with specified version.
/// - Parameters:
///   - majorVersion: Requested OpenGL major version (e.g., 4)
///   - minorVersion: Requested OpenGL minor version (e.g., 1)
///   - coreProfile: Whether to use core profile (true) or compatibility (false)
///   - directRendering: Whether to use direct rendering
/// Returns NULL on failure. Caller must release with GarchGLDebugContext_Release.
USD_INTEROP_API GarchGLDebugContextRef GarchGLDebugContext_Create(
    int majorVersion,
    int minorVersion,
    bool coreProfile,
    bool directRendering
);

/// Retains a debug context reference.
USD_INTEROP_API GarchGLDebugContextRef GarchGLDebugContext_Retain(GarchGLDebugContextRef ctx);

/// Releases a debug context reference.
USD_INTEROP_API void GarchGLDebugContext_Release(GarchGLDebugContextRef ctx);

/// Makes this debug context current.
USD_INTEROP_API UsdResult GarchGLDebugContext_MakeCurrent(GarchGLDebugContextRef ctx);

/// Returns true if debug output is enabled globally.
USD_INTEROP_API bool GarchGLDebugContext_IsEnabledDebugOutput(void);

/// Returns true if core profile is enabled globally.
USD_INTEROP_API bool GarchGLDebugContext_IsEnabledCoreProfile(void);

// ============================================================================
// MARK: - GL API Information
// ============================================================================

/// GL API version information
typedef struct {
    int majorVersion;
    int minorVersion;
    bool isCoreProfile;
    bool isES;          ///< OpenGL ES
    bool hasDebugOutput;
} GarchGLAPIInfo;

/// Gets information about the current GL context's API.
/// Returns false if no context is current.
USD_INTEROP_API bool Garch_GetGLAPIInfo(GarchGLAPIInfo* outInfo);

/// Returns the GL version string for the current context.
/// Caller must free the returned string with Garch_FreeString.
USD_INTEROP_API UsdResult Garch_GetGLVersionString(char** outVersion);

/// Returns the GL renderer string for the current context.
/// Caller must free the returned string with Garch_FreeString.
USD_INTEROP_API UsdResult Garch_GetGLRendererString(char** outRenderer);

/// Returns the GL vendor string for the current context.
/// Caller must free the returned string with Garch_FreeString.
USD_INTEROP_API UsdResult Garch_GetGLVendorString(char** outVendor);

/// Returns the GLSL version string for the current context.
/// Caller must free the returned string with Garch_FreeString.
USD_INTEROP_API UsdResult Garch_GetGLSLVersionString(char** outVersion);

/// Frees a string allocated by Garch functions.
USD_INTEROP_API void Garch_FreeString(char* str);

// ============================================================================
// MARK: - GL Extension Checking
// ============================================================================

/// Returns the count of supported GL extensions.
USD_INTEROP_API size_t Garch_GetGLExtensionCount(void);

/// Gets GL extension names.
/// outExtensions must have space for maxCount strings.
/// Each string is statically owned and should NOT be freed.
/// Returns the number of extensions written.
USD_INTEROP_API size_t Garch_GetGLExtensions(
    const char** outExtensions,
    size_t maxCount
);

/// Returns true if the specified GL extension is supported.
USD_INTEROP_API bool Garch_HasGLExtension(const char* extensionName);

#ifdef __cplusplus
}
#endif

#endif // OPENUSDINTEROP_IMAGING_GARCH_H
