// Arch.swift - Platform utilities wrapper
// Mirrors: pxr/base/arch/*.h (platform, timing, filesystem, environment)

import USDCxx

// MARK: - Arch Namespace

/// Architecture-dependent utilities for platform info, timing, filesystem, and environment.
/// Mirrors pxr::Arch from the USD C++ API.
public enum Arch {

    // MARK: - Platform Information

    /// Returns the system's memory page size. Safe to assume power-of-two.
    public static var pageSize: Int {
        Int(swiftusd.getPageSize())
    }

    /// Returns true if the calling thread is the main thread.
    public static var isMainThread: Bool {
        swiftusd.isMainThread()
    }

    // MARK: - System Information

    /// Returns the current working directory.
    public static var currentWorkingDirectory: String {
        get throws {
            guard let ptr = swiftusd.getCwd() else {
                throw ArchError.systemError("Failed to get current working directory")
            }
            defer { swiftusd.freeString(ptr) }
            return String(cString: ptr)
        }
    }

    /// Returns the path to the current executable.
    public static var executablePath: String {
        get throws {
            guard let ptr = swiftusd.getExecutablePath() else {
                throw ArchError.systemError("Failed to get executable path")
            }
            defer { swiftusd.freeString(ptr) }
            return String(cString: ptr)
        }
    }

    // MARK: - File System

    /// Returns the path to a temporary directory for this platform.
    public static var temporaryDirectory: String {
        String(cString: swiftusd.getTmpDir())
    }

    /// Creates a temporary file name.
    /// - Parameters:
    ///   - prefix: Prefix for the file name
    ///   - suffix: Optional suffix for the file name
    /// - Returns: The generated temporary file path
    public static func makeTemporaryFileName(prefix: String, suffix: String = "") throws -> String {
        if suffix.isEmpty {
            guard let ptr = swiftusd.makeTmpFileName(prefix, nil) else {
                throw ArchError.systemError("Failed to create temporary file name")
            }
            defer { swiftusd.freeString(ptr) }
            return String(cString: ptr)
        } else {
            return try suffix.withCString { suffixPtr in
                guard let ptr = swiftusd.makeTmpFileName(prefix, suffixPtr) else {
                    throw ArchError.systemError("Failed to create temporary file name")
                }
                defer { swiftusd.freeString(ptr) }
                return String(cString: ptr)
            }
        }
    }

    /// Creates a temporary file and returns the file descriptor and path.
    /// - Parameter prefix: Prefix for the file name
    /// - Returns: A tuple containing the file descriptor and path
    public static func makeTemporaryFile(prefix: String) throws -> (fileDescriptor: Int32, path: String) {
        var pathPtr: UnsafeMutablePointer<CChar>?
        let fd = swiftusd.makeTmpFile(prefix, &pathPtr)
        guard fd >= 0, let path = pathPtr else {
            throw ArchError.systemError("Failed to create temporary file")
        }
        defer { swiftusd.freeString(path) }
        return (fd, String(cString: path))
    }

    /// Creates a temporary subdirectory.
    /// - Parameters:
    ///   - directory: Parent directory for the temp subdirectory
    ///   - prefix: Prefix for the directory name
    /// - Returns: The path to the created subdirectory
    public static func makeTemporarySubdirectory(in directory: String, prefix: String) throws -> String {
        guard let ptr = swiftusd.makeTmpSubdir(directory, prefix) else {
            throw ArchError.systemError("Failed to create temporary subdirectory")
        }
        defer { swiftusd.freeString(ptr) }
        return String(cString: ptr)
    }

    /// Returns the length of a file in bytes.
    /// - Parameter path: Path to the file
    /// - Returns: The file size in bytes, or nil if the file cannot be read
    public static func fileLength(at path: String) -> Int64? {
        let length = swiftusd.getFileLength(path)
        return length >= 0 ? length : nil
    }

