// garch.cpp - Garch C wrapper implementation
// Mirrors: pxr/imaging/garch/*.h (Graphics Architecture - GL context, platform detection)

#include "../../include/imaging/garch.h"

// Check if we should use full USD library
#if defined(USE_PIXAR_USD) && USE_PIXAR_USD
    #define GARCH_USE_USD 1
    #include <pxr/imaging/garch/glApi.h>
    #include <pxr/imaging/garch/glPlatformContext.h>
    #include <pxr/imaging/garch/glPlatformDebugContext.h>
    PXR_NAMESPACE_USING_DIRECTIVE
#else
    #define GARCH_USE_USD 0
#endif

#include <cstring>
#include <cstdlib>
#include <string>
#include <vector>
#include <atomic>

#if defined(__APPLE__)
#include <TargetConditionals.h>
#include <dlfcn.h>
#if TARGET_OS_OSX
    // macOS - check for OpenGL and Metal
    #include <AvailabilityMacros.h>
    // OpenGL is deprecated but still available on macOS
    #define HAS_OPENGL 1
    #define HAS_METAL 1
    #define HAS_VULKAN 0  // MoltenVK exists but not native
#else
    // iOS/tvOS - Metal only
    #define HAS_OPENGL 0
    #define HAS_METAL 1
    #define HAS_VULKAN 0
#endif
#elif defined(__linux__)
    #define HAS_OPENGL 1
    #define HAS_METAL 0
    #define HAS_VULKAN 1
    #include <dlfcn.h>
#elif defined(_WIN32)
    #define HAS_OPENGL 1
    #define HAS_METAL 0
    #define HAS_VULKAN 1
    #include <windows.h>
#else
    #define HAS_OPENGL 0
    #define HAS_METAL 0
    #define HAS_VULKAN 0
#endif

// Helper to duplicate a std::string as a C string
static char* DuplicateString(const std::string& str) {
    char* result = static_cast<char*>(malloc(str.size() + 1));
    if (result) {
        memcpy(result, str.c_str(), str.size() + 1);
    }
    return result;
}

// ============================================================================
// MARK: - GL Context State Wrapper
// ============================================================================

struct GarchGLContextStateOpaque {
    std::atomic<int> refCount;
    bool isNull;
#if GARCH_USE_USD
    GarchGLPlatformContextState state;
#endif

    GarchGLContextStateOpaque() : refCount(1), isNull(true) {}
};

// ============================================================================
// MARK: - GL Debug Context Wrapper
// ============================================================================

struct GarchGLDebugContextOpaque {
    std::atomic<int> refCount;
#if GARCH_USE_USD
    GarchGLPlatformDebugContextRefPtr context;
#endif
    int majorVersion;
    int minorVersion;
    bool coreProfile;

    GarchGLDebugContextOpaque() : refCount(1), majorVersion(4), minorVersion(1), coreProfile(true) {}
};

