// MaterialBindingAPI.swift - UsdShadeMaterialBindingAPI wrapper
// Mirrors: pxr/usd/usdShade/materialBindingAPI.h

import OpenUSDInterop

// MARK: - BindingStrength

/// The strength of a material binding.
public enum BindingStrength: Sendable {
    /// Weak binding that can be overridden by descendants.
    case fallback
    /// Strong binding that overrides fallback bindings.
    case stronger

    internal var cValue: UsdShadeBindingStrength {
        switch self {
        case .fallback: return USDSHADE_BINDING_STRENGTH_FALLBACK
        case .stronger: return USDSHADE_BINDING_STRENGTH_STRONGER
        }
    }

    internal init(cValue: UsdShadeBindingStrength) {
        switch cValue {
        case USDSHADE_BINDING_STRENGTH_STRONGER, USDSHADE_BINDING_STRENGTH_STRONG_THAN_DESCENDANTS:
            self = .stronger
        default:
            self = .fallback
        }
    }
}

// MARK: - MaterialPurpose

/// The purpose of a material binding.
public enum MaterialPurpose: Sendable {
    /// All-purpose binding (default).
    case all
    /// Preview render binding.
    case preview
    /// Full render binding.
    case full

    internal var cValue: UsdShadeMaterialPurpose {
        switch self {
        case .all: return USDSHADE_PURPOSE_ALL
        case .preview: return USDSHADE_PURPOSE_PREVIEW
        case .full: return USDSHADE_PURPOSE_FULL
        }
    }
}

// MARK: - MaterialBindingAPI

/// API for binding materials to geometry.
///
/// The MaterialBindingAPI is used to bind materials to geometry prims.
/// It supports direct bindings and collection-based bindings.
///
/// Mirrors `pxr::UsdShadeMaterialBindingAPI` from the USD C++ API.
public final class MaterialBindingAPI: @unchecked Sendable {

    // MARK: - Properties

    /// The underlying opaque pointer to the C wrapper.
    internal let handle: UsdShadeMaterialBindingAPIRef

    // MARK: - Initialization

    /// Internal initializer from an existing handle.
    /// Takes ownership of the handle (does not retain).
    internal init(handle: UsdShadeMaterialBindingAPIRef) {
        self.handle = handle
    }

    deinit {
        UsdShadeMaterialBindingAPI_Release(handle)
    }

    // MARK: - Static Factory Methods

    /// Applies the MaterialBindingAPI to a prim.
    ///
    /// - Parameter prim: The prim to apply the API to.
    /// - Returns: The API, or `nil` if application fails.
    public static func apply(to prim: Prim) -> MaterialBindingAPI? {
        guard let ref = UsdShadeMaterialBindingAPI_Apply(prim.handle) else {
            return nil
        }
        return MaterialBindingAPI(handle: ref)
    }

    /// Gets the MaterialBindingAPI from a prim.
    ///
    /// - Parameter prim: The prim to get the API from.
    /// - Returns: The API, or `nil` if not applied.
    public static func get(from prim: Prim) -> MaterialBindingAPI? {
        guard let ref = UsdShadeMaterialBindingAPI_Get(prim.handle) else {
            return nil
        }
        return MaterialBindingAPI(handle: ref)
    }

    // MARK: - Properties

    /// Whether the API is valid.
    public var isValid: Bool {
        UsdShadeMaterialBindingAPI_IsValid(handle)
    }

    /// The underlying prim.
    public var prim: Prim? {
        guard let ref = UsdShadeMaterialBindingAPI_GetPrim(handle) else {
            return nil
        }
        return Prim(handle: ref)
    }

    // MARK: - Direct Binding

    /// Binds a material to this prim.
    ///
    /// - Parameters:
    ///   - material: The material to bind.
    ///   - strength: The binding strength.
    ///   - purpose: The binding purpose.
    /// - Throws: `ShadeError.bindingFailed` if the binding fails.
    public func bind(
        _ material: Material,
        strength: BindingStrength = .fallback,
        purpose: MaterialPurpose = .all
    ) throws {
        let result = UsdShadeMaterialBindingAPI_Bind(
            handle,
            material.handle,
            strength.cValue,
            purpose.cValue
        )
        guard result == USD_RESULT_SUCCESS else {
            throw ShadeError.bindingFailed("Failed to bind material")
        }
    }

