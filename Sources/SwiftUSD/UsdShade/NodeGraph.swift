// NodeGraph.swift - UsdShadeNodeGraph wrapper
// Mirrors: pxr/usd/usdShade/nodeGraph.h

import OpenUSDInterop

// MARK: - NodeGraph

/// A container for shading nodes and sub-graphs.
///
/// NodeGraphs provide encapsulation for complex shader networks.
/// They can contain shaders and other node graphs, and expose
/// a public interface through inputs and outputs.
///
/// Mirrors `pxr::UsdShadeNodeGraph` from the USD C++ API.
public final class NodeGraph: @unchecked Sendable {

    // MARK: - Properties

    /// The underlying opaque pointer to the C wrapper.
    internal let handle: UsdShadeNodeGraphRef

    // MARK: - Initialization

    /// Internal initializer from an existing handle.
    /// Takes ownership of the handle (does not retain).
    internal init(handle: UsdShadeNodeGraphRef) {
        self.handle = handle
    }

    deinit {
        UsdShadeNodeGraph_Release(handle)
    }

    // MARK: - Static Factory Methods

    /// Gets a NodeGraph schema from a prim at the given path.
    ///
    /// - Parameters:
    ///   - stage: The stage containing the prim.
    ///   - path: The path to the node graph prim.
    /// - Returns: The node graph, or `nil` if not found or invalid.
    public static func get(stage: Stage, path: Path) -> NodeGraph? {
        guard let ref = UsdShadeNodeGraph_Get(stage.handle, path.handle) else {
            return nil
        }
        return NodeGraph(handle: ref)
    }

    /// Defines a new NodeGraph prim at the given path.
    ///
    /// - Parameters:
    ///   - stage: The stage to create the node graph on.
    ///   - path: The path for the new node graph.
    /// - Returns: The newly created node graph.
    /// - Throws: `ShadeError.creationFailed` if the node graph cannot be created.
    public static func define(stage: Stage, path: Path) throws -> NodeGraph {
        guard let ref = UsdShadeNodeGraph_Define(stage.handle, path.handle) else {
            throw ShadeError.creationFailed("Failed to define node graph at '\(path)'")
        }
        return NodeGraph(handle: ref)
    }

    /// Creates a NodeGraph from an existing prim.
    ///
    /// - Parameter prim: The prim to wrap as a node graph.
    /// - Returns: The node graph, or `nil` if the prim is not a valid node graph.
    public static func fromPrim(_ prim: Prim) -> NodeGraph? {
        guard let ref = UsdShadeNodeGraph_FromPrim(prim.handle) else {
            return nil
        }
        return NodeGraph(handle: ref)
    }

    // MARK: - Properties

    /// Whether the node graph is valid.
    public var isValid: Bool {
        UsdShadeNodeGraph_IsValid(handle)
    }

    /// The underlying prim.
    public var prim: Prim? {
        guard let ref = UsdShadeNodeGraph_GetPrim(handle) else {
            return nil
        }
        return Prim(handle: ref)
    }

    /// The path of the node graph.
    public var path: Path? {
        guard let ref = UsdShadeNodeGraph_GetPath(handle) else {
            return nil
        }
        return Path(handle: ref)
    }

    // MARK: - Inputs

    /// Creates an input on the node graph.
    ///
    /// - Parameters:
    ///   - name: The name of the input.
    ///   - typeName: The type name of the input.
    /// - Returns: The created input.
    public func createInput(name: Token, typeName: Token) -> Input? {
        guard let ref = UsdShadeNodeGraph_CreateInput(handle, name.handle, typeName.handle) else {
            return nil
        }
        return Input(handle: ref)
    }

    /// Gets an input by name.
    ///
    /// - Parameter name: The name of the input.
    /// - Returns: The input, or `nil` if not found.
    public func input(named name: Token) -> Input? {
        guard let ref = UsdShadeNodeGraph_GetInput(handle, name.handle) else {
            return nil
        }
        return Input(handle: ref)
    }

    /// Gets all inputs.
    ///
    /// - Parameter onlyAuthored: If true, only returns authored inputs.
    /// - Returns: Array of inputs.
    public func inputs(onlyAuthored: Bool = true) -> [Input] {
        let count = UsdShadeNodeGraph_GetInputCount(handle, onlyAuthored)
        guard count > 0 else { return [] }

        var handles = [UsdShadeInputRef?](repeating: nil, count: count)
        let actualCount = handles.withUnsafeMutableBufferPointer { buffer in
            UsdShadeNodeGraph_GetInputs(handle, buffer.baseAddress, count, onlyAuthored)
        }

        var result = [Input]()
        result.reserveCapacity(actualCount)
        for i in 0..<actualCount {
            if let h = handles[i] {
                result.append(Input(handle: h))
            }
        }
        return result
    }

    // MARK: - Outputs

    /// Creates an output on the node graph.
    ///
    /// - Parameters:
    ///   - name: The name of the output.
    ///   - typeName: The type name of the output.
    /// - Returns: The created output.
    public func createOutput(name: Token, typeName: Token) -> Output? {
        guard let ref = UsdShadeNodeGraph_CreateOutput(handle, name.handle, typeName.handle) else {
            return nil
        }
        return Output(handle: ref)
    }

    /// Gets an output by name.
    ///
    /// - Parameter name: The name of the output.
    /// - Returns: The output, or `nil` if not found.
    public func output(named name: Token) -> Output? {
        guard let ref = UsdShadeNodeGraph_GetOutput(handle, name.handle) else {
            return nil
        }
        return Output(handle: ref)
    }

    /// Gets all outputs.
    ///
    /// - Parameter onlyAuthored: If true, only returns authored outputs.
    /// - Returns: Array of outputs.
    public func outputs(onlyAuthored: Bool = true) -> [Output] {
        let count = UsdShadeNodeGraph_GetOutputCount(handle, onlyAuthored)
        guard count > 0 else { return [] }

        var handles = [UsdShadeOutputRef?](repeating: nil, count: count)
        let actualCount = handles.withUnsafeMutableBufferPointer { buffer in
            UsdShadeNodeGraph_GetOutputs(handle, buffer.baseAddress, count, onlyAuthored)
        }

        var result = [Output]()
        result.reserveCapacity(actualCount)
        for i in 0..<actualCount {
            if let h = handles[i] {
                result.append(Output(handle: h))
            }
        }
        return result
    }

    // MARK: - Computations

    /// Computes the interface input consumers map.
    ///
    /// - Parameter computeTransitiveConsumers: Whether to compute transitive consumers.
    /// - Throws: `ShadeError.operationFailed` if the operation fails.
    public func computeInterfaceInputConsumersMap(computeTransitiveConsumers: Bool = false) throws {
        let result = UsdShadeNodeGraph_ComputeInterfaceInputConsumersMap(
            handle,
            computeTransitiveConsumers
        )
        guard result == USD_RESULT_SUCCESS else {
            throw ShadeError.operationFailed("Failed to compute interface input consumers map")
        }
    }
}

// MARK: - CustomStringConvertible

extension NodeGraph: CustomStringConvertible {

    public var description: String {
        path?.description ?? "<invalid node graph>"
    }
}

// MARK: - CustomDebugStringConvertible

extension NodeGraph: CustomDebugStringConvertible {

    public var debugDescription: String {
        "NodeGraph(\"\(path?.description ?? "<invalid>")\")"
    }
}
