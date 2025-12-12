/* ----------------------------------------------------------------
 *  A T H E M
 * ----------------------------------------------------------------
 *  Copyright (C) 2016 Pixar.
 *  Copyright (C) 2025 Afloat Technologies. All Rights Reserved.
 *  Licensed under https://openusd.org/license
 * ---------------------------------------------------------------- */

import Exec

// MARK: - Computation Context

public extension Exec
{
  /**
   * # ``Exec/ComputationContext``
   *
   * Context passed to custom computation callbacks.
   *
   * ## Overview
   *
   * `ComputationContext` provides access to input values and allows setting
   * the output value within a custom computation callback. It wraps the
   * underlying C++ `VdfContext`.
   *
   * ## Important
   *
   * - Do not store the context or use it after the callback returns
   * - Do not call into other execution system methods from within the callback
   * - All input/output access must happen synchronously within the callback
   *
   * ## Example Usage
   *
   * ```swift
   * Exec.registerPrimComputation(
   *     name: "computeScaledRadius",
   *     schema: "UsdGeomSphere",
   *     resultType: .double,
   *     inputs: [.attributeValue("radius", as: Double.self)]
   * ) { ctx in
   *     let radius = ctx.getInputDouble(name: "radius")
   *     ctx.setOutput(radius * 2.0)
   * }
   * ```
   */
  final class ComputationContext
  {
    /// The underlying C++ context handle.
    internal let _handle: UnsafeMutableRawPointer

    internal init(handle: UnsafeMutableRawPointer)
    {
      _handle = handle
    }

    // MARK: - Input Access

    /// Returns true if the input named `name` has a valid value.
    public func hasInputValue(name: String) -> Bool
    {
      name.withCString { cstr in
        Pixar.Exec_Swift_Context_HasInputValue(_handle, cstr)
      }
    }

    /// Gets an input value as `VtValue`.
    ///
    /// - Parameter name: The input name.
    /// - Returns: The input value, or an empty `VtValue` if not found.
    public func getInputValue(name: String) -> Pixar.VtValue
    {
      name.withCString { cstr in
        Pixar.Exec_Swift_Context_GetInputValue(_handle, cstr)
      }
    }

    // MARK: - Typed Input Getters

    /// Gets an input value as `Double`.
    ///
    /// - Parameter name: The input name.
    /// - Returns: The input value, or 0.0 if not found.
    public func getInputDouble(name: String) -> Double
    {
      name.withCString { cstr in
        Pixar.Exec_Swift_Context_GetInputDouble(_handle, cstr)
      }
    }

    /// Gets an input value as `Float`.
    ///
    /// - Parameter name: The input name.
    /// - Returns: The input value, or 0.0 if not found.
    public func getInputFloat(name: String) -> Float
    {
      name.withCString { cstr in
        Pixar.Exec_Swift_Context_GetInputFloat(_handle, cstr)
      }
    }

    /// Gets an input value as `Int`.
    ///
    /// - Parameter name: The input name.
    /// - Returns: The input value, or 0 if not found.
    public func getInputInt(name: String) -> Int
    {
      name.withCString { cstr in
        Int(Pixar.Exec_Swift_Context_GetInputInt(_handle, cstr))
      }
    }

    /// Gets an input value as `Bool`.
    ///
    /// - Parameter name: The input name.
    /// - Returns: The input value, or false if not found.
    public func getInputBool(name: String) -> Bool
    {
      name.withCString { cstr in
        Pixar.Exec_Swift_Context_GetInputBool(_handle, cstr)
      }
    }

    /// Gets an input value as `GfMatrix4d`.
    ///
    /// - Parameter name: The input name.
    /// - Returns: The input value, or identity matrix if not found.
    public func getInputMatrix4d(name: String) -> Pixar.GfMatrix4d
    {
      name.withCString { cstr in
        Pixar.Exec_Swift_Context_GetInputMatrix4d(_handle, cstr)
      }
    }

    /// Gets an input value as `GfMatrix4f`.
    ///
    /// - Parameter name: The input name.
    /// - Returns: The input value, or identity matrix if not found.
    public func getInputMatrix4f(name: String) -> Pixar.GfMatrix4f
    {
      name.withCString { cstr in
        Pixar.Exec_Swift_Context_GetInputMatrix4f(_handle, cstr)
      }
    }