    /// Normalizes a path, eliminating double slashes and resolving . and ..
    /// - Parameters:
    ///   - path: The path to normalize
    ///   - stripDriveSpecifier: On Windows, whether to remove drive specifiers
    /// - Returns: The normalized path
    public static func normalizePath(_ path: String, stripDriveSpecifier: Bool = false) throws -> String {
        guard let ptr = swiftusd.normPath(path, stripDriveSpecifier) else {
            throw ArchError.systemError("Failed to normalize path")
        }
        defer { swiftusd.freeString(ptr) }
        return String(cString: ptr)
    }

    /// Returns the absolute path for a given path.
    /// - Parameter path: The relative or absolute path
    /// - Returns: The absolute path
    public static func absolutePath(for path: String) throws -> String {
        guard let ptr = swiftusd.absPath(path) else {
            throw ArchError.systemError("Failed to get absolute path")
        }
        defer { swiftusd.freeString(ptr) }
        return String(cString: ptr)
    }

    /// Returns the modification time of a file.
    /// - Parameter path: Path to the file
    /// - Returns: The modification time as seconds since epoch
    public static func modificationTime(at path: String) throws -> Double {
        let time = swiftusd.getModificationTime(path)
        guard time >= 0 else {
            throw ArchError.fileNotFound(path)
        }
        return time
    }

    // MARK: - Error Handling

    /// Returns the error string for the current errno value.
    public static var currentErrorString: String {
        get throws {
            guard let ptr = swiftusd.strerror() else {
                throw ArchError.systemError("Failed to get error string")
            }
            defer { swiftusd.freeString(ptr) }
            return String(cString: ptr)
        }
    }

    /// Returns the error string for a specific error code.
    /// - Parameter errorCode: The error code (errno value)
    /// - Returns: The error description string
    public static func errorString(for errorCode: Int32) throws -> String {
        guard let ptr = swiftusd.strerror(errorCode) else {
            throw ArchError.systemError("Failed to get error string for code \(errorCode)")
        }
        defer { swiftusd.freeString(ptr) }
        return String(cString: ptr)
    }

    // MARK: - Debugger

    /// Returns true if a debugger is currently attached to the process.
    public static var isDebuggerAttached: Bool {
        swiftusd.debuggerIsAttached()
    }

    /// Triggers a debugger trap (breakpoint).
    public static func debuggerTrap() {
        swiftusd.debuggerTrap()
    }

    /// Sets whether debug traps should wait for debugger attachment.
    public static func setDebuggerWait(_ wait: Bool) {
        swiftusd.debuggerWait(wait)
    }
}

// MARK: - Environment

extension Arch {

    /// Environment variable utilities.
    public enum Environment {

        /// Returns true if the environment variable exists.
        /// - Parameter name: Name of the environment variable
        public static func has(_ name: String) -> Bool {
            swiftusd.hasEnv(name)
        }

        /// Gets the value of an environment variable.
        /// - Parameter name: Name of the environment variable
        /// - Returns: The value, or nil if not set
        public static func get(_ name: String) -> String? {
            guard let ptr = swiftusd.getEnv(name) else {
                return nil
            }
            defer { swiftusd.freeString(ptr) }
            return String(cString: ptr)
        }

        /// Sets an environment variable.
        /// - Parameters:
        ///   - name: Name of the environment variable
        ///   - value: Value to set
        ///   - overwrite: Whether to overwrite existing values
        /// - Returns: True if successful
        @discardableResult
        public static func set(_ name: String, value: String, overwrite: Bool = true) -> Bool {
            swiftusd.setEnv(name, value, overwrite)
        }

        /// Removes an environment variable.
        /// - Parameter name: Name of the environment variable to remove
        /// - Returns: True if successful
        @discardableResult
        public static func remove(_ name: String) -> Bool {
            swiftusd.removeEnv(name)
        }

        /// Expands environment variables in a string.
        /// - Parameter string: String containing environment variable references (e.g., ${VAR} or $VAR)
        /// - Returns: The expanded string
        public static func expand(_ string: String) throws -> String {
            guard let ptr = swiftusd.expandEnvironmentVariables(string) else {
                throw ArchError.systemError("Failed to expand environment variables")
            }
            defer { swiftusd.freeString(ptr) }
            return String(cString: ptr)
        }

