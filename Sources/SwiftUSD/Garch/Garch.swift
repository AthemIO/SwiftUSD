// Garch.swift - Graphics Architecture wrapper
// Mirrors: pxr/imaging/garch/*.h (GL context, platform detection)

import OpenUSDInterop

// MARK: - Garch Namespace

/// Graphics Architecture utilities for GL context management and platform detection.
/// Mirrors pxr::Garch from the USD imaging library.
///
/// The Garch module provides:
/// - Platform detection (OpenGL, Metal, Vulkan availability)
/// - GL context state management
/// - GL debug context creation
/// - GL extension querying
///
/// ## Usage
/// ```swift
/// // Check platform capabilities
/// print("Platform: \(Garch.platform)")
/// print("Has OpenGL: \(Garch.hasOpenGL)")
/// print("Has Metal: \(Garch.hasMetal)")
///
/// // Get current GL context
/// if let context = GLContextState.current {
///     print("Has valid GL context")
/// }
/// ```
public enum Garch {

    // MARK: - Platform Type

    /// Graphics platform types available on the system.
    public enum Platform: Int, Sendable, CustomStringConvertible {
        case none = 0
        case openGL = 1
        case metal = 2
        case vulkan = 3

        public var description: String {
            switch self {
            case .none: return "None"
            case .openGL: return "OpenGL"
            case .metal: return "Metal"
            case .vulkan: return "Vulkan"
            }
        }
    }

    // MARK: - Platform Detection

    /// Returns the primary graphics platform for this system.
    public static var platform: Platform {
        Platform(rawValue: Int(Garch_GetPlatform().rawValue)) ?? .none
    }

    /// Returns true if OpenGL is available on this system.
    public static var hasOpenGL: Bool {
        Garch_HasOpenGL()
    }

    /// Returns true if Metal is available on this system (macOS/iOS only).
    public static var hasMetal: Bool {
        Garch_HasMetal()
    }

    /// Returns true if Vulkan is available on this system.
    public static var hasVulkan: Bool {
        Garch_HasVulkan()
    }
}

// MARK: - GL Context State

extension Garch {

    /// Represents the state of a GL platform context.
    /// Used for capturing and restoring GL context state.
    public final class GLContextState: @unchecked Sendable {
        private let handle: GarchGLContextStateRef

        private init(_ handle: GarchGLContextStateRef) {
            self.handle = handle
        }

        deinit {
            GarchGLContextState_Release(handle)
        }

        /// Gets the current GL context state.
        /// Returns nil if no context is current.
        public static var current: GLContextState? {
            guard let handle = GarchGLContextState_GetCurrent() else {
                return nil
            }
            let state = GLContextState(handle)
            return state.isValid ? state : nil
        }

        /// Creates a null/empty context state.
        public static var null: GLContextState {
            GLContextState(GarchGLContextState_CreateNull()!)
        }

        /// Returns true if this context state is valid (not null/empty).
        public var isValid: Bool {
            GarchGLContextState_IsValid(handle)
        }

        /// Returns a hash value for this context state.
        public var hashValue: Int {
            Int(GarchGLContextState_GetHash(handle))
        }

        /// Makes this context state current.
        public func makeCurrent() throws {
            let result = GarchGLContextState_MakeCurrent(handle)
            guard result == USD_RESULT_SUCCESS else {
                throw GarchError.contextError("Failed to make GL context current")
            }
        }

        /// Clears the current GL context (makes no context current).
        public static func doneCurrent() {
            GarchGLContextState_DoneCurrent()
        }

        /// Compares two context states for equality.
        public func equals(_ other: GLContextState) -> Bool {
            GarchGLContextState_Equals(handle, other.handle)
        }
    }
}

// MARK: - GL Debug Context

extension Garch {

    /// A GL debug context for debugging and testing.
    /// Provides a GL context with debug output capabilities.
    public final class GLDebugContext: @unchecked Sendable {
        private let handle: GarchGLDebugContextRef

        /// Creates a new GL debug context.
        /// - Parameters:
        ///   - majorVersion: Requested OpenGL major version (default: 4)
        ///   - minorVersion: Requested OpenGL minor version (default: 1)
        ///   - coreProfile: Whether to use core profile (default: true)
        ///   - directRendering: Whether to use direct rendering (default: true)
        public init?(
            majorVersion: Int = 4,
            minorVersion: Int = 1,
            coreProfile: Bool = true,
            directRendering: Bool = true
        ) {
            guard let handle = GarchGLDebugContext_Create(
                Int32(majorVersion),
                Int32(minorVersion),
                coreProfile,
                directRendering
            ) else {
                return nil
            }
            self.handle = handle
        }

        deinit {
            GarchGLDebugContext_Release(handle)
        }

