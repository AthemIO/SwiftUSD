// arch.cpp - Arch C wrapper implementation
// Mirrors: pxr/base/arch/*.h (platform, timing, filesystem, environment)

#include "../../include/base/arch.h"

// Check if we should use full USD library
// Define USE_PIXAR_USD=1 when linking against the full USD library
#if defined(USE_PIXAR_USD) && USE_PIXAR_USD
    #define ARCH_USE_USD 1
    #include <pxr/base/arch/systemInfo.h>
    #include <pxr/base/arch/timing.h>
    #include <pxr/base/arch/env.h>
    #include <pxr/base/arch/fileSystem.h>
    #include <pxr/base/arch/errno.h>
    #include <pxr/base/arch/threads.h>
    #include <pxr/base/arch/debugger.h>
    PXR_NAMESPACE_USING_DIRECTIVE
#else
    #define ARCH_USE_USD 0
#endif

#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cerrno>
#include <string>
#include <vector>

#if defined(__APPLE__) || defined(__linux__)
#include <unistd.h>
#include <sys/stat.h>
#include <pthread.h>
#include <limits.h>
#endif

#ifdef __APPLE__
#include <mach/mach_time.h>
#include <mach-o/dyld.h>
#include <sys/sysctl.h>
#include <sys/types.h>
#endif

#ifdef __linux__
#include <sys/syscall.h>
#include <time.h>
#endif

// Helper to duplicate a std::string as a C string
static char* DuplicateString(const std::string& str) {
    char* result = static_cast<char*>(malloc(str.size() + 1));
    if (result) {
        memcpy(result, str.c_str(), str.size() + 1);
    }
    return result;
}

