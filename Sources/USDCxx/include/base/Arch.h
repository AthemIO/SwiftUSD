// Arch.h - Platform utilities for SwiftUSD
// Mirrors: pxr/base/arch/*.h (platform, timing, filesystem, environment)
// C++ header with Swift interop annotations

#pragma once

#include "../Swift.h"

#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <cerrno>
#include <cctype>
#include <string>
#include <vector>

#if defined(USE_PIXAR_USD)
#include "pxr/pxr.h"
#include "pxr/base/arch/systemInfo.h"
#include "pxr/base/arch/timing.h"
#include "pxr/base/arch/fileSystem.h"
#include "pxr/base/arch/env.h"
#include "pxr/base/arch/errno.h"
#include "pxr/base/arch/threads.h"
#include "pxr/base/arch/debugger.h"
#endif

// Platform-specific includes for standalone implementation
#if !defined(USE_PIXAR_USD)
#if defined(__APPLE__)
#include <unistd.h>
#include <sys/stat.h>
#include <sys/sysctl.h>
#include <climits>
#include <pthread.h>
#include <mach/mach_time.h>
#include <mach-o/dyld.h>
#elif defined(__linux__)
#include <unistd.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <climits>
#include <pthread.h>
#include <cstdio>
#elif defined(_WIN32)
#include <windows.h>
#include <direct.h>
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>
#endif
#endif

namespace swiftusd {

// ============================================================================
// MARK: - Platform Information
// ============================================================================

/// Returns the system memory page size.
inline int getPageSize() SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
    return PXR_NS::ArchGetPageSize();
#elif defined(__APPLE__) || defined(__linux__)
    return static_cast<int>(sysconf(_SC_PAGESIZE));
#elif defined(_WIN32)
    SYSTEM_INFO si;
    GetSystemInfo(&si);
    return static_cast<int>(si.dwPageSize);
#else
    return 4096; // Default fallback
#endif
}

/// Returns true if the calling thread is the main thread.
inline bool isMainThread() SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
    return PXR_NS::ArchIsMainThread();
#elif defined(__APPLE__)
    return pthread_main_np() != 0;
#elif defined(__linux__)
    return getpid() == static_cast<pid_t>(syscall(SYS_gettid));
#elif defined(_WIN32)
    // On Windows, compare with the main thread ID stored at startup
    static DWORD mainThreadId = GetCurrentThreadId();
    return GetCurrentThreadId() == mainThreadId;
#else
    return true;
#endif
}

// ============================================================================
// MARK: - System Information
// ============================================================================

/// Returns the current working directory as a C string.
/// Caller must free the returned pointer using free().
inline char* getCwd() SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
    std::string cwd = PXR_NS::ArchGetCwd();
    char* result = static_cast<char*>(std::malloc(cwd.size() + 1));
    if (result) {
        std::strcpy(result, cwd.c_str());
    }
    return result;
#elif defined(__APPLE__) || defined(__linux__)
    char buffer[PATH_MAX];
    if (getcwd(buffer, sizeof(buffer)) != nullptr) {
        char* result = static_cast<char*>(std::malloc(std::strlen(buffer) + 1));
        if (result) {
            std::strcpy(result, buffer);
        }
        return result;
    }
    return nullptr;
#elif defined(_WIN32)
    char buffer[MAX_PATH];
    if (_getcwd(buffer, sizeof(buffer)) != nullptr) {
        char* result = static_cast<char*>(std::malloc(std::strlen(buffer) + 1));
        if (result) {
            std::strcpy(result, buffer);
        }
        return result;
    }
    return nullptr;
#else
    return nullptr;
#endif
}

/// Returns the path to the executable.
/// Caller must free the returned pointer using free().
inline char* getExecutablePath() SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
    std::string path = PXR_NS::ArchGetExecutablePath();
    char* result = static_cast<char*>(std::malloc(path.size() + 1));
    if (result) {
        std::strcpy(result, path.c_str());
    }
    return result;
#elif defined(__APPLE__)
    char buffer[PATH_MAX];
    uint32_t size = sizeof(buffer);
    if (_NSGetExecutablePath(buffer, &size) == 0) {
        char resolved[PATH_MAX];
        if (realpath(buffer, resolved) != nullptr) {
            char* result = static_cast<char*>(std::malloc(std::strlen(resolved) + 1));
            if (result) {
                std::strcpy(result, resolved);
            }
            return result;
        }
    }
    return nullptr;
