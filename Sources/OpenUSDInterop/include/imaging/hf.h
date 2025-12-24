// hf.h - Hf C wrapper
// Mirrors: pxr/imaging/hf/*.h (Hydra plugin foundation - plugin registry)

#ifndef OPENUSDINTEROP_IMAGING_HF_H
#define OPENUSDINTEROP_IMAGING_HF_H

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

/// Opaque handle for HfPluginDesc (plugin description)
typedef struct HfPluginDescOpaque* HfPluginDescRef;

/// Opaque handle for HfPluginDescVector (array of plugin descriptions)
typedef struct HfPluginDescVectorOpaque* HfPluginDescVectorRef;

// ============================================================================
// MARK: - Plugin Description Structure
// ============================================================================

/// C-compatible plugin description for interop.
/// Contains plugin id, display name, and priority.
typedef struct {
    const char* id;           ///< Plugin identifier token (do not free)
    const char* displayName;  ///< Human-readable display name (do not free)
    int priority;             ///< Plugin priority (higher = more preferred)
} HfPluginDescData;

// ============================================================================
// MARK: - Plugin Description Lifecycle
// ============================================================================

/// Creates an HfPluginDesc from components.
/// Returns NULL on failure. Caller must release with HfPluginDesc_Release.
USD_INTEROP_API HfPluginDescRef HfPluginDesc_Create(
    const char* id,
    const char* displayName,
    int priority
);

/// Retains a plugin description reference.
USD_INTEROP_API HfPluginDescRef HfPluginDesc_Retain(HfPluginDescRef desc);

/// Releases a plugin description reference.
USD_INTEROP_API void HfPluginDesc_Release(HfPluginDescRef desc);

// ============================================================================
// MARK: - Plugin Description Accessors
// ============================================================================

/// Gets the plugin id as a string.
/// Returns empty string on error. The returned string is valid as long as desc is valid.
USD_INTEROP_API const char* HfPluginDesc_GetId(HfPluginDescRef desc);

/// Gets the plugin display name.
/// Returns empty string on error. The returned string is valid as long as desc is valid.
USD_INTEROP_API const char* HfPluginDesc_GetDisplayName(HfPluginDescRef desc);

/// Gets the plugin priority.
/// Returns 0 on error.
USD_INTEROP_API int HfPluginDesc_GetPriority(HfPluginDescRef desc);

/// Gets all plugin description data in a single call.
/// Returns USD_RESULT_SUCCESS on success.
USD_INTEROP_API UsdResult HfPluginDesc_GetData(HfPluginDescRef desc, HfPluginDescData* outData);

// ============================================================================
// MARK: - Plugin Description Vector Lifecycle
// ============================================================================

/// Creates an empty plugin description vector.
/// Returns NULL on failure. Caller must release with HfPluginDescVector_Release.
USD_INTEROP_API HfPluginDescVectorRef HfPluginDescVector_Create(void);

/// Retains a plugin description vector reference.
USD_INTEROP_API HfPluginDescVectorRef HfPluginDescVector_Retain(HfPluginDescVectorRef vec);

/// Releases a plugin description vector reference.
USD_INTEROP_API void HfPluginDescVector_Release(HfPluginDescVectorRef vec);

// ============================================================================
// MARK: - Plugin Description Vector Operations
// ============================================================================

/// Gets the count of plugin descriptions in the vector.
USD_INTEROP_API size_t HfPluginDescVector_GetCount(HfPluginDescVectorRef vec);

/// Gets plugin description data at a specific index.
/// Returns USD_RESULT_SUCCESS on success.
USD_INTEROP_API UsdResult HfPluginDescVector_GetAt(
    HfPluginDescVectorRef vec,
    size_t index,
    HfPluginDescData* outData
);

/// Gets all plugin descriptions as an array of data.
/// outData must have space for at least maxCount items.
/// Returns the number of items written.
USD_INTEROP_API size_t HfPluginDescVector_GetAll(
    HfPluginDescVectorRef vec,
    HfPluginDescData* outData,
    size_t maxCount
);

/// Appends a plugin description to the vector.
USD_INTEROP_API UsdResult HfPluginDescVector_Append(
    HfPluginDescVectorRef vec,
    const char* id,
    const char* displayName,
    int priority
);

/// Clears all plugin descriptions from the vector.
USD_INTEROP_API void HfPluginDescVector_Clear(HfPluginDescVectorRef vec);

/// Reserves capacity for a given number of plugin descriptions.
USD_INTEROP_API void HfPluginDescVector_Reserve(HfPluginDescVectorRef vec, size_t capacity);

// ============================================================================
// MARK: - Plugin Registry Query Functions
// ============================================================================

/// Checks if a plugin is registered with the given id.
/// Note: This requires a concrete registry instance which is subclass-specific.
/// For now, we provide utility functions for common queries.
USD_INTEROP_API bool Hf_IsPluginRegistered(const char* registryType, const char* pluginId);

/// Gets the count of registered plugins for a given registry type.
/// Returns 0 if the registry type is not found or has no plugins.
USD_INTEROP_API size_t Hf_GetRegisteredPluginCount(const char* registryType);

/// Gets plugin descriptions for a given registry type.
/// Populates the provided vector with plugin descriptions.
/// Returns USD_RESULT_SUCCESS on success.
USD_INTEROP_API UsdResult Hf_GetRegisteredPlugins(
    const char* registryType,
    HfPluginDescVectorRef outPlugins
);

// ============================================================================
// MARK: - String Management
// ============================================================================

/// Frees a string allocated by Hf functions.
USD_INTEROP_API void Hf_FreeString(char* str);

/// Duplicates a string (caller must free with Hf_FreeString).
USD_INTEROP_API char* Hf_DuplicateString(const char* str);

#ifdef __cplusplus
}
#endif

#endif // OPENUSDINTEROP_IMAGING_HF_H