    /// Gets an input value as `GfVec2d`.
    public func getInputVec2d(name: String) -> Pixar.GfVec2d
    {
      name.withCString { cstr in
        Pixar.Exec_Swift_Context_GetInputVec2d(_handle, cstr)
      }
    }

    /// Gets an input value as `GfVec2f`.
    public func getInputVec2f(name: String) -> Pixar.GfVec2f
    {
      name.withCString { cstr in
        Pixar.Exec_Swift_Context_GetInputVec2f(_handle, cstr)
      }
    }

    /// Gets an input value as `GfVec3d`.
    public func getInputVec3d(name: String) -> Pixar.GfVec3d
    {
      name.withCString { cstr in
        Pixar.Exec_Swift_Context_GetInputVec3d(_handle, cstr)
      }
    }

    /// Gets an input value as `GfVec3f`.
    public func getInputVec3f(name: String) -> Pixar.GfVec3f
    {
      name.withCString { cstr in
        Pixar.Exec_Swift_Context_GetInputVec3f(_handle, cstr)
      }
    }

    /// Gets an input value as `GfVec4d`.
    public func getInputVec4d(name: String) -> Pixar.GfVec4d
    {
      name.withCString { cstr in
        Pixar.Exec_Swift_Context_GetInputVec4d(_handle, cstr)
      }
    }

    /// Gets an input value as `GfVec4f`.
    public func getInputVec4f(name: String) -> Pixar.GfVec4f
    {
      name.withCString { cstr in
        Pixar.Exec_Swift_Context_GetInputVec4f(_handle, cstr)
      }
    }

    /// Gets an input value as `GfQuatd`.
    public func getInputQuatd(name: String) -> Pixar.GfQuatd
    {
      name.withCString { cstr in
        Pixar.Exec_Swift_Context_GetInputQuatd(_handle, cstr)
      }
    }

    /// Gets an input value as `GfQuatf`.
    public func getInputQuatf(name: String) -> Pixar.GfQuatf
    {
      name.withCString { cstr in
        Pixar.Exec_Swift_Context_GetInputQuatf(_handle, cstr)
      }
    }

    // MARK: - Output Setting

    /// Sets the output value from a `VtValue`.
    public func setOutput(_ value: Pixar.VtValue)
    {
      Pixar.Exec_Swift_Context_SetOutput(_handle, value)
    }

    /// Sets the output value to a `Double`.
    public func setOutput(_ value: Double)
    {
      Pixar.Exec_Swift_Context_SetOutputDouble(_handle, value)
    }

    /// Sets the output value to a `Float`.
    public func setOutput(_ value: Float)
    {
      Pixar.Exec_Swift_Context_SetOutputFloat(_handle, value)
    }

    /// Sets the output value to an `Int`.
    public func setOutput(_ value: Int)
    {
      Pixar.Exec_Swift_Context_SetOutputInt(_handle, Int32(value))
    }

    /// Sets the output value to a `Bool`.
    public func setOutput(_ value: Bool)
    {
      Pixar.Exec_Swift_Context_SetOutputBool(_handle, value)
    }

    /// Sets the output value to a `GfMatrix4d`.
    public func setOutput(_ value: Pixar.GfMatrix4d)
    {
      Pixar.Exec_Swift_Context_SetOutputMatrix4d(_handle, value)
    }

    /// Sets the output value to a `GfMatrix4f`.
    public func setOutput(_ value: Pixar.GfMatrix4f)
    {
      Pixar.Exec_Swift_Context_SetOutputMatrix4f(_handle, value)
    }

    /// Sets the output value to a `GfVec2d`.
    public func setOutput(_ value: Pixar.GfVec2d)
    {
      Pixar.Exec_Swift_Context_SetOutputVec2d(_handle, value)
    }

    /// Sets the output value to a `GfVec2f`.
    public func setOutput(_ value: Pixar.GfVec2f)
    {
      Pixar.Exec_Swift_Context_SetOutputVec2f(_handle, value)
    }

    /// Sets the output value to a `GfVec3d`.
    public func setOutput(_ value: Pixar.GfVec3d)
    {
      Pixar.Exec_Swift_Context_SetOutputVec3d(_handle, value)
    }

    /// Sets the output value to a `GfVec3f`.
    public func setOutput(_ value: Pixar.GfVec3f)
    {
      Pixar.Exec_Swift_Context_SetOutputVec3f(_handle, value)
    }

