// UsdGeom.swift - Geometry Schemas - Idiomatic Swift API
//
// This module provides Swift wrappers around CxxFacade's UsdGeom types,
// including transform, mesh, curves, camera, and geometric primitives.
//
// The UsdGeom module is the primary API for working with geometry in USD,
// providing schemas for transforms, meshes, curves, cameras, and
// geometric primitives.

import CxxFacade

// MARK: - Type Aliases

// --- Transform Operations ---

/// UsdGeomXformOpType - Type of transform operation.
public typealias UsdGeomXformOpType = cxxfacade.usdGeom.UsdGeomXformOpType

/// UsdGeomXformOpPrecision - Precision of transform operation values.
public typealias UsdGeomXformOpPrecision = cxxfacade.usdGeom.UsdGeomXformOpPrecision

/// UsdGeomXformOp - A single transform operation.
///
/// UsdGeomXformOp represents a single transformation operation like
/// translate, rotate, scale, or a 4x4 matrix.
public typealias UsdGeomXformOp = cxxfacade.usdGeom.UsdGeomXformOp

// --- Schema Types ---

/// UsdGeomImageable - Base class for imageable prims.
///
/// UsdGeomImageable is the base class for all prims that can be
/// rendered. It provides visibility and purpose attributes.
public typealias UsdGeomImageable = cxxfacade.usdGeom.UsdGeomImageable

/// UsdGeomXformable - Base class for transformable prims.
///
/// UsdGeomXformable provides the interface for encoding transformation
/// operations on prims including translate, rotate, and scale.
public typealias UsdGeomXformable = cxxfacade.usdGeom.UsdGeomXformable

/// UsdGeomXform - A transform prim (grouping with transform).
///
/// UsdGeomXform is a concrete prim used for grouping and transforming
/// its children. It has no geometry of its own.
public typealias UsdGeomXform = cxxfacade.usdGeom.UsdGeomXform

/// UsdGeomScope - A grouping prim without transform.
///
/// UsdGeomScope is a container prim for grouping geometry without
/// any transform. Useful for organizing the scene hierarchy.
public typealias UsdGeomScope = cxxfacade.usdGeom.UsdGeomScope

/// UsdGeomBoundable - Base class for prims with extent.
///
/// UsdGeomBoundable is the base class for prims that have a computable
/// bounding box via an "extent" attribute.
public typealias UsdGeomBoundable = cxxfacade.usdGeom.UsdGeomBoundable

/// UsdGeomGprim - Base class for geometric primitives.
///
/// UsdGeomGprim is the base class for all geometric primitives,
/// providing attributes like doubleSided, orientation, and displayColor.
public typealias UsdGeomGprim = cxxfacade.usdGeom.UsdGeomGprim

/// UsdGeomPointBased - Base class for point-based geometry.
///
/// UsdGeomPointBased is the base for meshes, points, and curves.
/// It provides points, normals, and velocities attributes.
public typealias UsdGeomPointBased = cxxfacade.usdGeom.UsdGeomPointBased

/// UsdGeomMesh - Polygon mesh geometry.
///
/// UsdGeomMesh encodes polygon meshes with optional subdivision
/// properties including Catmull-Clark, Loop, and bilinear schemes.
///
/// ## Usage
/// ```swift
/// var mesh = UsdGeomMesh.Define(&stage, SdfPath("/World/Mesh"))
///
/// // Set points
/// let pointsAttr = mesh.CreatePointsAttr()
///
/// // Set face topology
/// let faceCountsAttr = mesh.CreateFaceVertexCountsAttr()
/// let faceIndicesAttr = mesh.CreateFaceVertexIndicesAttr()
/// ```
public typealias UsdGeomMesh = cxxfacade.usdGeom.UsdGeomMesh

/// UsdGeomPoints - Point cloud geometry.
///
/// UsdGeomPoints represents a point cloud with per-point attributes
/// like widths and ids.
public typealias UsdGeomPoints = cxxfacade.usdGeom.UsdGeomPoints

/// UsdGeomCurves - Abstract base for curve types.
///
/// UsdGeomCurves is the base class for all curve types including
/// basis curves and NURBS curves.
public typealias UsdGeomCurves = cxxfacade.usdGeom.UsdGeomCurves

