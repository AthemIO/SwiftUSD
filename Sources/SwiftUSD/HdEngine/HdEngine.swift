// HdEngine.swift - Swift wrappers for Hydra Engine
// Provides convenience extensions and typealiases for HdEngine

import USDCxx

// MARK: - Type Aliases

// Note: HdEngine typealias is already defined in HdCxx.swift as:
// public typealias HdEngine = swiftusd.HdEngineRef
// The new swiftusd.HdEngine class in HdEngine.h provides extended functionality.

/// Reference to a render task.
public typealias HdTask = swiftusd.HdTaskRef

// MARK: - HdEngine Extensions

extension swiftusd.HdEngine {

    // MARK: Factory Methods

    /// Create a new Hydra engine.
    public static func create() -> swiftusd.HdEngine? {
        return swiftusd.HdEngine.Create()
    }

    // MARK: Task Context Management

    /// Set a string value in the task context.
    ///
    /// The task context is a shared dictionary that allows tasks to
    /// communicate with each other during execution. Common uses include
    /// passing camera matrices, viewport dimensions, and render settings.
    ///
    /// - Parameters:
    ///   - id: Token key for the context data
    ///   - value: String value to store
    public func setTaskContextData(id: swiftusd.Token, value: String) {
        SetTaskContextData(id, value)
    }

    /// Set a string value in the task context using a string key.
    ///
    /// - Parameters:
    ///   - key: String key for the context data
    ///   - value: String value to store
    public func setTaskContextData(key: String, value: String) {
        SetTaskContextData(swiftusd.Token(key), value)
    }

    /// Check if task context data exists for the given key.
    ///
    /// - Parameter id: Token key to check
    /// - Returns: true if data exists for the key
    public func hasTaskContextData(id: swiftusd.Token) -> Bool {
        return HasTaskContextData(id)
    }

    /// Check if task context data exists for the given string key.
    ///
    /// - Parameter key: String key to check
    /// - Returns: true if data exists for the key
    public func hasTaskContextData(key: String) -> Bool {
        return HasTaskContextData(swiftusd.Token(key))
    }

    /// Get task context data as a string.
    ///
    /// - Parameter id: Token key for the context data
    /// - Returns: The string value, or empty string if not found
    public func getTaskContextData(id: swiftusd.Token) -> String {
        return String(cString: GetTaskContextData(id))
    }

    /// Get task context data as a string using a string key.
    ///
    /// - Parameter key: String key for the context data
    /// - Returns: The string value, or empty string if not found
    public func getTaskContextData(key: String) -> String {
        return String(cString: GetTaskContextData(swiftusd.Token(key)))
    }

    /// Remove data from the task context.
    ///
    /// - Parameter id: Token key to remove
    public func removeTaskContextData(id: swiftusd.Token) {
        RemoveTaskContextData(id)
    }

    /// Remove data from the task context using a string key.
    ///
    /// - Parameter key: String key to remove
    public func removeTaskContextData(key: String) {
        RemoveTaskContextData(swiftusd.Token(key))
    }

    /// Clear all task context data.
    public func clearTaskContext() {
        ClearTaskContextData()
    }

    /// The number of items in the task context.
    public var taskContextSize: Int {
        return Int(GetTaskContextSize())
    }

    // MARK: Execution

    /// Execute rendering with the given render index.
    ///
    /// This runs the 4-phase Hydra execution pipeline:
    /// 1. Data Discovery - RenderIndex syncs all prims
    /// 2. Prepare - Tasks resolve inter-prim bindings
    /// 3. Data Commit - RenderDelegate commits GPU resources
    /// 4. Execute - Tasks render to frame buffers
    ///
    /// - Parameter renderIndex: The render index containing scene data
    public func execute(renderIndex: swiftusd.HdRenderIndexRef) {
        Execute(renderIndex)
    }

    /// Execute rendering with task paths looked up from the render index.
    ///
    /// - Parameters:
    ///   - renderIndex: The render index containing scene data
    ///   - taskPaths: Array of paths to tasks to execute
    public func execute(renderIndex: swiftusd.HdRenderIndexRef, taskPaths: [swiftusd.Path]) {
        taskPaths.withUnsafeBufferPointer { buffer in
            if let baseAddress = buffer.baseAddress {
                ExecuteTaskPaths(renderIndex, baseAddress, buffer.count)
            }
        }
    }

    /// Check if all tasks at the given paths have converged.
    ///
    /// Used for progressive rendering to determine if rendering is complete.
    /// A task is converged when it has rendered to a stable, final state.
    ///
    /// - Parameters:
    ///   - renderIndex: The render index containing scene data
    ///   - taskPaths: Array of paths to tasks to check
    /// - Returns: true if all tasks have converged
    public func areTasksConverged(renderIndex: swiftusd.HdRenderIndexRef, taskPaths: [swiftusd.Path]) -> Bool {
        return taskPaths.withUnsafeBufferPointer { buffer in
            if let baseAddress = buffer.baseAddress {
                return AreTasksConverged(renderIndex, baseAddress, buffer.count)
            }
            return true
        }
    }

    /// The number of tasks executed in the last Execute call.
    /// For debugging and testing purposes.
    public var lastExecutedTaskCount: Int {
        return Int(GetLastExecutedTaskCount())
    }
}

// MARK: - HdTaskRef Extensions

extension swiftusd.HdTaskRef {

    /// The scene path of this task.
    public var id: swiftusd.Path {
        return GetId()
    }

    /// Whether this task has converged (completed rendering).
    public var isConverged: Bool {
        return IsConverged()
    }

    /// Whether this task reference is valid.
    public var isValid: Bool {
        return IsValid()
    }
}