extern "C" {

// ============================================================================
// MARK: - Platform Detection
// ============================================================================

GarchPlatformType Garch_GetPlatform(void) {
#if GARCH_USE_USD
    // When using USD, prefer OpenGL as that's what Hydra uses
    return GARCH_PLATFORM_OPENGL;
#else
    #if HAS_METAL && defined(__APPLE__)
    // On Apple platforms, prefer Metal
    return GARCH_PLATFORM_METAL;
    #elif HAS_OPENGL
    return GARCH_PLATFORM_OPENGL;
    #elif HAS_VULKAN
    return GARCH_PLATFORM_VULKAN;
    #else
    return GARCH_PLATFORM_NONE;
    #endif
#endif
}

bool Garch_HasOpenGL(void) {
#if GARCH_USE_USD
    // If USD is available, OpenGL is supported
    return true;
#else
    #if HAS_OPENGL
        #if defined(__APPLE__)
        // Check if OpenGL framework is available
        void* handle = dlopen("/System/Library/Frameworks/OpenGL.framework/OpenGL", RTLD_LAZY);
        if (handle) {
            dlclose(handle);
            return true;
        }
        return false;
        #elif defined(__linux__)
        // Check if libGL is available
        void* handle = dlopen("libGL.so.1", RTLD_LAZY);
        if (handle) {
            dlclose(handle);
            return true;
        }
        return false;
        #elif defined(_WIN32)
        // OpenGL32.dll is always present on Windows
        return true;
        #else
        return false;
        #endif
    #else
    return false;
    #endif
#endif
}

bool Garch_HasMetal(void) {
#if HAS_METAL && defined(__APPLE__)
    // Check if Metal framework is available
    void* handle = dlopen("/System/Library/Frameworks/Metal.framework/Metal", RTLD_LAZY);
    if (handle) {
        dlclose(handle);
        return true;
    }
#endif
    return false;
}

bool Garch_HasVulkan(void) {
#if HAS_VULKAN
    #if defined(__linux__)
    void* handle = dlopen("libvulkan.so.1", RTLD_LAZY);
    if (handle) {
        dlclose(handle);
        return true;
    }
    #elif defined(_WIN32)
    HMODULE handle = LoadLibraryA("vulkan-1.dll");
    if (handle) {
        FreeLibrary(handle);
        return true;
    }
    #endif
#endif
    return false;
}

// ============================================================================
// MARK: - GL Platform Context State
// ============================================================================

GarchGLContextStateRef GarchGLContextState_GetCurrent(void) {
#if GARCH_USE_USD
    try {
        auto* wrapper = new GarchGLContextStateOpaque();
        wrapper->state = GarchGLPlatformContextState();
        wrapper->isNull = !wrapper->state.IsValid();
        return wrapper;
    } catch (...) {
        return nullptr;
    }
#else
    // Standalone mode: return null state since we don't have GL context
    auto* wrapper = new GarchGLContextStateOpaque();
    wrapper->isNull = true;
    return wrapper;
#endif
}

GarchGLContextStateRef GarchGLContextState_CreateNull(void) {
    auto* wrapper = new GarchGLContextStateOpaque();
    wrapper->isNull = true;
#if GARCH_USE_USD
    wrapper->state = GarchGetNullGLPlatformContextState();
#endif
    return wrapper;
}

GarchGLContextStateRef GarchGLContextState_Retain(GarchGLContextStateRef state) {
    if (state) {
        state->refCount.fetch_add(1, std::memory_order_relaxed);
    }
    return state;
}

void GarchGLContextState_Release(GarchGLContextStateRef state) {
    if (state) {
        if (state->refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) {
            delete state;
        }
    }
}

bool GarchGLContextState_IsValid(GarchGLContextStateRef state) {
    if (!state) return false;
#if GARCH_USE_USD
    try {
        return state->state.IsValid();
    } catch (...) {
        return false;
    }
#else
    return !state->isNull;
#endif
}

size_t GarchGLContextState_GetHash(GarchGLContextStateRef state) {
    if (!state) return 0;
#if GARCH_USE_USD
    try {
        return state->state.GetHash();
    } catch (...) {
        return 0;
    }
#else
    return state->isNull ? 0 : reinterpret_cast<size_t>(state);
#endif
}

bool GarchGLContextState_Equals(GarchGLContextStateRef a, GarchGLContextStateRef b) {
    if (!a && !b) return true;
    if (!a || !b) return false;
#if GARCH_USE_USD
    try {
        return a->state == b->state;
    } catch (...) {
        return false;
    }
#else
    return a->isNull == b->isNull;
#endif
}

UsdResult GarchGLContextState_MakeCurrent(GarchGLContextStateRef state) {
    if (!state) return USD_RESULT_INVALID_ARGUMENT;
#if GARCH_USE_USD
    try {
        state->state.MakeCurrent();
        return USD_RESULT_SUCCESS;
    } catch (...) {
        return USD_RESULT_ERROR;
    }
#else
    // Standalone mode: no-op
    return USD_RESULT_SUCCESS;
#endif
}

void GarchGLContextState_DoneCurrent(void) {
#if GARCH_USE_USD
    try {
        GarchGLPlatformContextState::DoneCurrent();
    } catch (...) {
        // Ignore errors
    }
#endif
    // Standalone mode: no-op
}

// ============================================================================
// MARK: - GL Platform Debug Context
// ============================================================================

GarchGLDebugContextRef GarchGLDebugContext_Create(
    int majorVersion,
    int minorVersion,
    bool coreProfile,
    bool directRendering
) {
#if GARCH_USE_USD
    try {
        auto* wrapper = new GarchGLDebugContextOpaque();
        wrapper->majorVersion = majorVersion;
        wrapper->minorVersion = minorVersion;
        wrapper->coreProfile = coreProfile;
        wrapper->context = GarchGLPlatformDebugContext::New(
            majorVersion, minorVersion, coreProfile, directRendering
        );
        if (!wrapper->context) {
            delete wrapper;
            return nullptr;
        }
        return wrapper;
    } catch (...) {
        return nullptr;
    }
#else
    // Standalone mode: create a stub debug context
    auto* wrapper = new GarchGLDebugContextOpaque();
    wrapper->majorVersion = majorVersion;
    wrapper->minorVersion = minorVersion;
    wrapper->coreProfile = coreProfile;
    return wrapper;
#endif
}

GarchGLDebugContextRef GarchGLDebugContext_Retain(GarchGLDebugContextRef ctx) {
    if (ctx) {
        ctx->refCount.fetch_add(1, std::memory_order_relaxed);
    }
    return ctx;
}

void GarchGLDebugContext_Release(GarchGLDebugContextRef ctx) {
    if (ctx) {
        if (ctx->refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) {
            delete ctx;
        }
    }
}

UsdResult GarchGLDebugContext_MakeCurrent(GarchGLDebugContextRef ctx) {
    if (!ctx) return USD_RESULT_INVALID_ARGUMENT;
#if GARCH_USE_USD
    try {
        if (ctx->context) {
            ctx->context->makeCurrent();
            return USD_RESULT_SUCCESS;
        }
        return USD_RESULT_ERROR;
    } catch (...) {
        return USD_RESULT_ERROR;
    }
#else
    // Standalone mode: no-op
    return USD_RESULT_SUCCESS;
#endif
}

bool GarchGLDebugContext_IsEnabledDebugOutput(void) {
#if GARCH_USE_USD
    try {
        return GarchGLPlatformDebugContext::IsEnabledDebugOutput();
    } catch (...) {
        return false;
    }
#else
    return false;
#endif
}

bool GarchGLDebugContext_IsEnabledCoreProfile(void) {
#if GARCH_USE_USD
    try {
        return GarchGLPlatformDebugContext::IsEnabledCoreProfile();
    } catch (...) {
        return true; // Default to core profile
    }
#else
    return true; // Default to core profile
#endif
}

// ============================================================================
// MARK: - GL API Information
// ============================================================================

bool Garch_GetGLAPIInfo(GarchGLAPIInfo* outInfo) {
    if (!outInfo) return false;

    // Initialize to defaults
    outInfo->majorVersion = 0;
    outInfo->minorVersion = 0;
    outInfo->isCoreProfile = false;
    outInfo->isES = false;
    outInfo->hasDebugOutput = false;

#if GARCH_USE_USD
    try {
        // glGetIntegerv is exposed through glApi.h
        if (glGetIntegerv) {
            GLint major = 0, minor = 0;
            glGetIntegerv(GL_MAJOR_VERSION, &major);
            glGetIntegerv(GL_MINOR_VERSION, &minor);
            outInfo->majorVersion = major;
            outInfo->minorVersion = minor;

            // Check for core profile (GL 3.2+)
            if (major > 3 || (major == 3 && minor >= 2)) {
                GLint profileMask = 0;
                glGetIntegerv(GL_CONTEXT_PROFILE_MASK, &profileMask);
                outInfo->isCoreProfile = (profileMask & GL_CONTEXT_CORE_PROFILE_BIT) != 0;
            }

            return true;
        }
        return false;
    } catch (...) {
        return false;
    }
#else
    // Standalone mode: no GL context info available
    return false;
#endif
}

UsdResult Garch_GetGLVersionString(char** outVersion) {
    if (!outVersion) return USD_RESULT_INVALID_ARGUMENT;

#if GARCH_USE_USD
    try {
        if (glGetString) {
            const GLubyte* version = glGetString(GL_VERSION);
            if (version) {
                *outVersion = DuplicateString(reinterpret_cast<const char*>(version));
                return *outVersion ? USD_RESULT_SUCCESS : USD_RESULT_OUT_OF_MEMORY;
            }
        }
        *outVersion = nullptr;
        return USD_RESULT_NOT_FOUND;
    } catch (...) {
        *outVersion = nullptr;
        return USD_RESULT_ERROR;
    }
#else
    *outVersion = DuplicateString("OpenGL (Standalone Mode)");
    return *outVersion ? USD_RESULT_SUCCESS : USD_RESULT_OUT_OF_MEMORY;
#endif
}

UsdResult Garch_GetGLRendererString(char** outRenderer) {
    if (!outRenderer) return USD_RESULT_INVALID_ARGUMENT;

#if GARCH_USE_USD
    try {
        if (glGetString) {
            const GLubyte* renderer = glGetString(GL_RENDERER);
            if (renderer) {
                *outRenderer = DuplicateString(reinterpret_cast<const char*>(renderer));
                return *outRenderer ? USD_RESULT_SUCCESS : USD_RESULT_OUT_OF_MEMORY;
            }
        }
        *outRenderer = nullptr;
        return USD_RESULT_NOT_FOUND;
    } catch (...) {
        *outRenderer = nullptr;
        return USD_RESULT_ERROR;
    }
#else
    *outRenderer = DuplicateString("SwiftUSD Standalone Renderer");
    return *outRenderer ? USD_RESULT_SUCCESS : USD_RESULT_OUT_OF_MEMORY;
#endif
}

UsdResult Garch_GetGLVendorString(char** outVendor) {
    if (!outVendor) return USD_RESULT_INVALID_ARGUMENT;

#if GARCH_USE_USD
    try {
        if (glGetString) {
            const GLubyte* vendor = glGetString(GL_VENDOR);
            if (vendor) {
                *outVendor = DuplicateString(reinterpret_cast<const char*>(vendor));
                return *outVendor ? USD_RESULT_SUCCESS : USD_RESULT_OUT_OF_MEMORY;
            }
        }
        *outVendor = nullptr;
        return USD_RESULT_NOT_FOUND;
    } catch (...) {
        *outVendor = nullptr;
        return USD_RESULT_ERROR;
    }
#else
    *outVendor = DuplicateString("SwiftUSD");
    return *outVendor ? USD_RESULT_SUCCESS : USD_RESULT_OUT_OF_MEMORY;
#endif
}

UsdResult Garch_GetGLSLVersionString(char** outVersion) {
    if (!outVersion) return USD_RESULT_INVALID_ARGUMENT;

#if GARCH_USE_USD
    try {
        if (glGetString) {
            const GLubyte* version = glGetString(GL_SHADING_LANGUAGE_VERSION);
            if (version) {
                *outVersion = DuplicateString(reinterpret_cast<const char*>(version));
                return *outVersion ? USD_RESULT_SUCCESS : USD_RESULT_OUT_OF_MEMORY;
            }
        }
        *outVersion = nullptr;
        return USD_RESULT_NOT_FOUND;
    } catch (...) {
        *outVersion = nullptr;
        return USD_RESULT_ERROR;
    }
#else
    *outVersion = DuplicateString("GLSL (Standalone Mode)");
    return *outVersion ? USD_RESULT_SUCCESS : USD_RESULT_OUT_OF_MEMORY;
#endif
}

void Garch_FreeString(char* str) {
    free(str);
}

// ============================================================================
// MARK: - GL Extension Checking
// ============================================================================

// Static storage for extension strings (populated on first call)
static std::vector<std::string> g_extensionStrings;
static std::vector<const char*> g_extensionPointers;
static bool g_extensionsInitialized = false;

static void InitializeExtensions() {
    if (g_extensionsInitialized) return;
    g_extensionsInitialized = true;

#if GARCH_USE_USD
    try {
        if (glGetIntegerv && glGetStringi) {
            GLint numExtensions = 0;
            glGetIntegerv(GL_NUM_EXTENSIONS, &numExtensions);
            g_extensionStrings.reserve(numExtensions);
            g_extensionPointers.reserve(numExtensions);

            for (GLint i = 0; i < numExtensions; ++i) {
                const GLubyte* ext = glGetStringi(GL_EXTENSIONS, i);
                if (ext) {
                    g_extensionStrings.push_back(reinterpret_cast<const char*>(ext));
                    g_extensionPointers.push_back(g_extensionStrings.back().c_str());
                }
            }
        }
    } catch (...) {
        // Clear on error
        g_extensionStrings.clear();
        g_extensionPointers.clear();
    }
#else
    // Standalone mode: no extensions
#endif
}

size_t Garch_GetGLExtensionCount(void) {
    InitializeExtensions();
    return g_extensionPointers.size();
}

size_t Garch_GetGLExtensions(const char** outExtensions, size_t maxCount) {
    if (!outExtensions || maxCount == 0) return 0;

    InitializeExtensions();

    size_t count = std::min(maxCount, g_extensionPointers.size());
    for (size_t i = 0; i < count; ++i) {
        outExtensions[i] = g_extensionPointers[i];
    }
    return count;
}

bool Garch_HasGLExtension(const char* extensionName) {
    if (!extensionName) return false;

    InitializeExtensions();

    for (const auto& ext : g_extensionStrings) {
        if (ext == extensionName) {
            return true;
        }
    }
    return false;
}

} // extern "C"