    /// Sets the output value to a `GfVec4d`.
    public func setOutput(_ value: Pixar.GfVec4d)
    {
      Pixar.Exec_Swift_Context_SetOutputVec4d(_handle, value)
    }

    /// Sets the output value to a `GfVec4f`.
    public func setOutput(_ value: Pixar.GfVec4f)
    {
      Pixar.Exec_Swift_Context_SetOutputVec4f(_handle, value)
    }

    /// Sets the output value to a `GfQuatd`.
    public func setOutput(_ value: Pixar.GfQuatd)
    {
      Pixar.Exec_Swift_Context_SetOutputQuatd(_handle, value)
    }

    /// Sets the output value to a `GfQuatf`.
    public func setOutput(_ value: Pixar.GfQuatf)
    {
      Pixar.Exec_Swift_Context_SetOutputQuatf(_handle, value)
    }
  }
}

// MARK: - Computation Type

public extension Exec
{
  /// Supported computation result/input value types.
  enum ComputationType: String
  {
    case double
    case float
    case int
    case bool
    case matrix4d = "GfMatrix4d"
    case matrix4f = "GfMatrix4f"
    case vec2d = "GfVec2d"
    case vec2f = "GfVec2f"
    case vec3d = "GfVec3d"
    case vec3f = "GfVec3f"
    case vec4d = "GfVec4d"
    case vec4f = "GfVec4f"
    case quatd = "GfQuatd"
    case quatf = "GfQuatf"
  }

  /// How to traverse to find an input provider.
  enum InputTraversal: Int32
  {
    /// The local traversal path directly indicates the provider.
    case local = 0
    /// Find providers by traversing relationship targets.
    case relationshipTargets = 1
    /// Find providers by traversing attribute connections.
    case connectionTargets = 2
    /// Find the provider by traversing upward in namespace.
    case namespaceAncestor = 3
  }
}

// MARK: - Input Specification

public extension Exec
{
  /**
   * # ``Exec/InputSpec``
   *
   * Specifies how to find an input value for a computation.
   *
   * ## Overview
   *
   * Input specifications tell the execution system how to find the input
   * values needed by a computation. Each input spec includes:
   * - The name to access the input by in the callback
   * - The computation to request on the provider
   * - The expected result type
   * - How to find the provider (local, ancestor, etc.)
   *
   * ## Factory Methods
   *
   * Use the factory methods to create common input patterns:
   * - `attributeValue(_:as:)` - Read an attribute's value
   * - `computation(_:as:)` - Use another computation's result
   * - `namespaceAncestor(_:as:)` - Find computation on ancestor prim
   */
  struct InputSpec
  {
    /// The name used to access this input in the callback.
    public var inputName: String

    /// The computation name to request on the provider.
    public var computationName: String

    /// A token for distinguishing computations with the same name.
    public var disambiguatingId: String

    /// The expected result type.
    public var resultType: ComputationType

    /// A path relative to the owner describing traversal to provider.
    public var localTraversalPath: String

    /// How to dynamically traverse to find the provider.
    public var traversal: InputTraversal

    /// Whether to fall back to dispatched computations.
    public var fallsBackToDispatched: Bool

    /// Whether the input is optional.
    public var isOptional: Bool

    /// Creates an input spec with full configuration.
    public init(
      inputName: String,
      computationName: String,
      disambiguatingId: String = "",
      resultType: ComputationType,
      localTraversalPath: String = "",
      traversal: InputTraversal = .local,
      fallsBackToDispatched: Bool = false,
      isOptional: Bool = true
    )
    {
      self.inputName = inputName
      self.computationName = computationName
      self.disambiguatingId = disambiguatingId
      self.resultType = resultType
      self.localTraversalPath = localTraversalPath
      self.traversal = traversal
      self.fallsBackToDispatched = fallsBackToDispatched
      self.isOptional = isOptional
    }

    // MARK: - Factory Methods

    /// Creates an input spec for reading an attribute's value.
    ///
    /// - Parameters:
    ///   - attributeName: The attribute name (e.g., "radius").
    ///   - type: The expected value type.
    /// - Returns: An input spec configured for attribute value access.
    public static func attributeValue(
      _ attributeName: String,
      as type: ComputationType
    ) -> InputSpec
    {
      InputSpec(
        inputName: attributeName,
        computationName: Exec.Tokens.computeValue.string,
        resultType: type,
        localTraversalPath: ".\(attributeName)",
        traversal: .local
      )
    }