#elif defined(__linux__)
    char buffer[PATH_MAX];
    ssize_t len = readlink("/proc/self/exe", buffer, sizeof(buffer) - 1);
    if (len != -1) {
        buffer[len] = '\0';
        char* result = static_cast<char*>(std::malloc(static_cast<size_t>(len) + 1));
        if (result) {
            std::strcpy(result, buffer);
        }
        return result;
    }
    return nullptr;
#elif defined(_WIN32)
    char buffer[MAX_PATH];
    DWORD len = GetModuleFileNameA(nullptr, buffer, sizeof(buffer));
    if (len > 0 && len < sizeof(buffer)) {
        char* result = static_cast<char*>(std::malloc(len + 1));
        if (result) {
            std::strcpy(result, buffer);
        }
        return result;
    }
    return nullptr;
#else
    return nullptr;
#endif
}

// ============================================================================
// MARK: - Timing
// ============================================================================

/// Returns the current time in system-dependent tick units.
inline uint64_t getTickTime() SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
    return PXR_NS::ArchGetTickTime();
#elif defined(__APPLE__)
    return mach_absolute_time();
#elif defined(__x86_64__) || defined(_M_X64)
    unsigned int lo, hi;
    __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
    return (static_cast<uint64_t>(hi) << 32) | lo;
#elif defined(__aarch64__) || defined(_M_ARM64)
    uint64_t result;
    __asm__ __volatile__("mrs %0, CNTVCT_EL0" : "=r"(result));
    return result;
#else
    return 0;
#endif
}

/// Returns a "start" tick time optimized for interval measurement.
inline uint64_t getStartTickTime() SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
    return PXR_NS::ArchGetStartTickTime();
#else
    return getTickTime();
#endif
}

/// Returns a "stop" tick time optimized for interval measurement.
inline uint64_t getStopTickTime() SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
    return PXR_NS::ArchGetStopTickTime();
#else
    return getTickTime();
#endif
}

/// Returns the tick time resolution (minimum tick increment).
inline uint64_t getTickQuantum() SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
    return PXR_NS::ArchGetTickQuantum();
#else
    return 1;
#endif
}

/// Returns the measurement overhead in ticks.
inline uint64_t getIntervalTimerTickOverhead() SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
    return PXR_NS::ArchGetIntervalTimerTickOverhead();
#else
    return 0;
#endif
}

/// Returns nanoseconds per tick.
inline double getNanosecondsPerTick() SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
    return PXR_NS::ArchGetNanosecondsPerTick();
#elif defined(__APPLE__)
    static mach_timebase_info_data_t timebase = {0, 0};
    if (timebase.denom == 0) {
        mach_timebase_info(&timebase);
    }
    return static_cast<double>(timebase.numer) / static_cast<double>(timebase.denom);
#else
    // Assume ~1ns per tick for x86 TSC at ~1GHz
    return 1.0;
#endif
}

/// Converts ticks to nanoseconds.
inline int64_t ticksToNanoseconds(uint64_t ticks) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
    return PXR_NS::ArchTicksToNanoseconds(ticks);
#else
    return static_cast<int64_t>(static_cast<double>(ticks) * getNanosecondsPerTick());
#endif
}

/// Converts ticks to seconds.
inline double ticksToSeconds(uint64_t ticks) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
    return PXR_NS::ArchTicksToSeconds(ticks);
#else
    return static_cast<double>(ticksToNanoseconds(ticks)) / 1e9;
#endif
}

/// Converts seconds to ticks.
inline uint64_t secondsToTicks(double seconds) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
    return PXR_NS::ArchSecondsToTicks(seconds);
#else
    return static_cast<uint64_t>((seconds * 1e9) / getNanosecondsPerTick());
#endif
}

// ============================================================================
// MARK: - File System
// ============================================================================

/// Returns the path to a temporary directory.
/// Returns a static string, do NOT free.
inline const char* getTmpDir() SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
    return PXR_NS::ArchGetTmpDir();
#elif defined(__APPLE__)
    return "/tmp";
#elif defined(__linux__)
    return "/var/tmp";
#elif defined(_WIN32)
    static char tmpPath[MAX_PATH] = {0};
    if (tmpPath[0] == 0) {
        GetTempPathA(sizeof(tmpPath), tmpPath);
        // Remove trailing backslash
        size_t len = strlen(tmpPath);
        if (len > 0 && tmpPath[len-1] == '\\') {
            tmpPath[len-1] = '\0';
        }
    }
    return tmpPath;