        /// Makes this debug context current.
        public func makeCurrent() throws {
            let result = GarchGLDebugContext_MakeCurrent(handle)
            guard result == USD_RESULT_SUCCESS else {
                throw GarchError.contextError("Failed to make debug context current")
            }
        }

        /// Returns true if debug output is enabled globally.
        public static var isDebugOutputEnabled: Bool {
            GarchGLDebugContext_IsEnabledDebugOutput()
        }

        /// Returns true if core profile is enabled globally.
        public static var isCoreProfileEnabled: Bool {
            GarchGLDebugContext_IsEnabledCoreProfile()
        }
    }
}

// MARK: - GL API Information

extension Garch {

    /// Information about the current GL API.
    public struct GLAPIInfo: Sendable, CustomStringConvertible {
        public let majorVersion: Int
        public let minorVersion: Int
        public let isCoreProfile: Bool
        public let isES: Bool
        public let hasDebugOutput: Bool

        public var description: String {
            let profile = isCoreProfile ? "Core" : "Compatibility"
            let variant = isES ? "ES" : ""
            return "OpenGL\(variant) \(majorVersion).\(minorVersion) \(profile)"
        }
    }

    /// Gets information about the current GL context's API.
    /// Returns nil if no context is current.
    public static var glAPIInfo: GLAPIInfo? {
        var info = GarchGLAPIInfo()
        guard Garch_GetGLAPIInfo(&info) else {
            return nil
        }
        return GLAPIInfo(
            majorVersion: Int(info.majorVersion),
            minorVersion: Int(info.minorVersion),
            isCoreProfile: info.isCoreProfile,
            isES: info.isES,
            hasDebugOutput: info.hasDebugOutput
        )
    }

    /// Returns the GL version string for the current context.
    public static var glVersionString: String? {
        var strPtr: UnsafeMutablePointer<CChar>?
        guard Garch_GetGLVersionString(&strPtr) == USD_RESULT_SUCCESS,
              let str = strPtr else {
            return nil
        }
        defer { Garch_FreeString(str) }
        return String(cString: str)
    }

    /// Returns the GL renderer string for the current context.
    public static var glRendererString: String? {
        var strPtr: UnsafeMutablePointer<CChar>?
        guard Garch_GetGLRendererString(&strPtr) == USD_RESULT_SUCCESS,
              let str = strPtr else {
            return nil
        }
        defer { Garch_FreeString(str) }
        return String(cString: str)
    }

    /// Returns the GL vendor string for the current context.
    public static var glVendorString: String? {
        var strPtr: UnsafeMutablePointer<CChar>?
        guard Garch_GetGLVendorString(&strPtr) == USD_RESULT_SUCCESS,
              let str = strPtr else {
            return nil
        }
        defer { Garch_FreeString(str) }
        return String(cString: str)
    }

    /// Returns the GLSL version string for the current context.
    public static var glslVersionString: String? {
        var strPtr: UnsafeMutablePointer<CChar>?
        guard Garch_GetGLSLVersionString(&strPtr) == USD_RESULT_SUCCESS,
              let str = strPtr else {
            return nil
        }
        defer { Garch_FreeString(str) }
        return String(cString: str)
    }
}

// MARK: - GL Extensions

extension Garch {

    /// GL extension querying utilities.
    public enum Extensions {

        /// Returns the count of supported GL extensions.
        public static var count: Int {
            Int(Garch_GetGLExtensionCount())
        }

        /// Returns all supported GL extension names.
        public static var all: [String] {
            let extensionCount = count
            guard extensionCount > 0 else { return [] }

            var pointers = [UnsafePointer<CChar>?](repeating: nil, count: extensionCount)
            let actualCount = Garch_GetGLExtensions(&pointers, extensionCount)

            var result = [String]()
            result.reserveCapacity(Int(actualCount))
            for i in 0..<Int(actualCount) {
                if let ptr = pointers[i] {
                    result.append(String(cString: ptr))
                }
            }
            return result
        }

        /// Returns true if the specified GL extension is supported.
        /// - Parameter name: The extension name (e.g., "GL_ARB_texture_storage")
        public static func has(_ name: String) -> Bool {
            Garch_HasGLExtension(name)
        }

        /// Subscript access to check extension support.
        public static subscript(name: String) -> Bool {
            has(name)
        }
    }
}

// MARK: - Error Type

/// Errors that can occur in Garch operations.
public enum GarchError: Error, CustomStringConvertible {
    case contextError(String)
    case platformNotSupported(String)

    public var description: String {
        switch self {
        case .contextError(let message):
            return "GL context error: \(message)"
        case .platformNotSupported(let platform):
            return "Platform not supported: \(platform)"
        }
    }
}