/// UsdGeomBasisCurves - Cubic basis curves.
///
/// UsdGeomBasisCurves represents curves using a basis like
/// Bezier, B-spline, or Catmull-Rom.
public typealias UsdGeomBasisCurves = cxxfacade.usdGeom.UsdGeomBasisCurves

/// UsdGeomNurbsCurves - NURBS curves.
///
/// UsdGeomNurbsCurves represents Non-Uniform Rational B-Spline curves.
public typealias UsdGeomNurbsCurves = cxxfacade.usdGeom.UsdGeomNurbsCurves

/// UsdGeomCamera - Camera for scene viewing.
///
/// UsdGeomCamera provides camera parameters including projection,
/// aperture, focal length, clipping, and focus properties.
public typealias UsdGeomCamera = cxxfacade.usdGeom.UsdGeomCamera

// --- Geometric Primitives ---

/// UsdGeomCube - A cube centered at the origin.
public typealias UsdGeomCube = cxxfacade.usdGeom.UsdGeomCube

/// UsdGeomSphere - A sphere centered at the origin.
public typealias UsdGeomSphere = cxxfacade.usdGeom.UsdGeomSphere

/// UsdGeomCylinder - A cylinder along an axis.
public typealias UsdGeomCylinder = cxxfacade.usdGeom.UsdGeomCylinder

/// UsdGeomCapsule - A capsule (cylinder with hemispherical ends).
public typealias UsdGeomCapsule = cxxfacade.usdGeom.UsdGeomCapsule

/// UsdGeomCone - A cone along an axis.
public typealias UsdGeomCone = cxxfacade.usdGeom.UsdGeomCone

// --- Primvars ---

/// UsdGeomPrimvarInterpolation - Interpolation type for primvars.
public typealias UsdGeomPrimvarInterpolation = cxxfacade.usdGeom.UsdGeomPrimvarInterpolation

/// UsdGeomPrimvar - A primvar (primitive variable) attribute.
///
/// UsdGeomPrimvar wraps a UsdAttribute to provide primvar-specific
/// APIs like interpolation and element size.
public typealias UsdGeomPrimvar = cxxfacade.usdGeom.UsdGeomPrimvar

/// UsdGeomPrimvarsAPI - API for creating and managing primvars.
///
/// UsdGeomPrimvarsAPI provides an interface for creating, getting,
/// and managing primvars on a prim.
public typealias UsdGeomPrimvarsAPI = cxxfacade.usdGeom.UsdGeomPrimvarsAPI

// --- Tokens ---

/// UsdGeomTokens - Common tokens used in UsdGeom.
public typealias UsdGeomTokens = cxxfacade.usdGeom.UsdGeomTokens

// MARK: - UsdGeomXformOp Extensions

extension UsdGeomXformOp {
    /// Whether this is a valid xform op.
    public var isValid: Bool {
        return self.IsValid()
    }

    /// The type of this operation.
    public var opType: UsdGeomXformOpType {
        return self.GetOpType()
    }

    /// The precision of this operation.
    public var precision: UsdGeomXformOpPrecision {
        return self.GetPrecision()
    }

    /// Whether this is an inverse operation.
    public var isInverse: Bool {
        return self.IsInverseOp()
    }

    /// The underlying attribute.
    public var attr: UsdAttribute {
        return self.GetAttr()
    }
}

// MARK: - UsdGeomImageable Extensions

extension UsdGeomImageable {
    /// Get visibility attribute.
    public var visibilityAttr: UsdAttribute {
        return self.GetVisibilityAttr()
    }

    /// Get purpose attribute.
    public var purposeAttr: UsdAttribute {
        return self.GetPurposeAttr()
    }

    /// Compute visibility at the given time.
    public func computeVisibility(at time: UsdTimeCode = UsdTimeCode.Default()) -> TfToken {
        return self.ComputeVisibility(time)
    }

    /// Compute purpose for this prim.
    public func computePurpose() -> TfToken {
        return self.ComputePurpose()
    }

    /// Make this prim invisible.
    public mutating func makeInvisible(at time: UsdTimeCode = UsdTimeCode.Default()) {
        self.MakeInvisible(time)
    }

    /// Make this prim visible.
    public mutating func makeVisible(at time: UsdTimeCode = UsdTimeCode.Default()) {
        self.MakeVisible(time)
    }
}

// MARK: - UsdGeomXformable Extensions