#else
    return "/tmp";
#endif
}

/// Returns the length of a file in bytes. Returns -1 if file cannot be read.
inline int64_t getFileLength(const char* fileName) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
    return PXR_NS::ArchGetFileLength(fileName);
#elif defined(__APPLE__) || defined(__linux__)
    struct stat st;
    if (stat(fileName, &st) == 0) {
        return static_cast<int64_t>(st.st_size);
    }
    return -1;
#elif defined(_WIN32)
    struct _stat64 st;
    if (_stat64(fileName, &st) == 0) {
        return st.st_size;
    }
    return -1;
#else
    return -1;
#endif
}

/// Returns the modification time of a file in seconds since epoch.
/// Returns -1.0 on failure.
inline double getModificationTime(const char* pathname) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
    double time;
    if (PXR_NS::ArchGetModificationTime(pathname, &time)) {
        return time;
    }
    return -1.0;
#elif defined(__APPLE__) || defined(__linux__)
    struct stat st;
    if (stat(pathname, &st) == 0) {
        return static_cast<double>(st.st_mtime);
    }
    return -1.0;
#elif defined(_WIN32)
    struct _stat64 st;
    if (_stat64(pathname, &st) == 0) {
        return static_cast<double>(st.st_mtime);
    }
    return -1.0;
#else
    return -1.0;
#endif
}

/// Normalizes a path, eliminating double slashes and resolving . and ..
/// Caller must free the returned pointer using free().
inline char* normPath(const char* path, bool stripDriveSpecifier = false) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
    std::string normalized = PXR_NS::ArchNormPath(std::string(path), stripDriveSpecifier);
    char* result = static_cast<char*>(std::malloc(normalized.size() + 1));
    if (result) {
        std::strcpy(result, normalized.c_str());
    }
    return result;
#else
    // Simple implementation
    std::string result;
    std::string input(path);

    // Replace backslashes with forward slashes
    for (char& c : input) {
        if (c == '\\') c = '/';
    }

    // Handle drive specifier on Windows
    size_t start = 0;
#if defined(_WIN32)
    if (input.size() >= 2 && input[1] == ':') {
        if (!stripDriveSpecifier) {
            result += input.substr(0, 2);
        }
        start = 2;
    }
#else
    (void)stripDriveSpecifier; // Unused on non-Windows
#endif

    // Split path and process components
    std::vector<std::string> components;
    std::string current;
    bool isAbsolute = (start < input.size() && input[start] == '/');

    for (size_t i = start; i < input.size(); ++i) {
        if (input[i] == '/') {
            if (!current.empty()) {
                if (current == "..") {
                    if (!components.empty() && components.back() != "..") {
                        components.pop_back();
                    } else if (!isAbsolute) {
                        components.push_back(current);
                    }
                } else if (current != ".") {
                    components.push_back(current);
                }
                current.clear();
            }
        } else {
            current += input[i];
        }
    }

    // Handle last component
    if (!current.empty()) {
        if (current == "..") {
            if (!components.empty() && components.back() != "..") {
                components.pop_back();
            } else if (!isAbsolute) {
                components.push_back(current);
            }
        } else if (current != ".") {
            components.push_back(current);
        }
    }

    // Build result
    if (isAbsolute) {
        result += "/";
    }
    for (size_t i = 0; i < components.size(); ++i) {
        if (i > 0) result += "/";
        result += components[i];
    }

    if (result.empty()) {
        result = ".";
    }

    char* ret = static_cast<char*>(std::malloc(result.size() + 1));
    if (ret) {
        std::strcpy(ret, result.c_str());
    }
    return ret;
#endif
}

/// Returns the absolute path for a given path.
/// Caller must free the returned pointer using free().
inline char* absPath(const char* path) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
    std::string absolute = PXR_NS::ArchAbsPath(std::string(path));
    char* result = static_cast<char*>(std::malloc(absolute.size() + 1));
    if (result) {
        std::strcpy(result, absolute.c_str());
    }
    return result;
