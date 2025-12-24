// arch.h - Arch C wrapper
// Mirrors: pxr/base/arch/*.h (platform, timing, filesystem, environment)

#ifndef OPENUSDINTEROP_BASE_ARCH_H
#define OPENUSDINTEROP_BASE_ARCH_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

// Forward declarations - these are defined in OpenUSDInterop.h
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

// MARK: - Platform Information

/// Returns the system's memory page size. Safe to assume power-of-two.
USD_INTEROP_API int Arch_GetPageSize(void);

/// Returns true if the calling thread is the main thread.
USD_INTEROP_API bool Arch_IsMainThread(void);

// MARK: - System Information

/// Returns the current working directory.
/// Caller must free the returned string with Arch_FreeString.
USD_INTEROP_API UsdResult Arch_GetCwd(char** outPath);

/// Returns the path to the executable.
/// Caller must free the returned string with Arch_FreeString.
USD_INTEROP_API UsdResult Arch_GetExecutablePath(char** outPath);

/// Frees a string allocated by Arch functions.
USD_INTEROP_API void Arch_FreeString(char* str);

// MARK: - Timing

/// Returns the current time in system-dependent tick units.
USD_INTEROP_API uint64_t Arch_GetTickTime(void);

/// Returns a "start" tick time optimized for interval measurement.
USD_INTEROP_API uint64_t Arch_GetStartTickTime(void);

/// Returns a "stop" tick time optimized for interval measurement.
USD_INTEROP_API uint64_t Arch_GetStopTickTime(void);

/// Returns the tick time resolution (minimum tick increment).
USD_INTEROP_API uint64_t Arch_GetTickQuantum(void);

/// Returns the ticks taken to record an interval (measurement overhead).
USD_INTEROP_API uint64_t Arch_GetIntervalTimerTickOverhead(void);

/// Converts ticks to nanoseconds.
USD_INTEROP_API int64_t Arch_TicksToNanoseconds(uint64_t nTicks);

/// Converts ticks to seconds.
USD_INTEROP_API double Arch_TicksToSeconds(uint64_t nTicks);

/// Converts seconds to ticks.
USD_INTEROP_API uint64_t Arch_SecondsToTicks(double seconds);

/// Returns nanoseconds per tick.
USD_INTEROP_API double Arch_GetNanosecondsPerTick(void);

// MARK: - Environment Variables

/// Returns true if the environment variable exists.
USD_INTEROP_API bool Arch_HasEnv(const char* name);

/// Gets an environment variable value.
/// Caller must free the returned string with Arch_FreeString.
USD_INTEROP_API UsdResult Arch_GetEnv(const char* name, char** outValue);

/// Sets an environment variable.
USD_INTEROP_API UsdResult Arch_SetEnv(const char* name, const char* value, bool overwrite);

/// Removes an environment variable.
USD_INTEROP_API UsdResult Arch_RemoveEnv(const char* name);

/// Expands environment variables in a string (e.g., ${VAR} or $VAR).
/// Caller must free the returned string with Arch_FreeString.
USD_INTEROP_API UsdResult Arch_ExpandEnvironmentVariables(const char* str, char** outExpanded);

// MARK: - File System

/// Returns the path to a temporary directory.
/// The returned string is owned by the library and should not be freed.
USD_INTEROP_API const char* Arch_GetTmpDir(void);

/// Creates a temporary file name.
/// Caller must free the returned string with Arch_FreeString.
USD_INTEROP_API UsdResult Arch_MakeTmpFileName(const char* prefix, const char* suffix, char** outPath);

/// Creates a temporary file and returns its file descriptor.
/// If outPath is not NULL, the path is returned (caller must free with Arch_FreeString).
USD_INTEROP_API int Arch_MakeTmpFile(const char* prefix, char** outPath);

/// Creates a temporary subdirectory.
/// Caller must free the returned string with Arch_FreeString.
USD_INTEROP_API UsdResult Arch_MakeTmpSubdir(const char* tmpdir, const char* prefix, char** outPath);

/// Returns the length of a file in bytes, or -1 on error.
USD_INTEROP_API int64_t Arch_GetFileLength(const char* fileName);

/// Normalizes a path, eliminating double slashes and resolving . and ..
/// Caller must free the returned string with Arch_FreeString.
USD_INTEROP_API UsdResult Arch_NormPath(const char* path, bool stripDriveSpecifier, char** outPath);

/// Returns the absolute path.
/// Caller must free the returned string with Arch_FreeString.
USD_INTEROP_API UsdResult Arch_AbsPath(const char* path, char** outPath);

/// Returns the modification time of a file in seconds since epoch.
USD_INTEROP_API UsdResult Arch_GetModificationTime(const char* pathname, double* outTime);

// MARK: - Error Handling

/// Returns the error string for the current errno value.
/// Caller must free the returned string with Arch_FreeString.
USD_INTEROP_API UsdResult Arch_Strerror(char** outError);

/// Returns the error string for a specific errno value.
/// Caller must free the returned string with Arch_FreeString.
USD_INTEROP_API UsdResult Arch_StrerrorCode(int errorCode, char** outError);

// MARK: - Debugger

/// Returns true if a debugger is attached.
USD_INTEROP_API bool Arch_DebuggerIsAttached(void);

/// Triggers a debugger trap (breakpoint).
USD_INTEROP_API void Arch_DebuggerTrap(void);

/// Sets whether debug traps should wait for debugger attachment.
USD_INTEROP_API void Arch_DebuggerWait(bool wait);

#ifdef __cplusplus
}
#endif

#endif // OPENUSDINTEROP_BASE_ARCH_H