extension UsdGeomXformable {
    /// Get the local transformation matrix at the given time.
    public func localTransformation(at time: UsdTimeCode = UsdTimeCode.Default()) -> Matrix4d? {
        var matrix = Matrix4d()
        var resetsStack = false
        if self.GetLocalTransformation(&matrix, &resetsStack, time) {
            return matrix
        }
        return nil
    }

    /// Whether the transform might be time-varying.
    public var transformMightBeTimeVarying: Bool {
        return self.TransformMightBeTimeVarying()
    }

    /// Clear all transform ops.
    @discardableResult
    public mutating func clearXformOpOrder() -> Bool {
        return self.ClearXformOpOrder()
    }

    /// Create a single matrix transform op, clearing any existing ops.
    @discardableResult
    public mutating func makeMatrixXform() -> UsdGeomXformOp {
        return self.MakeMatrixXform()
    }

    /// Add a translate operation.
    @discardableResult
    public mutating func addTranslateOp(
        precision: UsdGeomXformOpPrecision = .PrecisionDouble
    ) -> UsdGeomXformOp {
        return self.AddTranslateOp(precision, TfToken(), false)
    }

    /// Add a scale operation.
    @discardableResult
    public mutating func addScaleOp(
        precision: UsdGeomXformOpPrecision = .PrecisionFloat
    ) -> UsdGeomXformOp {
        return self.AddScaleOp(precision, TfToken(), false)
    }

    /// Add a rotate X operation.
    @discardableResult
    public mutating func addRotateXOp(
        precision: UsdGeomXformOpPrecision = .PrecisionFloat
    ) -> UsdGeomXformOp {
        return self.AddRotateXOp(precision, TfToken(), false)
    }

    /// Add a rotate Y operation.
    @discardableResult
    public mutating func addRotateYOp(
        precision: UsdGeomXformOpPrecision = .PrecisionFloat
    ) -> UsdGeomXformOp {
        return self.AddRotateYOp(precision, TfToken(), false)
    }

    /// Add a rotate Z operation.
    @discardableResult
    public mutating func addRotateZOp(
        precision: UsdGeomXformOpPrecision = .PrecisionFloat
    ) -> UsdGeomXformOp {
        return self.AddRotateZOp(precision, TfToken(), false)
    }

    /// Add a rotate XYZ operation (Euler angles).
    @discardableResult
    public mutating func addRotateXYZOp(
        precision: UsdGeomXformOpPrecision = .PrecisionFloat
    ) -> UsdGeomXformOp {
        return self.AddRotateXYZOp(precision, TfToken(), false)
    }

    /// Add an orient (quaternion) operation.
    @discardableResult
    public mutating func addOrientOp(
        precision: UsdGeomXformOpPrecision = .PrecisionFloat
    ) -> UsdGeomXformOp {
        return self.AddOrientOp(precision, TfToken(), false)
    }

    /// Add a 4x4 transform matrix operation.
    @discardableResult
    public mutating func addTransformOp(
        precision: UsdGeomXformOpPrecision = .PrecisionDouble
    ) -> UsdGeomXformOp {
        return self.AddTransformOp(precision, TfToken(), false)
    }
}

// MARK: - UsdGeomXform Extensions

extension UsdGeomXform {
    /// Define a UsdGeomXform at the given path.
    public static func define(_ stage: inout UsdStage, path: SdfPath) -> UsdGeomXform {
        return UsdGeomXform.Define(&stage, path)
    }

    /// Define a UsdGeomXform at the given path string.
    public static func define(_ stage: inout UsdStage, path: String) -> UsdGeomXform {
        return UsdGeomXform.Define(&stage, SdfPath(path))
    }
}

// MARK: - UsdGeomScope Extensions

extension UsdGeomScope {
    /// Define a UsdGeomScope at the given path.
    public static func define(_ stage: inout UsdStage, path: SdfPath) -> UsdGeomScope {
        return UsdGeomScope.Define(&stage, path)
    }

    /// Define a UsdGeomScope at the given path string.
    public static func define(_ stage: inout UsdStage, path: String) -> UsdGeomScope {
        return UsdGeomScope.Define(&stage, SdfPath(path))
    }
}

// MARK: - UsdGeomBoundable Extensions

extension UsdGeomBoundable {
    /// The extent attribute.
    public var extentAttr: UsdAttribute {
        return self.GetExtentAttr()
    }

    /// Create the extent attribute.
    public mutating func createExtentAttr() -> UsdAttribute {
        return self.CreateExtentAttr()
    }
}

