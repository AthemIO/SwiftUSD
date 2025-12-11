/* ----------------------------------------------------------------
 *  A T H E M
 * ----------------------------------------------------------------
 *  Copyright (C) 2016 Pixar.
 *  Copyright (C) 2025 Afloat Technologies. All Rights Reserved.
 *  Licensed under https://openusd.org/license
 * ---------------------------------------------------------------- */

import Hgi

/// Hydra Graphics Interface namespace.
///
/// Hgi is a graphics API abstraction layer that allows Hydra to work with
/// multiple graphics backends (Metal, OpenGL, Vulkan).
public enum Hgi {}

// MARK: - Hgi.Instance

public extension Hgi
{
  /// A wrapper around a platform-specific Hgi instance with automatic memory management.
  ///
  /// `Hgi.Instance` manages the lifetime of an underlying C++ `Hgi` object.
  /// Use `createPlatformDefault()` or `createNamed(_:)` to create instances.
  ///
  /// ## Memory Management
  ///
  /// This class automatically manages the C++ object's lifetime via `deinit`.
  /// The Hgi instance is destroyed when this Swift object is deallocated.
  ///
  /// **Important**: Command buffers created from this instance must not
  /// outlive it. Ensure all `GraphicsCmds`, `BlitCmds`, and `ComputeCmds`
  /// are destroyed before the `Hgi.Instance`.
  ///
  /// ## Thread Safety
  ///
  /// - Creation: Not thread safe
  /// - Command submission: Main thread only
  /// - Command recording: One secondary thread per command buffer
  ///
  /// ## Example
  ///
  /// ```swift
  /// guard let hgi = Hgi.Instance.createPlatformDefault() else {
  ///     print("Failed to create Hgi")
  ///     return
  /// }
  ///
  /// // Create and record a blit command buffer
  /// if let blitCmds = hgi.createBlitCmds() {
  ///     // ... record commands using blitCmds.unsafeTypedPointer ...
  ///     hgi.submitCmds(blitCmds)
  /// }
  /// // blitCmds automatically destroyed when going out of scope
  /// ```
  final class Instance
  {
    /// The underlying C++ Hgi pointer (stored as opaque raw pointer).
    private var _hgiPtr: UnsafeMutableRawPointer?

    /// Private initializer from opaque pointer.
    private init(ptr: UnsafeMutableRawPointer?)
    {
      _hgiPtr = ptr
    }

    deinit
    {
      if let ptr = _hgiPtr
      {
        Pixar.Hgi_Swift_DestroyHgi(ptr)
      }
    }

    /// Creates the platform default Hgi instance.
    ///
    /// On macOS this returns HgiMetal, on Linux HgiGL, etc.
    ///
    /// - Returns: A new Hgi instance, or `nil` if creation failed.
    public static func createPlatformDefault() -> Instance?
    {
      guard let ptr = Pixar.Hgi_Swift_CreatePlatformDefaultHgi()
      else
      {
        return nil
      }
      return Instance(ptr: ptr)
    }

    /// Creates an Hgi instance of the specified backend type.
    ///
    /// - Parameter token: The backend token (e.g., from `HgiTokens`).
    /// - Returns: A new Hgi instance, or `nil` if the backend is unavailable.
    public static func createNamed(_ token: Pixar.TfToken) -> Instance?
    {
      guard let ptr = Pixar.Hgi_Swift_CreateNamedHgi(token)
      else
      {
        return nil
      }
      return Instance(ptr: ptr)
    }

    /// Returns whether the instance is valid.
    public var isValid: Bool
    {
      _hgiPtr != nil
    }

    /// Returns the API name (e.g., "Metal", "OpenGL").
    public var apiName: Pixar.TfToken
    {
      guard let ptr = _hgiPtr
      else
      {
        return Pixar.TfToken()
      }
      return Pixar.Hgi_Swift_GetAPIName(ptr)
    }

    /// Returns whether the backend is supported on the current hardware.
    public var isBackendSupported: Bool
    {
      guard let ptr = _hgiPtr
      else
      {
        return false
      }
      return Pixar.Hgi_Swift_IsBackendSupported(ptr)
    }

    /// Provides access to the underlying opaque pointer for advanced usage.
    ///
    /// - Warning: The pointer is only valid while this Instance exists.
    public var unsafePointer: UnsafeMutableRawPointer?
    {
      _hgiPtr
    }

    // MARK: - Command Buffer Creation