    /// Creates an input spec for using another computation's result.
    ///
    /// - Parameters:
    ///   - computationName: The computation name.
    ///   - type: The expected result type.
    /// - Returns: An input spec configured for computation access.
    public static func computation(
      _ computationName: String,
      as type: ComputationType
    ) -> InputSpec
    {
      InputSpec(
        inputName: computationName,
        computationName: computationName,
        resultType: type,
        traversal: .local
      )
    }

    /// Creates an input spec for finding a computation on a namespace ancestor.
    ///
    /// - Parameters:
    ///   - computationName: The computation name to find on ancestors.
    ///   - type: The expected result type.
    /// - Returns: An input spec configured for namespace ancestor traversal.
    public static func namespaceAncestor(
      _ computationName: String,
      as type: ComputationType
    ) -> InputSpec
    {
      InputSpec(
        inputName: computationName,
        computationName: computationName,
        resultType: type,
        traversal: .namespaceAncestor
      )
    }

    /// Makes this input required (non-optional).
    public func required() -> InputSpec
    {
      var copy = self
      copy.isOptional = false
      return copy
    }

    /// Makes this input fall back to dispatched computations if local not found.
    public func withDispatchFallback() -> InputSpec
    {
      var copy = self
      copy.fallsBackToDispatched = true
      return copy
    }
  }
}

// MARK: - Callback Box

/// Internal class to box Swift closures for C callback context.
private final class ComputationCallbackBox
{
  let callback: (Exec.ComputationContext) -> Void

  init(callback: @escaping (Exec.ComputationContext) -> Void)
  {
    self.callback = callback
  }
}

/// Trampoline function called from C++.
private func computationTrampoline(
  contextHandle: UnsafeMutableRawPointer?,
  userContext: UnsafeMutableRawPointer?
)
{
  guard let contextHandle = contextHandle, let userContext = userContext else { return }

  let box = Unmanaged<ComputationCallbackBox>.fromOpaque(userContext).takeUnretainedValue()
  let context = Exec.ComputationContext(handle: contextHandle)
  box.callback(context)
}

// MARK: - Registration API

public extension Exec
{
  /// Callback type for custom computations.
  typealias ComputationCallback = (ComputationContext) -> Void

  /**
   * Registers a custom prim computation.
   *
   * ## Overview
   *
   * Registers a computation that can be requested on prims of the specified
   * schema type. The callback will be invoked during execution with a context
   * that provides access to input values.
   *
   * ## Important
   *
   * - Must be called after `Pixar.Bundler.shared.setup(.resources)`
   * - Must be called before creating any `ExecUsd.System` instances
   * - The callback must not call into the execution system
   * - The callback must be thread-safe
   *
   * ## Example
   *
   * ```swift
   * Exec.registerPrimComputation(
   *     name: "computeScaledRadius",
   *     schema: "UsdGeomSphere",
   *     resultType: .double,
   *     inputs: [
   *         .attributeValue("radius", as: .double)
   *     ]
   * ) { ctx in
   *     let radius = ctx.getInputDouble(name: "radius")
   *     ctx.setOutput(radius * 2.0)
   * }
   * ```
   *
   * - Parameters:
   *   - name: The computation name (e.g., "computeMyValue").
   *   - schema: The schema type name (e.g., "UsdGeomSphere").
   *   - resultType: The computation result type.
   *   - inputs: Input specifications for the computation.
   *   - callback: The computation implementation.
   * - Returns: `true` on success, `false` on failure.
   */
  @discardableResult
  static func registerPrimComputation(
    name: String,
    schema: String,
    resultType: ComputationType,
    inputs: [InputSpec] = [],
    callback: @escaping ComputationCallback
  ) -> Bool
  {
    _registerComputation(
      name: name,
      schema: schema,
      resultType: resultType,
      inputs: inputs,
      callback: callback,
      isPrimComputation: true,
      attributeName: nil
    )
  }

  /**
   * Registers a custom attribute computation.
   *
   * ## Overview
   *
   * Registers a computation that can be requested on attributes of the
   * specified name on prims of the specified schema type.
   *
   * - Parameters:
   *   - name: The computation name.
   *   - attributeName: The attribute this computation applies to.
   *   - schema: The schema type name.
   *   - resultType: The computation result type.
   *   - inputs: Input specifications for the computation.
   *   - callback: The computation implementation.
   * - Returns: `true` on success, `false` on failure.
   */
  @discardableResult
  static func registerAttributeComputation(
    name: String,
    attributeName: String,
    schema: String,
    resultType: ComputationType,
    inputs: [InputSpec] = [],
    callback: @escaping ComputationCallback
  ) -> Bool
  {
    _registerComputation(
      name: name,
      schema: schema,
      resultType: resultType,
      inputs: inputs,
      callback: callback,
      isPrimComputation: false,
      attributeName: attributeName
    )
  }
}