// MARK: - UsdGeomGprim Extensions

extension UsdGeomGprim {
    /// The doubleSided attribute.
    public var doubleSidedAttr: UsdAttribute {
        return self.GetDoubleSidedAttr()
    }

    /// The orientation attribute.
    public var orientationAttr: UsdAttribute {
        return self.GetOrientationAttr()
    }

    /// The displayColor primvar.
    public var displayColorAttr: UsdAttribute {
        return self.GetDisplayColorAttr()
    }

    /// The displayOpacity primvar.
    public var displayOpacityAttr: UsdAttribute {
        return self.GetDisplayOpacityAttr()
    }
}

// MARK: - UsdGeomPointBased Extensions

extension UsdGeomPointBased {
    /// The points attribute.
    public var pointsAttr: UsdAttribute {
        return self.GetPointsAttr()
    }

    /// Create the points attribute.
    public mutating func createPointsAttr() -> UsdAttribute {
        return self.CreatePointsAttr()
    }

    /// The normals attribute.
    public var normalsAttr: UsdAttribute {
        return self.GetNormalsAttr()
    }

    /// Create the normals attribute.
    public mutating func createNormalsAttr() -> UsdAttribute {
        return self.CreateNormalsAttr()
    }

    /// The velocities attribute.
    public var velocitiesAttr: UsdAttribute {
        return self.GetVelocitiesAttr()
    }

    /// Create the velocities attribute.
    public mutating func createVelocitiesAttr() -> UsdAttribute {
        return self.CreateVelocitiesAttr()
    }

    /// Get the interpolation of the normals attribute.
    public var normalsInterpolation: TfToken {
        return self.GetNormalsInterpolation()
    }
}

// MARK: - UsdGeomMesh Extensions

extension UsdGeomMesh {
    /// Define a UsdGeomMesh at the given path.
    public static func define(_ stage: inout UsdStage, path: SdfPath) -> UsdGeomMesh {
        return UsdGeomMesh.Define(&stage, path)
    }

    /// Define a UsdGeomMesh at the given path string.
    public static func define(_ stage: inout UsdStage, path: String) -> UsdGeomMesh {
        return UsdGeomMesh.Define(&stage, SdfPath(path))
    }

    /// The faceVertexIndices attribute.
    public var faceVertexIndicesAttr: UsdAttribute {
        return self.GetFaceVertexIndicesAttr()
    }

    /// Create the faceVertexIndices attribute.
    public mutating func createFaceVertexIndicesAttr() -> UsdAttribute {
        return self.CreateFaceVertexIndicesAttr()
    }

    /// The faceVertexCounts attribute.
    public var faceVertexCountsAttr: UsdAttribute {
        return self.GetFaceVertexCountsAttr()
    }

    /// Create the faceVertexCounts attribute.
    public mutating func createFaceVertexCountsAttr() -> UsdAttribute {
        return self.CreateFaceVertexCountsAttr()
    }

    /// The number of faces.
    public var faceCount: Int {
        return Int(self.GetFaceCount())
    }

    /// The subdivisionScheme attribute.
    public var subdivisionSchemeAttr: UsdAttribute {
        return self.GetSubdivisionSchemeAttr()
    }

    /// Create the subdivisionScheme attribute.
    public mutating func createSubdivisionSchemeAttr() -> UsdAttribute {
        return self.CreateSubdivisionSchemeAttr()
    }

    /// The infinite sharpness value for creases and corners.
    public static var sharpnessInfinite: Float {
        return UsdGeomMesh.GetSharpnessInfinite()
    }
}

// MARK: - UsdGeomPoints Extensions

extension UsdGeomPoints {
    /// Define a UsdGeomPoints at the given path.
    public static func define(_ stage: inout UsdStage, path: SdfPath) -> UsdGeomPoints {
        return UsdGeomPoints.Define(&stage, path)
    }

    /// Define a UsdGeomPoints at the given path string.
    public static func define(_ stage: inout UsdStage, path: String) -> UsdGeomPoints {
        return UsdGeomPoints.Define(&stage, SdfPath(path))
    }

    /// The widths attribute.
    public var widthsAttr: UsdAttribute {
        return self.GetWidthsAttr()
    }

    /// The ids attribute.
    public var idsAttr: UsdAttribute {
        return self.GetIdsAttr()
    }
}