extern "C" {

// MARK: - Platform Information

int Arch_GetPageSize(void) {
#if ARCH_USE_USD
    try {
        return ArchGetPageSize();
    } catch (...) {
        return 4096;
    }
#else
    #if defined(__APPLE__) || defined(__linux__)
    return static_cast<int>(sysconf(_SC_PAGESIZE));
    #else
    return 4096;
    #endif
#endif
}

bool Arch_IsMainThread(void) {
#if ARCH_USE_USD
    try {
        return ArchIsMainThread();
    } catch (...) {
        return true;
    }
#else
    #if defined(__APPLE__)
    return pthread_main_np() != 0;
    #elif defined(__linux__)
    return getpid() == static_cast<int>(syscall(SYS_gettid));
    #else
    return true;
    #endif
#endif
}

// MARK: - System Information

UsdResult Arch_GetCwd(char** outPath) {
    if (!outPath) return USD_RESULT_INVALID_ARGUMENT;

#if ARCH_USE_USD
    try {
        std::string cwd = ArchGetCwd();
        if (!cwd.empty()) {
            *outPath = DuplicateString(cwd);
            return *outPath ? USD_RESULT_SUCCESS : USD_RESULT_OUT_OF_MEMORY;
        }
    } catch (...) {
        // Fall through to error
    }
    *outPath = nullptr;
    return USD_RESULT_ERROR;
#else
    #if defined(__APPLE__) || defined(__linux__)
    char buffer[PATH_MAX];
    if (getcwd(buffer, sizeof(buffer)) != nullptr) {
        *outPath = DuplicateString(buffer);
        return *outPath ? USD_RESULT_SUCCESS : USD_RESULT_OUT_OF_MEMORY;
    }
    #endif
    *outPath = nullptr;
    return USD_RESULT_ERROR;
#endif
}

UsdResult Arch_GetExecutablePath(char** outPath) {
    if (!outPath) return USD_RESULT_INVALID_ARGUMENT;

#if ARCH_USE_USD
    try {
        std::string exePath = ArchGetExecutablePath();
        if (!exePath.empty()) {
            *outPath = DuplicateString(exePath);
            return *outPath ? USD_RESULT_SUCCESS : USD_RESULT_OUT_OF_MEMORY;
        }
    } catch (...) {
        // Fall through to error
    }
    *outPath = nullptr;
    return USD_RESULT_ERROR;
#else
    #if defined(__APPLE__)
    char buffer[PATH_MAX];
    uint32_t size = sizeof(buffer);
    if (_NSGetExecutablePath(buffer, &size) == 0) {
        *outPath = DuplicateString(buffer);
        return *outPath ? USD_RESULT_SUCCESS : USD_RESULT_OUT_OF_MEMORY;
    }
    #elif defined(__linux__)
    char buffer[PATH_MAX];
    ssize_t len = readlink("/proc/self/exe", buffer, sizeof(buffer) - 1);
    if (len != -1) {
        buffer[len] = '\0';
        *outPath = DuplicateString(buffer);
        return *outPath ? USD_RESULT_SUCCESS : USD_RESULT_OUT_OF_MEMORY;
    }
    #endif
    *outPath = nullptr;
    return USD_RESULT_ERROR;
#endif
}

void Arch_FreeString(char* str) {
    free(str);
}

// MARK: - Timing

uint64_t Arch_GetTickTime(void) {
#if ARCH_USE_USD
    try {
        return ArchGetTickTime();
    } catch (...) {
        return 0;
    }
#else
    #if defined(__APPLE__)
    return mach_absolute_time();
    #elif defined(__linux__)
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return static_cast<uint64_t>(ts.tv_sec) * 1000000000ULL + static_cast<uint64_t>(ts.tv_nsec);
    #else
    return 0;
    #endif
#endif
}

uint64_t Arch_GetStartTickTime(void) {
#if ARCH_USE_USD
    try {
        return ArchGetStartTickTime();
    } catch (...) {
        return Arch_GetTickTime();
    }
#else
    return Arch_GetTickTime();
#endif
}

uint64_t Arch_GetStopTickTime(void) {
#if ARCH_USE_USD
    try {
        return ArchGetStopTickTime();
    } catch (...) {
        return Arch_GetTickTime();
    }
#else
    return Arch_GetTickTime();
#endif
}

uint64_t Arch_GetTickQuantum(void) {
#if ARCH_USE_USD
    try {
        return ArchGetTickQuantum();
    } catch (...) {
        return 1;
    }
#else
    return 1;
#endif
}

uint64_t Arch_GetIntervalTimerTickOverhead(void) {
#if ARCH_USE_USD
    try {
        return ArchGetIntervalTimerTickOverhead();
    } catch (...) {
        return 0;
    }
#else
    return 0;
#endif
}

int64_t Arch_TicksToNanoseconds(uint64_t nTicks) {
#if ARCH_USE_USD
    try {
        return ArchTicksToNanoseconds(nTicks);
    } catch (...) {
        return static_cast<int64_t>(nTicks);
    }
#else
    #if defined(__APPLE__)
    static mach_timebase_info_data_t timebaseInfo = {0, 0};
    if (timebaseInfo.denom == 0) {
        mach_timebase_info(&timebaseInfo);
    }
    return static_cast<int64_t>(nTicks * timebaseInfo.numer / timebaseInfo.denom);
    #else
    return static_cast<int64_t>(nTicks);
    #endif
#endif
}

double Arch_TicksToSeconds(uint64_t nTicks) {
#if ARCH_USE_USD
    try {
        return ArchTicksToSeconds(nTicks);
    } catch (...) {
        return static_cast<double>(nTicks) / 1e9;
    }
#else
    return static_cast<double>(Arch_TicksToNanoseconds(nTicks)) / 1e9;
#endif
}

uint64_t Arch_SecondsToTicks(double seconds) {
#if ARCH_USE_USD
    try {
        return ArchSecondsToTicks(seconds);
    } catch (...) {
        return static_cast<uint64_t>(seconds * 1e9);
    }
#else
    #if defined(__APPLE__)
    static mach_timebase_info_data_t timebaseInfo = {0, 0};
    if (timebaseInfo.denom == 0) {
        mach_timebase_info(&timebaseInfo);
    }
    double nanos = seconds * 1e9;
    return static_cast<uint64_t>(nanos * timebaseInfo.denom / timebaseInfo.numer);
    #else
    return static_cast<uint64_t>(seconds * 1e9);
    #endif
#endif
}

double Arch_GetNanosecondsPerTick(void) {
#if ARCH_USE_USD
    try {
        return ArchGetNanosecondsPerTick();
    } catch (...) {
        return 1.0;
    }
#else
    #if defined(__APPLE__)
    static mach_timebase_info_data_t timebaseInfo = {0, 0};
    if (timebaseInfo.denom == 0) {
        mach_timebase_info(&timebaseInfo);
    }
    return static_cast<double>(timebaseInfo.numer) / static_cast<double>(timebaseInfo.denom);
    #else
    return 1.0;
    #endif
#endif
}

// MARK: - Environment Variables

bool Arch_HasEnv(const char* name) {
    if (!name) return false;
#if ARCH_USE_USD
    try {
        return ArchHasEnv(std::string(name));
    } catch (...) {
        return false;
    }
#else
    return getenv(name) != nullptr;
#endif
}

UsdResult Arch_GetEnv(const char* name, char** outValue) {
    if (!name || !outValue) return USD_RESULT_INVALID_ARGUMENT;

#if ARCH_USE_USD
    try {
        std::string value = ArchGetEnv(std::string(name));
        if (!value.empty() || ArchHasEnv(std::string(name))) {
            *outValue = DuplicateString(value);
            return *outValue ? USD_RESULT_SUCCESS : USD_RESULT_OUT_OF_MEMORY;
        }
    } catch (...) {
        // Fall through
    }
    *outValue = nullptr;
    return USD_RESULT_NOT_FOUND;
#else
    const char* value = getenv(name);
    if (value) {
        *outValue = DuplicateString(value);
        return *outValue ? USD_RESULT_SUCCESS : USD_RESULT_OUT_OF_MEMORY;
    }
    *outValue = nullptr;
    return USD_RESULT_NOT_FOUND;
#endif
}

UsdResult Arch_SetEnv(const char* name, const char* value, bool overwrite) {
    if (!name || !value) return USD_RESULT_INVALID_ARGUMENT;

#if ARCH_USE_USD
    try {
        bool success = ArchSetEnv(std::string(name), std::string(value), overwrite);
        return success ? USD_RESULT_SUCCESS : USD_RESULT_ERROR;
    } catch (...) {
        return USD_RESULT_ERROR;
    }
#else
    #if defined(__APPLE__) || defined(__linux__)
    int result = setenv(name, value, overwrite ? 1 : 0);
    return result == 0 ? USD_RESULT_SUCCESS : USD_RESULT_ERROR;
    #else
    return USD_RESULT_ERROR;
    #endif
#endif
}

UsdResult Arch_RemoveEnv(const char* name) {
    if (!name) return USD_RESULT_INVALID_ARGUMENT;

#if ARCH_USE_USD
    try {
        bool success = ArchRemoveEnv(std::string(name));
        return success ? USD_RESULT_SUCCESS : USD_RESULT_ERROR;
    } catch (...) {
        return USD_RESULT_ERROR;
    }
#else
    #if defined(__APPLE__) || defined(__linux__)
    int result = unsetenv(name);
    return result == 0 ? USD_RESULT_SUCCESS : USD_RESULT_ERROR;
    #else
    return USD_RESULT_ERROR;
    #endif
#endif
}

UsdResult Arch_ExpandEnvironmentVariables(const char* str, char** outExpanded) {
    if (!str || !outExpanded) return USD_RESULT_INVALID_ARGUMENT;

#if ARCH_USE_USD
    try {
        std::string expanded = ArchExpandEnvironmentVariables(std::string(str));
        *outExpanded = DuplicateString(expanded);
        return *outExpanded ? USD_RESULT_SUCCESS : USD_RESULT_OUT_OF_MEMORY;
    } catch (...) {
        *outExpanded = nullptr;
        return USD_RESULT_ERROR;
    }
#else
    // Simple implementation: replace ${VAR} and $VAR with env values
    std::string input(str);
    std::string result;
    size_t i = 0;

    while (i < input.size()) {
        if (input[i] == '$') {
            size_t start = i + 1;
            bool braced = false;

            if (start < input.size() && input[start] == '{') {
                braced = true;
                start++;
            }

            size_t end = start;
            while (end < input.size() &&
                   (isalnum(input[end]) || input[end] == '_')) {
                end++;
            }

            if (end > start) {
                std::string varName = input.substr(start, end - start);
                const char* value = getenv(varName.c_str());
                if (value) {
                    result += value;
                }
                i = braced && end < input.size() && input[end] == '}' ? end + 1 : end;
            } else {
                result += input[i];
                i++;
            }
        } else {
            result += input[i];
            i++;
        }
    }

    *outExpanded = DuplicateString(result);
    return *outExpanded ? USD_RESULT_SUCCESS : USD_RESULT_OUT_OF_MEMORY;
#endif
}

// MARK: - File System

const char* Arch_GetTmpDir(void) {
#if ARCH_USE_USD
    try {
        return ArchGetTmpDir();
    } catch (...) {
        return "/tmp";
    }
#else
    #if defined(__APPLE__)
    return "/tmp";
    #elif defined(__linux__)
    return "/var/tmp";
    #else
    return "/tmp";
    #endif
#endif
}

UsdResult Arch_MakeTmpFileName(const char* prefix, const char* suffix, char** outPath) {
    if (!prefix || !outPath) return USD_RESULT_INVALID_ARGUMENT;

#if ARCH_USE_USD
    try {
        std::string fileName = ArchMakeTmpFileName(
            std::string(prefix),
            suffix ? std::string(suffix) : std::string()
        );
        *outPath = DuplicateString(fileName);
        return *outPath ? USD_RESULT_SUCCESS : USD_RESULT_OUT_OF_MEMORY;
    } catch (...) {
        *outPath = nullptr;
        return USD_RESULT_ERROR;
    }
#else
    static int counter = 0;
    std::string path = Arch_GetTmpDir();
    path += "/";
    path += prefix;
    path += ".";
    path += std::to_string(getpid());
    if (counter > 0) {
        path += ".";
        path += std::to_string(counter);
    }
    counter++;
    if (suffix) {
        path += suffix;
    }
    *outPath = DuplicateString(path);
    return *outPath ? USD_RESULT_SUCCESS : USD_RESULT_OUT_OF_MEMORY;
#endif
}

int Arch_MakeTmpFile(const char* prefix, char** outPath) {
    if (!prefix) return -1;

#if ARCH_USE_USD
    try {
        std::string pathname;
        int fd = ArchMakeTmpFile(std::string(prefix), outPath ? &pathname : nullptr);
        if (fd >= 0 && outPath) {
            *outPath = DuplicateString(pathname);
        } else if (outPath) {
            *outPath = nullptr;
        }
        return fd;
    } catch (...) {
        if (outPath) *outPath = nullptr;
        return -1;
    }
#else
    std::string pathTemplate = Arch_GetTmpDir();
    pathTemplate += "/";
    pathTemplate += prefix;
    pathTemplate += ".XXXXXX";

    char* pathBuffer = static_cast<char*>(malloc(pathTemplate.size() + 1));
    if (!pathBuffer) return -1;
    strcpy(pathBuffer, pathTemplate.c_str());

    int fd = mkstemp(pathBuffer);
    if (fd >= 0 && outPath) {
        *outPath = pathBuffer;
    } else {
        free(pathBuffer);
        if (outPath) *outPath = nullptr;
    }
    return fd;
#endif
}

UsdResult Arch_MakeTmpSubdir(const char* tmpdir, const char* prefix, char** outPath) {
    if (!tmpdir || !prefix || !outPath) return USD_RESULT_INVALID_ARGUMENT;

#if ARCH_USE_USD
    try {
        std::string subdir = ArchMakeTmpSubdir(std::string(tmpdir), std::string(prefix));
        if (!subdir.empty()) {
            *outPath = DuplicateString(subdir);
            return *outPath ? USD_RESULT_SUCCESS : USD_RESULT_OUT_OF_MEMORY;
        }
    } catch (...) {
        // Fall through to error
    }
    *outPath = nullptr;
    return USD_RESULT_ERROR;
#else
    std::string pathTemplate = tmpdir;
    pathTemplate += "/";
    pathTemplate += prefix;
    pathTemplate += ".XXXXXX";

    char* pathBuffer = static_cast<char*>(malloc(pathTemplate.size() + 1));
    if (!pathBuffer) {
        *outPath = nullptr;
        return USD_RESULT_OUT_OF_MEMORY;
    }
    strcpy(pathBuffer, pathTemplate.c_str());

    char* result = mkdtemp(pathBuffer);
    if (result) {
        *outPath = pathBuffer;
        return USD_RESULT_SUCCESS;
    }

    free(pathBuffer);
    *outPath = nullptr;
    return USD_RESULT_ERROR;
#endif
}

int64_t Arch_GetFileLength(const char* fileName) {
    if (!fileName) return -1;

#if ARCH_USE_USD
    try {
        return ArchGetFileLength(fileName);
    } catch (...) {
        return -1;
    }
#else
    struct stat st;
    if (stat(fileName, &st) == 0) {
        return static_cast<int64_t>(st.st_size);
    }
    return -1;
#endif
}

UsdResult Arch_NormPath(const char* path, bool stripDriveSpecifier, char** outPath) {
    if (!path || !outPath) return USD_RESULT_INVALID_ARGUMENT;

#if ARCH_USE_USD
    try {
        std::string normalized = ArchNormPath(std::string(path), stripDriveSpecifier);
        *outPath = DuplicateString(normalized);
        return *outPath ? USD_RESULT_SUCCESS : USD_RESULT_OUT_OF_MEMORY;
    } catch (...) {
        *outPath = nullptr;
        return USD_RESULT_ERROR;
    }
#else
    (void)stripDriveSpecifier; // Unused on Unix

    // Simple normalization: resolve . and .. components
    std::string input(path);
    std::vector<std::string> components;
    bool isAbsolute = !input.empty() && input[0] == '/';

    std::string component;
    for (size_t i = 0; i < input.size(); i++) {
        if (input[i] == '/') {
            if (!component.empty()) {
                if (component == "..") {
                    if (!components.empty() && components.back() != "..") {
                        components.pop_back();
                    } else if (!isAbsolute) {
                        components.push_back(component);
                    }
                } else if (component != ".") {
                    components.push_back(component);
                }
                component.clear();
            }
        } else {
            component += input[i];
        }
    }
    if (!component.empty()) {
        if (component == "..") {
            if (!components.empty() && components.back() != "..") {
                components.pop_back();
            } else if (!isAbsolute) {
                components.push_back(component);
            }
        } else if (component != ".") {
            components.push_back(component);
        }
    }

    std::string result;
    if (isAbsolute) result = "/";
    for (size_t i = 0; i < components.size(); i++) {
        if (i > 0) result += "/";
        result += components[i];
    }
    if (result.empty()) result = ".";

    *outPath = DuplicateString(result);
    return *outPath ? USD_RESULT_SUCCESS : USD_RESULT_OUT_OF_MEMORY;
#endif
}

UsdResult Arch_AbsPath(const char* path, char** outPath) {
    if (!path || !outPath) return USD_RESULT_INVALID_ARGUMENT;

#if ARCH_USE_USD
    try {
        std::string absPath = ArchAbsPath(std::string(path));
        *outPath = DuplicateString(absPath);
        return *outPath ? USD_RESULT_SUCCESS : USD_RESULT_OUT_OF_MEMORY;
    } catch (...) {
        *outPath = nullptr;
        return USD_RESULT_ERROR;
    }
#else
    if (path[0] == '/') {
        return Arch_NormPath(path, false, outPath);
    }

    char* cwd = nullptr;
    UsdResult result = Arch_GetCwd(&cwd);
    if (result != USD_RESULT_SUCCESS || !cwd) {
        *outPath = nullptr;
        return USD_RESULT_ERROR;
    }

    std::string fullPath = cwd;
    fullPath += "/";
    fullPath += path;
    free(cwd);

    return Arch_NormPath(fullPath.c_str(), false, outPath);
#endif
}

UsdResult Arch_GetModificationTime(const char* pathname, double* outTime) {
    if (!pathname || !outTime) return USD_RESULT_INVALID_ARGUMENT;

#if ARCH_USE_USD
    try {
        if (ArchGetModificationTime(pathname, outTime)) {
            return USD_RESULT_SUCCESS;
        }
    } catch (...) {
        // Fall through
    }
    return USD_RESULT_NOT_FOUND;
#else
    struct stat st;
    if (stat(pathname, &st) == 0) {
        #if defined(__APPLE__)
        *outTime = static_cast<double>(st.st_mtimespec.tv_sec) +
                   static_cast<double>(st.st_mtimespec.tv_nsec) / 1e9;
        #else
        *outTime = static_cast<double>(st.st_mtime);
        #endif
        return USD_RESULT_SUCCESS;
    }
    return USD_RESULT_NOT_FOUND;
#endif
}

// MARK: - Error Handling

UsdResult Arch_Strerror(char** outError) {
    if (!outError) return USD_RESULT_INVALID_ARGUMENT;

#if ARCH_USE_USD
    try {
        std::string errStr = ArchStrerror();
        *outError = DuplicateString(errStr);
        return *outError ? USD_RESULT_SUCCESS : USD_RESULT_OUT_OF_MEMORY;
    } catch (...) {
        *outError = DuplicateString("Unknown error");
        return *outError ? USD_RESULT_SUCCESS : USD_RESULT_OUT_OF_MEMORY;
    }
#else
    char buffer[256];
    #if defined(__APPLE__)
    int result = strerror_r(errno, buffer, sizeof(buffer));
    if (result == 0) {
        *outError = DuplicateString(buffer);
    } else {
        *outError = DuplicateString("Unknown error");
    }
    #else
    *outError = DuplicateString(strerror(errno));
    #endif
    return *outError ? USD_RESULT_SUCCESS : USD_RESULT_OUT_OF_MEMORY;
#endif
}

UsdResult Arch_StrerrorCode(int errorCode, char** outError) {
    if (!outError) return USD_RESULT_INVALID_ARGUMENT;

#if ARCH_USE_USD
    try {
        std::string errStr = ArchStrerror(errorCode);
        *outError = DuplicateString(errStr);
        return *outError ? USD_RESULT_SUCCESS : USD_RESULT_OUT_OF_MEMORY;
    } catch (...) {
        *outError = DuplicateString("Unknown error");
        return *outError ? USD_RESULT_SUCCESS : USD_RESULT_OUT_OF_MEMORY;
    }
#else
    char buffer[256];
    #if defined(__APPLE__)
    int result = strerror_r(errorCode, buffer, sizeof(buffer));
    if (result == 0) {
        *outError = DuplicateString(buffer);
    } else {
        *outError = DuplicateString("Unknown error");
    }
    #else
    *outError = DuplicateString(strerror(errorCode));
    #endif
    return *outError ? USD_RESULT_SUCCESS : USD_RESULT_OUT_OF_MEMORY;
#endif
}

// MARK: - Debugger

bool Arch_DebuggerIsAttached(void) {
#if ARCH_USE_USD
    try {
        return ArchDebuggerIsAttached();
    } catch (...) {
        return false;
    }
#else
    #if defined(__APPLE__)
    // Check if being traced (debugger attached)
    int mib[4] = {CTL_KERN, KERN_PROC, KERN_PROC_PID, getpid()};
    struct kinfo_proc info;
    size_t size = sizeof(info);
    if (sysctl(mib, 4, &info, &size, nullptr, 0) == 0) {
        return (info.kp_proc.p_flag & P_TRACED) != 0;
    }
    #endif
    return false;
#endif
}

void Arch_DebuggerTrap(void) {
#if ARCH_USE_USD
    try {
        ArchDebuggerTrap();
    } catch (...) {
        // Fallback to inline trap
        #if defined(__APPLE__) || defined(__linux__)
        #if defined(__arm64__) || defined(__aarch64__)
        __asm__ volatile("brk #0");
        #elif defined(__x86_64__)
        __asm__ volatile("int $3");
        #endif
        #endif
    }
#else
    #if defined(__APPLE__) || defined(__linux__)
    #if defined(__arm64__) || defined(__aarch64__)
    __asm__ volatile("brk #0");
    #elif defined(__x86_64__)
    __asm__ volatile("int $3");
    #endif
    #endif
#endif
}

void Arch_DebuggerWait(bool wait) {
#if ARCH_USE_USD
    try {
        ArchDebuggerWait(wait);
    } catch (...) {
        // No-op fallback
    }
#else
    // Stub - actual implementation would set a flag that ArchDebuggerTrap checks
    (void)wait;
#endif
}

} // extern "C"