// MARK: - Internal Implementation

private func _registerComputation(
  name: String,
  schema: String,
  resultType: Exec.ComputationType,
  inputs: [Exec.InputSpec],
  callback: @escaping Exec.ComputationCallback,
  isPrimComputation: Bool,
  attributeName: String?
) -> Bool
{
  // Box the Swift closure
  let boxedCallback = ComputationCallbackBox(callback: callback)
  let callbackContext = Unmanaged.passRetained(boxedCallback).toOpaque()

  // Build C input specs array
  var cInputSpecs: [Pixar.Exec_Swift_InputSpec] = []
  var inputStrings: [[CChar]] = [] // Keep strings alive

  for input in inputs
  {
    // Convert strings to C strings and keep them alive
    let inputNameChars = Array(input.inputName.utf8CString)
    let computationNameChars = Array(input.computationName.utf8CString)
    let disambiguatingIdChars = Array(input.disambiguatingId.utf8CString)
    let resultTypeChars = Array(input.resultType.rawValue.utf8CString)
    let localPathChars = Array(input.localTraversalPath.utf8CString)

    inputStrings.append(inputNameChars)
    inputStrings.append(computationNameChars)
    inputStrings.append(disambiguatingIdChars)
    inputStrings.append(resultTypeChars)
    inputStrings.append(localPathChars)

    var cSpec = Pixar.Exec_Swift_InputSpec()
    cSpec.dynamicTraversal = Pixar.Exec_Swift_DynamicTraversal(rawValue: UInt32(input.traversal.rawValue))
    cSpec.fallsBackToDispatched = input.fallsBackToDispatched
    cSpec.optional = input.isOptional

    cInputSpecs.append(cSpec)
  }

  // Now set pointers (must be done after all strings are added to keep them alive)
  var stringIndex = 0
  for i in 0 ..< cInputSpecs.count
  {
    cInputSpecs[i].inputName = inputStrings[stringIndex].withUnsafeBufferPointer { $0.baseAddress }
    stringIndex += 1
    cInputSpecs[i].computationName = inputStrings[stringIndex].withUnsafeBufferPointer { $0.baseAddress }
    stringIndex += 1
    cInputSpecs[i].disambiguatingId = inputStrings[stringIndex].withUnsafeBufferPointer { $0.baseAddress }
    stringIndex += 1
    cInputSpecs[i].resultTypeName = inputStrings[stringIndex].withUnsafeBufferPointer { $0.baseAddress }
    stringIndex += 1
    cInputSpecs[i].localTraversalPath = inputStrings[stringIndex].withUnsafeBufferPointer { $0.baseAddress }
    stringIndex += 1
  }

  // Build C computation spec
  let result = name.withCString { nameCStr in
    schema.withCString { schemaCStr in
      resultType.rawValue.withCString { resultTypeCStr in
        cInputSpecs.withUnsafeBufferPointer { inputsPtr in
          var spec = Pixar.Exec_Swift_ComputationSpec()
          spec.schemaTypeName = schemaCStr
          spec.computationName = nameCStr
          spec.resultTypeName = resultTypeCStr
          spec.callback = computationTrampoline
          spec.callbackContext = callbackContext
          spec.inputs = inputsPtr.baseAddress
          spec.inputCount = inputsPtr.count
          spec.isPrimComputation = isPrimComputation

          if let attrName = attributeName
          {
            return attrName.withCString { attrCStr in
              spec.attributeName = attrCStr
              return Pixar.Exec_Swift_RegisterComputation(&spec)
            }
          }
          else
          {
            spec.attributeName = nil
            return Pixar.Exec_Swift_RegisterComputation(&spec)
          }
        }
      }
    }
  }

  if !result
  {
    // Release the retained callback on failure
    Unmanaged<ComputationCallbackBox>.fromOpaque(callbackContext).release()
  }
  // Note: On success, ownership is transferred to C++ (lives for program duration)

  return result
}