// MARK: - UsdGeomBasisCurves Extensions

extension UsdGeomBasisCurves {
    /// Define a UsdGeomBasisCurves at the given path.
    public static func define(_ stage: inout UsdStage, path: SdfPath) -> UsdGeomBasisCurves {
        return UsdGeomBasisCurves.Define(&stage, path)
    }

    /// Define a UsdGeomBasisCurves at the given path string.
    public static func define(_ stage: inout UsdStage, path: String) -> UsdGeomBasisCurves {
        return UsdGeomBasisCurves.Define(&stage, SdfPath(path))
    }

    /// The type attribute (linear or cubic).
    public var typeAttr: UsdAttribute {
        return self.GetTypeAttr()
    }

    /// The basis attribute (bezier, bspline, catmullRom).
    public var basisAttr: UsdAttribute {
        return self.GetBasisAttr()
    }

    /// The wrap attribute (nonperiodic, periodic, pinned).
    public var wrapAttr: UsdAttribute {
        return self.GetWrapAttr()
    }
}

// MARK: - UsdGeomNurbsCurves Extensions

extension UsdGeomNurbsCurves {
    /// Define a UsdGeomNurbsCurves at the given path.
    public static func define(_ stage: inout UsdStage, path: SdfPath) -> UsdGeomNurbsCurves {
        return UsdGeomNurbsCurves.Define(&stage, path)
    }

    /// Define a UsdGeomNurbsCurves at the given path string.
    public static func define(_ stage: inout UsdStage, path: String) -> UsdGeomNurbsCurves {
        return UsdGeomNurbsCurves.Define(&stage, SdfPath(path))
    }
}

// MARK: - UsdGeomCamera Extensions

extension UsdGeomCamera {
    /// Define a UsdGeomCamera at the given path.
    public static func define(_ stage: inout UsdStage, path: SdfPath) -> UsdGeomCamera {
        return UsdGeomCamera.Define(&stage, path)
    }

    /// Define a UsdGeomCamera at the given path string.
    public static func define(_ stage: inout UsdStage, path: String) -> UsdGeomCamera {
        return UsdGeomCamera.Define(&stage, SdfPath(path))
    }

    /// The projection attribute.
    public var projectionAttr: UsdAttribute {
        return self.GetProjectionAttr()
    }

    /// The focalLength attribute.
    public var focalLengthAttr: UsdAttribute {
        return self.GetFocalLengthAttr()
    }

    /// The horizontalAperture attribute.
    public var horizontalApertureAttr: UsdAttribute {
        return self.GetHorizontalApertureAttr()
    }

    /// The verticalAperture attribute.
    public var verticalApertureAttr: UsdAttribute {
        return self.GetVerticalApertureAttr()
    }

    /// The clippingRange attribute.
    public var clippingRangeAttr: UsdAttribute {
        return self.GetClippingRangeAttr()
    }

    /// The fStop attribute.
    public var fStopAttr: UsdAttribute {
        return self.GetFStopAttr()
    }

    /// The focusDistance attribute.
    public var focusDistanceAttr: UsdAttribute {
        return self.GetFocusDistanceAttr()
    }
}

// MARK: - Geometric Primitives Extensions

extension UsdGeomCube {
    /// Define a UsdGeomCube at the given path.
    public static func define(_ stage: inout UsdStage, path: SdfPath) -> UsdGeomCube {
        return UsdGeomCube.Define(&stage, path)
    }

    /// Define a UsdGeomCube at the given path string.
    public static func define(_ stage: inout UsdStage, path: String) -> UsdGeomCube {
        return UsdGeomCube.Define(&stage, SdfPath(path))
    }

    /// The size attribute.
    public var sizeAttr: UsdAttribute {
        return self.GetSizeAttr()
    }
}

extension UsdGeomSphere {
    /// Define a UsdGeomSphere at the given path.
    public static func define(_ stage: inout UsdStage, path: SdfPath) -> UsdGeomSphere {
        return UsdGeomSphere.Define(&stage, path)
    }

    /// Define a UsdGeomSphere at the given path string.
    public static func define(_ stage: inout UsdStage, path: String) -> UsdGeomSphere {
        return UsdGeomSphere.Define(&stage, SdfPath(path))
    }

    /// The radius attribute.
    public var radiusAttr: UsdAttribute {
        return self.GetRadiusAttr()
    }
}