#elif defined(__APPLE__) || defined(__linux__)
    char resolved[PATH_MAX];
    if (realpath(path, resolved) != nullptr) {
        char* result = static_cast<char*>(std::malloc(std::strlen(resolved) + 1));
        if (result) {
            std::strcpy(result, resolved);
        }
        return result;
    }
    // If realpath fails (file doesn't exist), build absolute path manually
    char* cwd = getCwd();
    if (cwd && path[0] != '/') {
        size_t len = std::strlen(cwd) + 1 + std::strlen(path) + 1;
        char* result = static_cast<char*>(std::malloc(len));
        if (result) {
            std::strcpy(result, cwd);
            std::strcat(result, "/");
            std::strcat(result, path);
        }
        std::free(cwd);
        return result;
    }
    std::free(cwd);
    char* result = static_cast<char*>(std::malloc(std::strlen(path) + 1));
    if (result) {
        std::strcpy(result, path);
    }
    return result;
#elif defined(_WIN32)
    char resolved[MAX_PATH];
    if (_fullpath(resolved, path, sizeof(resolved)) != nullptr) {
        char* result = static_cast<char*>(std::malloc(std::strlen(resolved) + 1));
        if (result) {
            std::strcpy(result, resolved);
        }
        return result;
    }
    return nullptr;
#else
    return nullptr;
#endif
}

/// Creates a temporary file name.
/// Caller must free the returned pointer using free().
inline char* makeTmpFileName(const char* prefix, const char* suffix = nullptr) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
    std::string fileName = PXR_NS::ArchMakeTmpFileName(
        std::string(prefix),
        suffix ? std::string(suffix) : std::string()
    );
    char* result = static_cast<char*>(std::malloc(fileName.size() + 1));
    if (result) {
        std::strcpy(result, fileName.c_str());
    }
    return result;
#else
    static int counter = 0;
    const char* tmpDir = getTmpDir();
#if defined(__APPLE__) || defined(__linux__)
    int pid = static_cast<int>(getpid());
#elif defined(_WIN32)
    int pid = static_cast<int>(GetCurrentProcessId());
#else
    int pid = 0;
#endif

    std::string fileName = std::string(tmpDir) + "/" + prefix + "." +
                          std::to_string(pid);
    if (counter > 0) {
        fileName += "." + std::to_string(counter);
    }
    if (suffix && suffix[0]) {
        fileName += suffix;
    }
    ++counter;

    char* result = static_cast<char*>(std::malloc(fileName.size() + 1));
    if (result) {
        std::strcpy(result, fileName.c_str());
    }
    return result;
#endif
}

/// Creates a temporary file and returns the file descriptor.
/// If pathOut is not null, it receives the path (caller must free).
/// Returns -1 on failure.
inline int makeTmpFile(const char* prefix, char** pathOut) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
    std::string pathname;
    int fd = PXR_NS::ArchMakeTmpFile(std::string(prefix), pathOut ? &pathname : nullptr);
    if (pathOut && fd >= 0) {
        *pathOut = static_cast<char*>(std::malloc(pathname.size() + 1));
        if (*pathOut) {
            std::strcpy(*pathOut, pathname.c_str());
        }
    }
    return fd;
#elif defined(__APPLE__) || defined(__linux__)
    const char* tmpDir = getTmpDir();
    std::string templatePath = std::string(tmpDir) + "/" + prefix + ".XXXXXX";
    char* pathTemplate = static_cast<char*>(std::malloc(templatePath.size() + 1));
    if (!pathTemplate) return -1;
    std::strcpy(pathTemplate, templatePath.c_str());

    int fd = mkstemp(pathTemplate);
    if (fd >= 0 && pathOut) {
        *pathOut = pathTemplate;
    } else {
        if (pathOut) *pathOut = nullptr;
        if (fd < 0) {
            std::free(pathTemplate);
        }
    }
    if (fd >= 0 && !pathOut) {
        std::free(pathTemplate);
    }
    return fd;
#elif defined(_WIN32)
    char tmpPath[MAX_PATH];
    char tmpFile[MAX_PATH];
    GetTempPathA(sizeof(tmpPath), tmpPath);
    if (GetTempFileNameA(tmpPath, prefix, 0, tmpFile) == 0) {
        return -1;
    }
    int fd = _open(tmpFile, _O_CREAT | _O_RDWR | _O_BINARY, _S_IREAD | _S_IWRITE);
    if (fd >= 0 && pathOut) {
        *pathOut = static_cast<char*>(std::malloc(std::strlen(tmpFile) + 1));
        if (*pathOut) {
            std::strcpy(*pathOut, tmpFile);
        }
    }
    return fd;
#else
    (void)prefix;
    (void)pathOut;
    return -1;
#endif
}