    /// Creates a graphics command buffer for rendering operations.
    ///
    /// - Parameter desc: The graphics commands descriptor.
    /// - Returns: A new graphics command buffer, or `nil` if creation failed.
    ///
    /// - Important: The returned command buffer must not outlive this Hgi instance.
    public func createGraphicsCmds(desc: Pixar.HgiGraphicsCmdsDesc) -> GraphicsCmds?
    {
      guard let hgiPtr = _hgiPtr
      else
      {
        return nil
      }
      guard let cmdsPtr = Pixar.Hgi_Swift_CreateGraphicsCmds(hgiPtr, desc)
      else
      {
        return nil
      }
      return GraphicsCmds(ptr: cmdsPtr)
    }

    /// Creates a blit (copy) command buffer for resource operations.
    ///
    /// - Returns: A new blit command buffer, or `nil` if creation failed.
    ///
    /// - Important: The returned command buffer must not outlive this Hgi instance.
    public func createBlitCmds() -> BlitCmds?
    {
      guard let hgiPtr = _hgiPtr
      else
      {
        return nil
      }
      guard let cmdsPtr = Pixar.Hgi_Swift_CreateBlitCmds(hgiPtr)
      else
      {
        return nil
      }
      return BlitCmds(ptr: cmdsPtr)
    }

    /// Creates a compute command buffer for compute shader dispatch.
    ///
    /// - Parameter desc: The compute commands descriptor.
    /// - Returns: A new compute command buffer, or `nil` if creation failed.
    ///
    /// - Important: The returned command buffer must not outlive this Hgi instance.
    public func createComputeCmds(desc: Pixar.HgiComputeCmdsDesc) -> ComputeCmds?
    {
      guard let hgiPtr = _hgiPtr
      else
      {
        return nil
      }
      guard let cmdsPtr = Pixar.Hgi_Swift_CreateComputeCmds(hgiPtr, desc)
      else
      {
        return nil
      }
      return ComputeCmds(ptr: cmdsPtr)
    }

    // MARK: - Command Submission

    /// Submits a graphics command buffer to the GPU.
    ///
    /// - Parameters:
    ///   - cmds: The graphics command buffer to submit.
    ///   - wait: The wait type for synchronization (default: no wait).
    ///
    /// - Important: Must be called on the main thread.
    public func submitCmds(_ cmds: GraphicsCmds, wait: Pixar.HgiSubmitWaitType = Pixar.HgiSubmitWaitTypeNoWait)
    {
      guard let hgiPtr = _hgiPtr, let cmdsPtr = cmds._cmdsPtr
      else
      {
        return
      }
      Pixar.Hgi_Swift_SubmitCmds(hgiPtr, cmdsPtr, wait)
    }

    /// Submits a blit command buffer to the GPU.
    ///
    /// - Parameters:
    ///   - cmds: The blit command buffer to submit.
    ///   - wait: The wait type for synchronization (default: no wait).
    ///
    /// - Important: Must be called on the main thread.
    public func submitCmds(_ cmds: BlitCmds, wait: Pixar.HgiSubmitWaitType = Pixar.HgiSubmitWaitTypeNoWait)
    {
      guard let hgiPtr = _hgiPtr, let cmdsPtr = cmds._cmdsPtr
      else
      {
        return
      }
      Pixar.Hgi_Swift_SubmitCmds(hgiPtr, cmdsPtr, wait)
    }

    /// Submits a compute command buffer to the GPU.
    ///
    /// - Parameters:
    ///   - cmds: The compute command buffer to submit.
    ///   - wait: The wait type for synchronization (default: no wait).
    ///
    /// - Important: Must be called on the main thread.
    public func submitCmds(_ cmds: ComputeCmds, wait: Pixar.HgiSubmitWaitType = Pixar.HgiSubmitWaitTypeNoWait)
    {
      guard let hgiPtr = _hgiPtr, let cmdsPtr = cmds._cmdsPtr
      else
      {
        return
      }
      Pixar.Hgi_Swift_SubmitCmds(hgiPtr, cmdsPtr, wait)
    }

    // MARK: - Frame Management

    /// Called at the start of a new rendering frame.
    ///
    /// This is optional and used for GPU frame debug markers.
    public func startFrame()
    {
      guard let ptr = _hgiPtr
      else
      {
        return
      }
      Pixar.Hgi_Swift_StartFrame(ptr)
    }

    /// Called at the end of a rendering frame.
    ///
    /// This is optional and used for GPU frame debug markers.
    public func endFrame()
    {
      guard let ptr = _hgiPtr
      else
      {
        return
      }
      Pixar.Hgi_Swift_EndFrame(ptr)
    }

    /// Performs garbage collection of GPU resources.
    ///
    /// This can be used to flush pending deletes immediately after unloading assets.
    public func garbageCollect()
    {
      guard let ptr = _hgiPtr
      else
      {
        return
      }
      Pixar.Hgi_Swift_GarbageCollect(ptr)
    }
  }
}

// MARK: - Hgi.GraphicsCmds