extension UsdGeomCylinder {
    /// Define a UsdGeomCylinder at the given path.
    public static func define(_ stage: inout UsdStage, path: SdfPath) -> UsdGeomCylinder {
        return UsdGeomCylinder.Define(&stage, path)
    }

    /// Define a UsdGeomCylinder at the given path string.
    public static func define(_ stage: inout UsdStage, path: String) -> UsdGeomCylinder {
        return UsdGeomCylinder.Define(&stage, SdfPath(path))
    }

    /// The radius attribute.
    public var radiusAttr: UsdAttribute {
        return self.GetRadiusAttr()
    }

    /// The height attribute.
    public var heightAttr: UsdAttribute {
        return self.GetHeightAttr()
    }

    /// The axis attribute.
    public var axisAttr: UsdAttribute {
        return self.GetAxisAttr()
    }
}

extension UsdGeomCapsule {
    /// Define a UsdGeomCapsule at the given path.
    public static func define(_ stage: inout UsdStage, path: SdfPath) -> UsdGeomCapsule {
        return UsdGeomCapsule.Define(&stage, path)
    }

    /// Define a UsdGeomCapsule at the given path string.
    public static func define(_ stage: inout UsdStage, path: String) -> UsdGeomCapsule {
        return UsdGeomCapsule.Define(&stage, SdfPath(path))
    }

    /// The radius attribute.
    public var radiusAttr: UsdAttribute {
        return self.GetRadiusAttr()
    }

    /// The height attribute.
    public var heightAttr: UsdAttribute {
        return self.GetHeightAttr()
    }

    /// The axis attribute.
    public var axisAttr: UsdAttribute {
        return self.GetAxisAttr()
    }
}

extension UsdGeomCone {
    /// Define a UsdGeomCone at the given path.
    public static func define(_ stage: inout UsdStage, path: SdfPath) -> UsdGeomCone {
        return UsdGeomCone.Define(&stage, path)
    }

    /// Define a UsdGeomCone at the given path string.
    public static func define(_ stage: inout UsdStage, path: String) -> UsdGeomCone {
        return UsdGeomCone.Define(&stage, SdfPath(path))
    }

    /// The radius attribute.
    public var radiusAttr: UsdAttribute {
        return self.GetRadiusAttr()
    }

    /// The height attribute.
    public var heightAttr: UsdAttribute {
        return self.GetHeightAttr()
    }

    /// The axis attribute.
    public var axisAttr: UsdAttribute {
        return self.GetAxisAttr()
    }
}

// MARK: - UsdGeomPrimvar Extensions

extension UsdGeomPrimvar {
    /// Whether this is a valid primvar.
    public var isValid: Bool {
        return self.IsValid()
    }

    /// The underlying attribute.
    public var attr: UsdAttribute {
        return self.GetAttr()
    }

    /// The name of the primvar (without namespace prefix).
    public var primvarName: TfToken {
        return self.GetPrimvarName()
    }

    /// The interpolation of this primvar.
    public var interpolation: TfToken {
        return self.GetInterpolation()
    }

    /// The element size for indexed primvars.
    public var elementSize: Int {
        return Int(self.GetElementSize())
    }

    /// Whether this primvar is indexed.
    public var isIndexed: Bool {
        return self.IsIndexed()
    }
}

// MARK: - UsdGeomPrimvarsAPI Extensions

extension UsdGeomPrimvarsAPI {
    /// Check if a primvar exists.
    public func hasPrimvar(_ name: TfToken) -> Bool {
        return self.HasPrimvar(name)
    }

    /// Check if a primvar exists (by string name).
    public func hasPrimvar(_ name: String) -> Bool {
        return self.HasPrimvar(TfToken(name))
    }

    /// Get a primvar by name.
    public func primvar(_ name: TfToken) -> UsdGeomPrimvar {
        return self.GetPrimvar(name)
    }

    /// Get a primvar by string name.
    public func primvar(_ name: String) -> UsdGeomPrimvar {
        return self.GetPrimvar(TfToken(name))
    }

    /// Remove a primvar.
    @discardableResult
    public mutating func removePrimvar(_ name: TfToken) -> Bool {
        return self.RemovePrimvar(name)
    }

    /// Remove a primvar by string name.
    @discardableResult
    public mutating func removePrimvar(_ name: String) -> Bool {
        return self.RemovePrimvar(TfToken(name))
    }
}