/// Creates a temporary subdirectory.
/// Caller must free the returned pointer using free().
inline char* makeTmpSubdir(const char* tmpdir, const char* prefix) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
    std::string subdir = PXR_NS::ArchMakeTmpSubdir(std::string(tmpdir), std::string(prefix));
    if (subdir.empty()) return nullptr;
    char* result = static_cast<char*>(std::malloc(subdir.size() + 1));
    if (result) {
        std::strcpy(result, subdir.c_str());
    }
    return result;
#elif defined(__APPLE__) || defined(__linux__)
    std::string templatePath = std::string(tmpdir) + "/" + prefix + ".XXXXXX";
    char* pathTemplate = static_cast<char*>(std::malloc(templatePath.size() + 1));
    if (!pathTemplate) return nullptr;
    std::strcpy(pathTemplate, templatePath.c_str());

    char* result = mkdtemp(pathTemplate);
    if (!result) {
        std::free(pathTemplate);
        return nullptr;
    }
    return pathTemplate;
#elif defined(_WIN32)
    // Windows doesn't have mkdtemp, so we generate a unique name
    char tmpFile[MAX_PATH];
    if (GetTempFileNameA(tmpdir, prefix, 0, tmpFile) == 0) {
        return nullptr;
    }
    // Delete the file and create a directory instead
    DeleteFileA(tmpFile);
    if (CreateDirectoryA(tmpFile, nullptr) == 0) {
        return nullptr;
    }
    char* result = static_cast<char*>(std::malloc(std::strlen(tmpFile) + 1));
    if (result) {
        std::strcpy(result, tmpFile);
    }
    return result;
#else
    (void)tmpdir;
    (void)prefix;
    return nullptr;
#endif
}

// ============================================================================
// MARK: - Environment
// ============================================================================

/// Returns true if the environment variable exists.
inline bool hasEnv(const char* name) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
    return PXR_NS::ArchHasEnv(std::string(name));
#else
    return std::getenv(name) != nullptr;
#endif
}

/// Gets the value of an environment variable.
/// Caller must free the returned pointer using free().
/// Returns nullptr if not set.
inline char* getEnv(const char* name) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
    std::string value = PXR_NS::ArchGetEnv(std::string(name));
    if (value.empty() && !PXR_NS::ArchHasEnv(std::string(name))) {
        return nullptr;
    }
    char* result = static_cast<char*>(std::malloc(value.size() + 1));
    if (result) {
        std::strcpy(result, value.c_str());
    }
    return result;
#else
    const char* value = std::getenv(name);
    if (!value) return nullptr;
    char* result = static_cast<char*>(std::malloc(std::strlen(value) + 1));
    if (result) {
        std::strcpy(result, value);
    }
    return result;
#endif
}

/// Sets an environment variable.
inline bool setEnv(const char* name, const char* value, bool overwrite) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
    return PXR_NS::ArchSetEnv(std::string(name), std::string(value), overwrite);
#elif defined(__APPLE__) || defined(__linux__)
    return setenv(name, value, overwrite ? 1 : 0) == 0;
#elif defined(_WIN32)
    if (!overwrite && std::getenv(name) != nullptr) {
        return true;
    }
    std::string envStr = std::string(name) + "=" + value;
    return _putenv(envStr.c_str()) == 0;
#else
    (void)name; (void)value; (void)overwrite;
    return false;
#endif
}

/// Removes an environment variable.
inline bool removeEnv(const char* name) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
    return PXR_NS::ArchRemoveEnv(std::string(name));
#elif defined(__APPLE__) || defined(__linux__)
    return unsetenv(name) == 0;
#elif defined(_WIN32)
    std::string envStr = std::string(name) + "=";
    return _putenv(envStr.c_str()) == 0;
#else
    (void)name;
    return false;
#endif
}

/// Expands environment variables in a string.
/// Caller must free the returned pointer using free().
inline char* expandEnvironmentVariables(const char* str) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
    std::string expanded = PXR_NS::ArchExpandEnvironmentVariables(std::string(str));
    char* result = static_cast<char*>(std::malloc(expanded.size() + 1));
    if (result) {
        std::strcpy(result, expanded.c_str());
    }
    return result;