public extension Hgi
{
  /// A graphics command buffer wrapper with automatic memory management.
  ///
  /// Graphics command buffers record rendering commands like draw calls,
  /// pipeline binding, and resource binding.
  ///
  /// ## Memory Management
  ///
  /// This class manages the lifetime of the underlying C++ `HgiGraphicsCmds`.
  /// The command buffer is destroyed when this Swift object is deallocated.
  ///
  /// ## Thread Safety
  ///
  /// - Creation: Main thread
  /// - Recording: One secondary thread (exclusive access)
  /// - Submission: Main thread
  final class GraphicsCmds
  {
    /// The underlying C++ HgiGraphicsCmds pointer.
    var _cmdsPtr: UnsafeMutableRawPointer?

    /// Private initializer from opaque pointer.
    init(ptr: UnsafeMutableRawPointer?)
    {
      _cmdsPtr = ptr
    }

    deinit
    {
      if let ptr = _cmdsPtr
      {
        Pixar.Hgi_Swift_DestroyGraphicsCmds(ptr)
      }
    }

    /// Returns whether the command buffer is valid.
    public var isValid: Bool
    {
      _cmdsPtr != nil
    }

    /// Provides access to the underlying opaque pointer for advanced C++ interop.
    ///
    /// This pointer can be cast to `HgiGraphicsCmds*` in C++ code or passed
    /// to bridge functions that expect the raw command buffer pointer.
    ///
    /// - Returns: The underlying pointer, or `nil` if invalid.
    /// - Warning: The pointer is only valid while this `GraphicsCmds` exists.
    public var unsafePointer: UnsafeMutableRawPointer?
    {
      _cmdsPtr
    }
  }
}

// MARK: - Hgi.BlitCmds

public extension Hgi
{
  /// A blit (copy) command buffer wrapper with automatic memory management.
  ///
  /// Blit command buffers record resource copy operations between GPU
  /// and CPU memory, such as texture uploads and buffer copies.
  ///
  /// ## Memory Management
  ///
  /// This class manages the lifetime of the underlying C++ `HgiBlitCmds`.
  /// The command buffer is destroyed when this Swift object is deallocated.
  final class BlitCmds
  {
    /// The underlying C++ HgiBlitCmds pointer.
    var _cmdsPtr: UnsafeMutableRawPointer?

    /// Private initializer from opaque pointer.
    init(ptr: UnsafeMutableRawPointer?)
    {
      _cmdsPtr = ptr
    }

    deinit
    {
      if let ptr = _cmdsPtr
      {
        Pixar.Hgi_Swift_DestroyBlitCmds(ptr)
      }
    }

    /// Returns whether the command buffer is valid.
    public var isValid: Bool
    {
      _cmdsPtr != nil
    }

    /// Provides access to the underlying opaque pointer for advanced C++ interop.
    ///
    /// This pointer can be cast to `HgiBlitCmds*` in C++ code or passed
    /// to bridge functions that expect the raw command buffer pointer.
    ///
    /// - Returns: The underlying pointer, or `nil` if invalid.
    /// - Warning: The pointer is only valid while this `BlitCmds` exists.
    public var unsafePointer: UnsafeMutableRawPointer?
    {
      _cmdsPtr
    }
  }
}

// MARK: - Hgi.ComputeCmds

public extension Hgi
{
  /// A compute command buffer wrapper with automatic memory management.
  ///
  /// Compute command buffers record compute shader dispatch commands
  /// for general-purpose GPU computation.
  ///
  /// ## Memory Management
  ///
  /// This class manages the lifetime of the underlying C++ `HgiComputeCmds`.
  /// The command buffer is destroyed when this Swift object is deallocated.
  final class ComputeCmds
  {
    /// The underlying C++ HgiComputeCmds pointer.
    var _cmdsPtr: UnsafeMutableRawPointer?

    /// Private initializer from opaque pointer.
    init(ptr: UnsafeMutableRawPointer?)
    {
      _cmdsPtr = ptr
    }

    deinit
    {
      if let ptr = _cmdsPtr
      {
        Pixar.Hgi_Swift_DestroyComputeCmds(ptr)
      }
    }

    /// Returns whether the command buffer is valid.
    public var isValid: Bool
    {
      _cmdsPtr != nil
    }

    /// Provides access to the underlying opaque pointer for advanced C++ interop.
    ///
    /// This pointer can be cast to `HgiComputeCmds*` in C++ code or passed
    /// to bridge functions that expect the raw command buffer pointer.
    ///
    /// - Returns: The underlying pointer, or `nil` if invalid.
    /// - Warning: The pointer is only valid while this `ComputeCmds` exists.
    public var unsafePointer: UnsafeMutableRawPointer?
    {
      _cmdsPtr
    }
  }
}