    /// Gets the directly bound material.
    ///
    /// - Parameter purpose: The binding purpose.
    /// - Returns: The bound material, or `nil` if none.
    public func directlyBoundMaterial(purpose: MaterialPurpose = .all) -> Material? {
        guard let ref = UsdShadeMaterialBindingAPI_GetDirectlyBoundMaterial(handle, purpose.cValue) else {
            return nil
        }
        return Material(handle: ref)
    }

    /// Gets the path of the directly bound material.
    ///
    /// - Parameter purpose: The binding purpose.
    /// - Returns: The path, or `nil` if none.
    public func directBindingPath(purpose: MaterialPurpose = .all) -> Path? {
        guard let ref = UsdShadeMaterialBindingAPI_GetDirectBindingPath(handle, purpose.cValue) else {
            return nil
        }
        return Path(handle: ref)
    }

    /// Unbinds the direct material binding.
    ///
    /// - Parameter purpose: The binding purpose.
    /// - Throws: `ShadeError.operationFailed` if the operation fails.
    public func unbindDirectBinding(purpose: MaterialPurpose = .all) throws {
        let result = UsdShadeMaterialBindingAPI_UnbindDirectBinding(handle, purpose.cValue)
        guard result == USD_RESULT_SUCCESS else {
            throw ShadeError.operationFailed("Failed to unbind direct binding")
        }
    }

    // MARK: - Computed Binding

    /// Computes the bound material considering inheritance.
    ///
    /// - Parameter purpose: The binding purpose.
    /// - Returns: The computed material, or `nil` if none.
    public func computeBoundMaterial(purpose: MaterialPurpose = .all) -> Material? {
        guard let ref = UsdShadeMaterialBindingAPI_ComputeBoundMaterial(handle, purpose.cValue) else {
            return nil
        }
        return Material(handle: ref)
    }

    /// Computes the bound material path considering inheritance.
    ///
    /// - Parameter purpose: The binding purpose.
    /// - Returns: The computed path, or `nil` if none.
    public func computeBoundMaterialPath(purpose: MaterialPurpose = .all) -> Path? {
        guard let ref = UsdShadeMaterialBindingAPI_ComputeBoundMaterialPath(handle, purpose.cValue) else {
            return nil
        }
        return Path(handle: ref)
    }

    // MARK: - Binding Strength

    /// Gets the binding strength of the direct binding.
    ///
    /// - Parameter purpose: The binding purpose.
    /// - Returns: The binding strength.
    public func directBindingStrength(purpose: MaterialPurpose = .all) -> BindingStrength {
        let cValue = UsdShadeMaterialBindingAPI_GetDirectBindingStrength(handle, purpose.cValue)
        return BindingStrength(cValue: cValue)
    }

    /// Sets the binding strength of the direct binding.
    ///
    /// - Parameters:
    ///   - strength: The binding strength.
    ///   - purpose: The binding purpose.
    /// - Throws: `ShadeError.operationFailed` if the operation fails.
    public func setDirectBindingStrength(
        _ strength: BindingStrength,
        purpose: MaterialPurpose = .all
    ) throws {
        let result = UsdShadeMaterialBindingAPI_SetDirectBindingStrength(
            handle,
            purpose.cValue,
            strength.cValue
        )
        guard result == USD_RESULT_SUCCESS else {
            throw ShadeError.operationFailed("Failed to set binding strength")
        }
    }
}

// MARK: - CustomStringConvertible

extension MaterialBindingAPI: CustomStringConvertible {

    public var description: String {
        if let p = prim {
            return "MaterialBindingAPI(\(p.path.description))"
        }
        return "<invalid MaterialBindingAPI>"
    }
}