#else
    std::string input(str);
    std::string result;
    size_t i = 0;

    while (i < input.size()) {
        if (input[i] == '$') {
            if (i + 1 < input.size() && input[i + 1] == '{') {
                // ${VAR} form
                size_t end = input.find('}', i + 2);
                if (end != std::string::npos) {
                    std::string varName = input.substr(i + 2, end - i - 2);
                    const char* value = std::getenv(varName.c_str());
                    if (value) {
                        result += value;
                    }
                    i = end + 1;
                    continue;
                }
            } else if (i + 1 < input.size() && (std::isalnum(static_cast<unsigned char>(input[i + 1])) || input[i + 1] == '_')) {
                // $VAR form
                size_t start = i + 1;
                size_t end = start;
                while (end < input.size() && (std::isalnum(static_cast<unsigned char>(input[end])) || input[end] == '_')) {
                    ++end;
                }
                std::string varName = input.substr(start, end - start);
                const char* value = std::getenv(varName.c_str());
                if (value) {
                    result += value;
                }
                i = end;
                continue;
            }
        }
        result += input[i];
        ++i;
    }

    char* ret = static_cast<char*>(std::malloc(result.size() + 1));
    if (ret) {
        std::strcpy(ret, result.c_str());
    }
    return ret;
#endif
}

// ============================================================================
// MARK: - Error Handling
// ============================================================================

/// Returns the error string for the current errno.
/// Caller must free the returned pointer using free().
inline char* strerror() SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
    std::string errStr = PXR_NS::ArchStrerror();
    char* result = static_cast<char*>(std::malloc(errStr.size() + 1));
    if (result) {
        std::strcpy(result, errStr.c_str());
    }
    return result;
#else
    const char* errStr = std::strerror(errno);
    if (!errStr) errStr = "Unknown error";
    char* result = static_cast<char*>(std::malloc(std::strlen(errStr) + 1));
    if (result) {
        std::strcpy(result, errStr);
    }
    return result;
#endif
}

/// Returns the error string for a specific error code.
/// Caller must free the returned pointer using free().
inline char* strerror(int errorCode) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
    std::string errStr = PXR_NS::ArchStrerror(errorCode);
    char* result = static_cast<char*>(std::malloc(errStr.size() + 1));
    if (result) {
        std::strcpy(result, errStr.c_str());
    }
    return result;
#else
    const char* errStr = std::strerror(errorCode);
    if (!errStr) errStr = "Unknown error";
    char* result = static_cast<char*>(std::malloc(std::strlen(errStr) + 1));
    if (result) {
        std::strcpy(result, errStr);
    }
    return result;
#endif
}

// ============================================================================
// MARK: - Debugger
// ============================================================================

/// Returns true if a debugger is attached to the process.
inline bool debuggerIsAttached() SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
    return PXR_NS::ArchDebuggerIsAttached();
#elif defined(__APPLE__)
    // Check if being traced
    int mib[4] = { CTL_KERN, KERN_PROC, KERN_PROC_PID, getpid() };
    struct kinfo_proc info;
    size_t size = sizeof(info);
    info.kp_proc.p_flag = 0;
    sysctl(mib, 4, &info, &size, nullptr, 0);
    return (info.kp_proc.p_flag & P_TRACED) != 0;
#elif defined(__linux__)
    // Check TracerPid in /proc/self/status
    char buffer[1024];
    FILE* f = std::fopen("/proc/self/status", "r");
    if (!f) return false;
    bool attached = false;
    while (std::fgets(buffer, sizeof(buffer), f)) {
        if (std::strncmp(buffer, "TracerPid:", 10) == 0) {
            int pid = std::atoi(buffer + 10);
            attached = (pid > 0);
            break;
        }
    }
    std::fclose(f);
    return attached;
#elif defined(_WIN32)
    return IsDebuggerPresent() != 0;
#else
    return false;
#endif
}

/// Triggers a debugger trap (breakpoint).
inline void debuggerTrap() SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
    PXR_NS::ArchDebuggerTrap();
#elif defined(__APPLE__) || defined(__linux__)
    __builtin_debugtrap();
#elif defined(_WIN32)
    __debugbreak();
#endif
}

/// Sets whether debug traps should wait for debugger attachment.
inline void debuggerWait(bool wait) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
    PXR_NS::ArchDebuggerWait(wait);
#else
    // No-op in standalone mode
    (void)wait;
#endif
}

// ============================================================================
// MARK: - Memory Management Helper
// ============================================================================

/// Frees a string allocated by the Arch functions.
inline void freeString(char* str) SWIFTUSD_NOEXCEPT {
    std::free(str);
}

} // namespace swiftusd