        /// Subscript access to environment variables.
        public static subscript(name: String) -> String? {
            get { get(name) }
            set {
                if let value = newValue {
                    set(name, value: value)
                } else {
                    remove(name)
                }
            }
        }
    }
}

// MARK: - Timing

extension Arch {

    /// High-resolution timing utilities.
    public enum Timing {

        /// Returns the current time in system-dependent tick units.
        public static var tickTime: UInt64 {
            swiftusd.getTickTime()
        }

        /// Returns a "start" tick time optimized for interval measurement.
        public static var startTickTime: UInt64 {
            swiftusd.getStartTickTime()
        }

        /// Returns a "stop" tick time optimized for interval measurement.
        public static var stopTickTime: UInt64 {
            swiftusd.getStopTickTime()
        }

        /// Returns the tick time resolution (minimum tick increment).
        public static var tickQuantum: UInt64 {
            swiftusd.getTickQuantum()
        }

        /// Returns the measurement overhead in ticks.
        public static var intervalTimerTickOverhead: UInt64 {
            swiftusd.getIntervalTimerTickOverhead()
        }

        /// Returns nanoseconds per tick.
        public static var nanosecondsPerTick: Double {
            swiftusd.getNanosecondsPerTick()
        }

        /// Converts ticks to nanoseconds.
        /// - Parameter ticks: Number of ticks
        /// - Returns: Duration in nanoseconds
        public static func ticksToNanoseconds(_ ticks: UInt64) -> Int64 {
            swiftusd.ticksToNanoseconds(ticks)
        }

        /// Converts ticks to seconds.
        /// - Parameter ticks: Number of ticks
        /// - Returns: Duration in seconds
        public static func ticksToSeconds(_ ticks: UInt64) -> Double {
            swiftusd.ticksToSeconds(ticks)
        }

        /// Converts seconds to ticks.
        /// - Parameter seconds: Duration in seconds
        /// - Returns: Number of ticks
        public static func secondsToTicks(_ seconds: Double) -> UInt64 {
            swiftusd.secondsToTicks(seconds)
        }
    }
}

// MARK: - Interval Timer

extension Arch {

    /// A simple timer class for measuring intervals of time.
    public final class IntervalTimer: @unchecked Sendable {
        private var startTicks: UInt64 = 0
        private var _isStarted: Bool = false

        /// Creates a new interval timer.
        /// - Parameter start: Whether to start the timer immediately
        public init(start: Bool = true) {
            if start {
                self.start()
            }
        }

        /// Starts or restarts the timer.
        public func start() {
            startTicks = Timing.startTickTime
            _isStarted = true
        }

        /// Returns whether the timer has been started.
        public var isStarted: Bool {
            _isStarted
        }

        /// Returns the start time in ticks.
        public var startTime: UInt64 {
            startTicks
        }

        /// Returns the current time in ticks.
        public var currentTicks: UInt64 {
            Timing.stopTickTime
        }

        /// Returns the elapsed time in ticks since the timer was started.
        public var elapsedTicks: UInt64 {
            guard _isStarted else { return 0 }
            return Timing.stopTickTime - startTicks
        }

        /// Returns the elapsed time in nanoseconds.
        public var elapsedNanoseconds: Int64 {
            Timing.ticksToNanoseconds(elapsedTicks)
        }

        /// Returns the elapsed time in seconds.
        public var elapsedSeconds: Double {
            Timing.ticksToSeconds(elapsedTicks)
        }
    }
}

// MARK: - Error Type

/// Errors that can occur in Arch operations.
public enum ArchError: Error, CustomStringConvertible {
    case systemError(String)
    case fileNotFound(String)
    case invalidArgument(String)

    public var description: String {
        switch self {
        case .systemError(let message):
            return "Arch system error: \(message)"
        case .fileNotFound(let path):
            return "File not found: \(path)"
        case .invalidArgument(let message):
            return "Invalid argument: \(message)"
        }
    }
}
