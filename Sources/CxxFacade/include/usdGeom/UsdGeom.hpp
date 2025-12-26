// UsdGeom.hpp - Geometry Schemas facade
// CLEAN HEADER - NO pxr includes!
//
// This module provides wrappers for UsdGeom schema classes including transforms,
// meshes, curves, cameras, and geometric primitives.
#pragma once

#include "../CxxFacade.h"
#include "../usd/Usd.hpp"
#include "../base/Gf.hpp"
#include "../base/Vt.hpp"

namespace cxxfacade {
namespace usdGeom {

// ============================================================================
// Forward Declarations
// ============================================================================

class UsdGeomImageable;
class UsdGeomXformable;
class UsdGeomXform;
class UsdGeomScope;
class UsdGeomBoundable;
class UsdGeomGprim;
class UsdGeomPointBased;
class UsdGeomMesh;
class UsdGeomPoints;
class UsdGeomCurves;
class UsdGeomBasisCurves;
class UsdGeomNurbsCurves;
class UsdGeomCamera;
class UsdGeomCube;
class UsdGeomSphere;
class UsdGeomCylinder;
class UsdGeomCapsule;
class UsdGeomCone;
class UsdGeomPrimvar;
class UsdGeomPrimvarsAPI;
class UsdGeomXformOp;

// ============================================================================
// UsdGeomXformOp - Transform operation
// ============================================================================

/// Enum for xform operation types
enum class UsdGeomXformOpType {
    TypeInvalid,
    TypeTranslate,
    TypeScale,
    TypeRotateX,
    TypeRotateY,
    TypeRotateZ,
    TypeRotateXYZ,
    TypeRotateXZY,
    TypeRotateYXZ,
    TypeRotateYZX,
    TypeRotateZXY,
    TypeRotateZYX,
    TypeOrient,
    TypeTransform
};

/// Enum for xform operation precision
enum class UsdGeomXformOpPrecision {
    PrecisionDouble,
    PrecisionFloat,
    PrecisionHalf
};

/// UsdGeomXformOp - Represents a single transform operation
class UsdGeomXformOp {
public:
    UsdGeomXformOp();
    UsdGeomXformOp(const UsdGeomXformOp& other);
    UsdGeomXformOp(UsdGeomXformOp&& other) noexcept;
    ~UsdGeomXformOp();

    UsdGeomXformOp& operator=(const UsdGeomXformOp& other);
    UsdGeomXformOp& operator=(UsdGeomXformOp&& other) noexcept;

    /// Check if this is a valid xform op
    bool IsValid() const;

    /// Get the type of this operation
    UsdGeomXformOpType GetOpType() const;

    /// Get the precision of this operation
    UsdGeomXformOpPrecision GetPrecision() const;

    /// Get the op name (e.g., "xformOp:translate")
    tf::TfToken GetOpName() const;

    /// Get the underlying attribute
    usd::UsdAttribute GetAttr() const;

    /// Check if this is an inverse operation
    bool IsInverseOp() const;

    /// Get the transformation matrix for this op at the given time
    bool GetOpTransform(gf::Matrix4d* transform, usd::UsdTimeCode time = usd::UsdTimeCode::Default()) const;

    void* GetImpl() const { return impl_; }
    static UsdGeomXformOp FromImpl(void* impl);

private:
    void* impl_;
};

// ============================================================================
// UsdGeomImageable - Base class for imageable prims
// ============================================================================

/// UsdGeomImageable - Base class for all imageable prims.
///
/// This is an abstract base class that provides visibility and purpose
/// attributes for all geometry prims.
class UsdGeomImageable : public usd::UsdTyped {
public:
    explicit UsdGeomImageable(const usd::UsdPrim& prim = usd::UsdPrim());
    explicit UsdGeomImageable(const usd::UsdSchemaBase& schemaObj);
    virtual ~UsdGeomImageable();

    /// Get this prim at path on stage
    static UsdGeomImageable Get(const usd::UsdStage& stage, const sdf::SdfPath& path);

    // ----- Visibility -----

    /// Get the visibility attribute
    usd::UsdAttribute GetVisibilityAttr() const;

    /// Create the visibility attribute
    usd::UsdAttribute CreateVisibilityAttr();

    /// Compute visibility at the given time
    tf::TfToken ComputeVisibility(usd::UsdTimeCode time = usd::UsdTimeCode::Default()) const;

    /// Make this prim invisible at the given time
    void MakeInvisible(usd::UsdTimeCode time = usd::UsdTimeCode::Default());

    /// Make this prim visible at the given time
    void MakeVisible(usd::UsdTimeCode time = usd::UsdTimeCode::Default());

    // ----- Purpose -----

    /// Get the purpose attribute
    usd::UsdAttribute GetPurposeAttr() const;

    /// Create the purpose attribute
    usd::UsdAttribute CreatePurposeAttr();

    /// Compute the purpose, considering inheritance
    tf::TfToken ComputePurpose() const;

    // ----- Proxy Prim -----

    /// Get the proxy prim relationship
    usd::UsdRelationship GetProxyPrimRel() const;

    /// Create the proxy prim relationship
    usd::UsdRelationship CreateProxyPrimRel();

    // ----- World Bounds -----

    /// Compute the bounding box in local space at the given time
    gf::BBox3d ComputeLocalBound(usd::UsdTimeCode time = usd::UsdTimeCode::Default(),
                                  const tf::TfToken& purpose1 = tf::TfToken(),
                                  const tf::TfToken& purpose2 = tf::TfToken(),
                                  const tf::TfToken& purpose3 = tf::TfToken(),
                                  const tf::TfToken& purpose4 = tf::TfToken()) const;

    /// Compute the world-space bounding box at the given time
    gf::BBox3d ComputeWorldBound(usd::UsdTimeCode time = usd::UsdTimeCode::Default(),
                                  const tf::TfToken& purpose1 = tf::TfToken(),
                                  const tf::TfToken& purpose2 = tf::TfToken(),
                                  const tf::TfToken& purpose3 = tf::TfToken(),
                                  const tf::TfToken& purpose4 = tf::TfToken()) const;

protected:
    usd::UsdSchemaKind _GetSchemaKind() const override;
};

// ============================================================================
// UsdGeomXformable - Base class for transformable prims
// ============================================================================

/// UsdGeomXformable - Base class for all transformable prims.
///
/// This abstract class provides the interface for encoding transformation
/// operations on prims.
class UsdGeomXformable : public UsdGeomImageable {
public:
    explicit UsdGeomXformable(const usd::UsdPrim& prim = usd::UsdPrim());
    explicit UsdGeomXformable(const usd::UsdSchemaBase& schemaObj);
    virtual ~UsdGeomXformable();

    /// Get this prim at path on stage
    static UsdGeomXformable Get(const usd::UsdStage& stage, const sdf::SdfPath& path);

    // ----- Transform Operations -----

    /// Add a translate operation
    UsdGeomXformOp AddTranslateOp(
        UsdGeomXformOpPrecision precision = UsdGeomXformOpPrecision::PrecisionDouble,
        const tf::TfToken& opSuffix = tf::TfToken(),
        bool isInverseOp = false);

    /// Add a scale operation
    UsdGeomXformOp AddScaleOp(
        UsdGeomXformOpPrecision precision = UsdGeomXformOpPrecision::PrecisionFloat,
        const tf::TfToken& opSuffix = tf::TfToken(),
        bool isInverseOp = false);

    /// Add a rotate X operation
    UsdGeomXformOp AddRotateXOp(
        UsdGeomXformOpPrecision precision = UsdGeomXformOpPrecision::PrecisionFloat,
        const tf::TfToken& opSuffix = tf::TfToken(),
        bool isInverseOp = false);

    /// Add a rotate Y operation
    UsdGeomXformOp AddRotateYOp(
        UsdGeomXformOpPrecision precision = UsdGeomXformOpPrecision::PrecisionFloat,
        const tf::TfToken& opSuffix = tf::TfToken(),
        bool isInverseOp = false);

    /// Add a rotate Z operation
    UsdGeomXformOp AddRotateZOp(
        UsdGeomXformOpPrecision precision = UsdGeomXformOpPrecision::PrecisionFloat,
        const tf::TfToken& opSuffix = tf::TfToken(),
        bool isInverseOp = false);

    /// Add a rotate XYZ operation
    UsdGeomXformOp AddRotateXYZOp(
        UsdGeomXformOpPrecision precision = UsdGeomXformOpPrecision::PrecisionFloat,
        const tf::TfToken& opSuffix = tf::TfToken(),
        bool isInverseOp = false);

    /// Add an orient (quaternion) operation
    UsdGeomXformOp AddOrientOp(
        UsdGeomXformOpPrecision precision = UsdGeomXformOpPrecision::PrecisionFloat,
        const tf::TfToken& opSuffix = tf::TfToken(),
        bool isInverseOp = false);

    /// Add a 4x4 transform matrix operation
    UsdGeomXformOp AddTransformOp(
        UsdGeomXformOpPrecision precision = UsdGeomXformOpPrecision::PrecisionDouble,
        const tf::TfToken& opSuffix = tf::TfToken(),
        bool isInverseOp = false);

    /// Get the ordered list of transform ops
    std::vector<UsdGeomXformOp> GetOrderedXformOps(bool* resetsXformStack = nullptr) const;

    /// Set whether this prim resets its parent's transform stack
    bool SetResetXformStack(bool resetXform);

    /// Check if this prim resets its parent's transform stack
    bool GetResetXformStack() const;

    /// Clear all transform ops
    bool ClearXformOpOrder();

    /// Create a single matrix transform op, clearing any existing ops
    UsdGeomXformOp MakeMatrixXform();

    // ----- Local Transformation -----

    /// Get the local transformation matrix at the given time
    bool GetLocalTransformation(gf::Matrix4d* transform,
                                bool* resetsXformStack = nullptr,
                                usd::UsdTimeCode time = usd::UsdTimeCode::Default()) const;

    /// Check if the transform might be time-varying
    bool TransformMightBeTimeVarying() const;

protected:
    usd::UsdSchemaKind _GetSchemaKind() const override;
};

// ============================================================================
// UsdGeomXform - Concrete transform prim
// ============================================================================

/// UsdGeomXform - Concrete prim schema for a transform.
class UsdGeomXform : public UsdGeomXformable {
public:
    explicit UsdGeomXform(const usd::UsdPrim& prim = usd::UsdPrim());
    explicit UsdGeomXform(const usd::UsdSchemaBase& schemaObj);
    virtual ~UsdGeomXform();

    /// Get this prim at path on stage
    static UsdGeomXform Get(const usd::UsdStage& stage, const sdf::SdfPath& path);

    /// Define a UsdGeomXform at the given path
    static UsdGeomXform Define(usd::UsdStage& stage, const sdf::SdfPath& path);

protected:
    usd::UsdSchemaKind _GetSchemaKind() const override;
};

// ============================================================================
// UsdGeomScope - Grouping prim without transform
// ============================================================================

/// UsdGeomScope - A Scope is a container prim for grouping geometry.
class UsdGeomScope : public UsdGeomImageable {
public:
    explicit UsdGeomScope(const usd::UsdPrim& prim = usd::UsdPrim());
    explicit UsdGeomScope(const usd::UsdSchemaBase& schemaObj);
    virtual ~UsdGeomScope();

    /// Get this prim at path on stage
    static UsdGeomScope Get(const usd::UsdStage& stage, const sdf::SdfPath& path);

    /// Define a UsdGeomScope at the given path
    static UsdGeomScope Define(usd::UsdStage& stage, const sdf::SdfPath& path);

protected:
    usd::UsdSchemaKind _GetSchemaKind() const override;
};

// ============================================================================
// UsdGeomBoundable - Base class for bounded prims
// ============================================================================

/// UsdGeomBoundable - Abstract base class for prims with extent.
class UsdGeomBoundable : public UsdGeomXformable {
public:
    explicit UsdGeomBoundable(const usd::UsdPrim& prim = usd::UsdPrim());
    explicit UsdGeomBoundable(const usd::UsdSchemaBase& schemaObj);
    virtual ~UsdGeomBoundable();

    /// Get this prim at path on stage
    static UsdGeomBoundable Get(const usd::UsdStage& stage, const sdf::SdfPath& path);

    /// Get the extent attribute
    usd::UsdAttribute GetExtentAttr() const;

    /// Create the extent attribute
    usd::UsdAttribute CreateExtentAttr();

    /// Compute the extent from points
    static bool ComputeExtentFromPlugins(const UsdGeomBoundable& boundable,
                                          usd::UsdTimeCode time,
                                          std::vector<gf::Vec3f>* extent);

protected:
    usd::UsdSchemaKind _GetSchemaKind() const override;
};

// ============================================================================
// UsdGeomGprim - Base class for geometric primitives
// ============================================================================

/// UsdGeomGprim - Base class for all geometric primitives.
class UsdGeomGprim : public UsdGeomBoundable {
public:
    explicit UsdGeomGprim(const usd::UsdPrim& prim = usd::UsdPrim());
    explicit UsdGeomGprim(const usd::UsdSchemaBase& schemaObj);
    virtual ~UsdGeomGprim();

    /// Get this prim at path on stage
    static UsdGeomGprim Get(const usd::UsdStage& stage, const sdf::SdfPath& path);

    // ----- Display -----

    /// Get the doubleSided attribute
    usd::UsdAttribute GetDoubleSidedAttr() const;

    /// Create the doubleSided attribute
    usd::UsdAttribute CreateDoubleSidedAttr();

    /// Get the orientation attribute
    usd::UsdAttribute GetOrientationAttr() const;

    /// Create the orientation attribute
    usd::UsdAttribute CreateOrientationAttr();

    /// Get the displayColor primvar
    usd::UsdAttribute GetDisplayColorAttr() const;

    /// Create the displayColor primvar
    usd::UsdAttribute CreateDisplayColorAttr();

    /// Get the displayOpacity primvar
    usd::UsdAttribute GetDisplayOpacityAttr() const;

    /// Create the displayOpacity primvar
    usd::UsdAttribute CreateDisplayOpacityAttr();

protected:
    usd::UsdSchemaKind _GetSchemaKind() const override;
};

// ============================================================================
// UsdGeomPointBased - Base class for point-based geometry
// ============================================================================

/// UsdGeomPointBased - Base class for point-based geometry like meshes and curves.
class UsdGeomPointBased : public UsdGeomGprim {
public:
    explicit UsdGeomPointBased(const usd::UsdPrim& prim = usd::UsdPrim());
    explicit UsdGeomPointBased(const usd::UsdSchemaBase& schemaObj);
    virtual ~UsdGeomPointBased();

    /// Get this prim at path on stage
    static UsdGeomPointBased Get(const usd::UsdStage& stage, const sdf::SdfPath& path);

    // ----- Points -----

    /// Get the points attribute
    usd::UsdAttribute GetPointsAttr() const;

    /// Create the points attribute
    usd::UsdAttribute CreatePointsAttr();

    // ----- Velocities -----

    /// Get the velocities attribute
    usd::UsdAttribute GetVelocitiesAttr() const;

    /// Create the velocities attribute
    usd::UsdAttribute CreateVelocitiesAttr();

    // ----- Accelerations -----

    /// Get the accelerations attribute
    usd::UsdAttribute GetAccelerationsAttr() const;

    /// Create the accelerations attribute
    usd::UsdAttribute CreateAccelerationsAttr();

    // ----- Normals -----

    /// Get the normals attribute
    usd::UsdAttribute GetNormalsAttr() const;

    /// Create the normals attribute
    usd::UsdAttribute CreateNormalsAttr();

    /// Get the interpolation of the normals attribute
    tf::TfToken GetNormalsInterpolation() const;

    /// Set the interpolation of the normals attribute
    bool SetNormalsInterpolation(const tf::TfToken& interpolation);

    /// Compute points at the given time
    bool ComputePointsAtTime(std::vector<gf::Vec3f>* points,
                              usd::UsdTimeCode time,
                              usd::UsdTimeCode baseTime = usd::UsdTimeCode::Default()) const;

protected:
    usd::UsdSchemaKind _GetSchemaKind() const override;
};

// ============================================================================
// UsdGeomMesh - Polygon mesh
// ============================================================================

/// UsdGeomMesh - Encodes a mesh with optional subdivision properties.
class UsdGeomMesh : public UsdGeomPointBased {
public:
    explicit UsdGeomMesh(const usd::UsdPrim& prim = usd::UsdPrim());
    explicit UsdGeomMesh(const usd::UsdSchemaBase& schemaObj);
    virtual ~UsdGeomMesh();

    /// Get this prim at path on stage
    static UsdGeomMesh Get(const usd::UsdStage& stage, const sdf::SdfPath& path);

    /// Define a UsdGeomMesh at the given path
    static UsdGeomMesh Define(usd::UsdStage& stage, const sdf::SdfPath& path);

    // ----- Topology -----

    /// Get the faceVertexIndices attribute
    usd::UsdAttribute GetFaceVertexIndicesAttr() const;

    /// Create the faceVertexIndices attribute
    usd::UsdAttribute CreateFaceVertexIndicesAttr();

    /// Get the faceVertexCounts attribute
    usd::UsdAttribute GetFaceVertexCountsAttr() const;

    /// Create the faceVertexCounts attribute
    usd::UsdAttribute CreateFaceVertexCountsAttr();

    /// Get the number of faces
    size_t GetFaceCount(usd::UsdTimeCode time = usd::UsdTimeCode::Default()) const;

    // ----- Subdivision -----

    /// Get the subdivisionScheme attribute
    usd::UsdAttribute GetSubdivisionSchemeAttr() const;

    /// Create the subdivisionScheme attribute
    usd::UsdAttribute CreateSubdivisionSchemeAttr();

    /// Get the interpolateBoundary attribute
    usd::UsdAttribute GetInterpolateBoundaryAttr() const;

    /// Create the interpolateBoundary attribute
    usd::UsdAttribute CreateInterpolateBoundaryAttr();

    /// Get the faceVaryingLinearInterpolation attribute
    usd::UsdAttribute GetFaceVaryingLinearInterpolationAttr() const;

    /// Create the faceVaryingLinearInterpolation attribute
    usd::UsdAttribute CreateFaceVaryingLinearInterpolationAttr();

    // ----- Creases and Corners -----

    /// Get the holeIndices attribute
    usd::UsdAttribute GetHoleIndicesAttr() const;

    /// Create the holeIndices attribute
    usd::UsdAttribute CreateHoleIndicesAttr();

    /// Get the cornerIndices attribute
    usd::UsdAttribute GetCornerIndicesAttr() const;

    /// Create the cornerIndices attribute
    usd::UsdAttribute CreateCornerIndicesAttr();

    /// Get the cornerSharpnesses attribute
    usd::UsdAttribute GetCornerSharpnessesAttr() const;

    /// Create the cornerSharpnesses attribute
    usd::UsdAttribute CreateCornerSharpnessesAttr();

    /// Get the creaseIndices attribute
    usd::UsdAttribute GetCreaseIndicesAttr() const;

    /// Create the creaseIndices attribute
    usd::UsdAttribute CreateCreaseIndicesAttr();

    /// Get the creaseLengths attribute
    usd::UsdAttribute GetCreaseLengthsAttr() const;

    /// Create the creaseLengths attribute
    usd::UsdAttribute CreateCreaseLengthsAttr();

    /// Get the creaseSharpnesses attribute
    usd::UsdAttribute GetCreaseSharpnessesAttr() const;

    /// Create the creaseSharpnesses attribute
    usd::UsdAttribute CreateCreaseSharpnessesAttr();

    // ----- Validation -----

    /// Validate the topology of a mesh
    static bool ValidateTopology(const std::vector<int>& faceVertexIndices,
                                  const std::vector<int>& faceVertexCounts,
                                  size_t numPoints,
                                  std::string* reason = nullptr);

    /// Infinite sharpness constant
    static float GetSharpnessInfinite();

protected:
    usd::UsdSchemaKind _GetSchemaKind() const override;
};

// ============================================================================
// UsdGeomPoints - Point cloud
// ============================================================================

/// UsdGeomPoints - Point cloud geometry.
class UsdGeomPoints : public UsdGeomPointBased {
public:
    explicit UsdGeomPoints(const usd::UsdPrim& prim = usd::UsdPrim());
    explicit UsdGeomPoints(const usd::UsdSchemaBase& schemaObj);
    virtual ~UsdGeomPoints();

    /// Get this prim at path on stage
    static UsdGeomPoints Get(const usd::UsdStage& stage, const sdf::SdfPath& path);

    /// Define a UsdGeomPoints at the given path
    static UsdGeomPoints Define(usd::UsdStage& stage, const sdf::SdfPath& path);

    /// Get the widths attribute
    usd::UsdAttribute GetWidthsAttr() const;

    /// Create the widths attribute
    usd::UsdAttribute CreateWidthsAttr();

    /// Get the ids attribute
    usd::UsdAttribute GetIdsAttr() const;

    /// Create the ids attribute
    usd::UsdAttribute CreateIdsAttr();

protected:
    usd::UsdSchemaKind _GetSchemaKind() const override;
};

// ============================================================================
// UsdGeomCurves - Abstract base for curves
// ============================================================================

/// UsdGeomCurves - Abstract base class for all curve types.
class UsdGeomCurves : public UsdGeomPointBased {
public:
    explicit UsdGeomCurves(const usd::UsdPrim& prim = usd::UsdPrim());
    explicit UsdGeomCurves(const usd::UsdSchemaBase& schemaObj);
    virtual ~UsdGeomCurves();

    /// Get this prim at path on stage
    static UsdGeomCurves Get(const usd::UsdStage& stage, const sdf::SdfPath& path);

    /// Get the curveVertexCounts attribute
    usd::UsdAttribute GetCurveVertexCountsAttr() const;

    /// Create the curveVertexCounts attribute
    usd::UsdAttribute CreateCurveVertexCountsAttr();

    /// Get the widths attribute
    usd::UsdAttribute GetWidthsAttr() const;

    /// Create the widths attribute
    usd::UsdAttribute CreateWidthsAttr();

protected:
    usd::UsdSchemaKind _GetSchemaKind() const override;
};

// ============================================================================
// UsdGeomBasisCurves - Cubic basis curves
// ============================================================================

/// UsdGeomBasisCurves - Curves using a basis like Bezier, B-spline, or Catmull-Rom.
class UsdGeomBasisCurves : public UsdGeomCurves {
public:
    explicit UsdGeomBasisCurves(const usd::UsdPrim& prim = usd::UsdPrim());
    explicit UsdGeomBasisCurves(const usd::UsdSchemaBase& schemaObj);
    virtual ~UsdGeomBasisCurves();

    /// Get this prim at path on stage
    static UsdGeomBasisCurves Get(const usd::UsdStage& stage, const sdf::SdfPath& path);

    /// Define a UsdGeomBasisCurves at the given path
    static UsdGeomBasisCurves Define(usd::UsdStage& stage, const sdf::SdfPath& path);

    /// Get the type attribute
    usd::UsdAttribute GetTypeAttr() const;

    /// Create the type attribute
    usd::UsdAttribute CreateTypeAttr();

    /// Get the basis attribute
    usd::UsdAttribute GetBasisAttr() const;

    /// Create the basis attribute
    usd::UsdAttribute CreateBasisAttr();

    /// Get the wrap attribute
    usd::UsdAttribute GetWrapAttr() const;

    /// Create the wrap attribute
    usd::UsdAttribute CreateWrapAttr();

protected:
    usd::UsdSchemaKind _GetSchemaKind() const override;
};

// ============================================================================
// UsdGeomNurbsCurves - NURBS curves
// ============================================================================

/// UsdGeomNurbsCurves - NURBS curves.
class UsdGeomNurbsCurves : public UsdGeomCurves {
public:
    explicit UsdGeomNurbsCurves(const usd::UsdPrim& prim = usd::UsdPrim());
    explicit UsdGeomNurbsCurves(const usd::UsdSchemaBase& schemaObj);
    virtual ~UsdGeomNurbsCurves();

    /// Get this prim at path on stage
    static UsdGeomNurbsCurves Get(const usd::UsdStage& stage, const sdf::SdfPath& path);

    /// Define a UsdGeomNurbsCurves at the given path
    static UsdGeomNurbsCurves Define(usd::UsdStage& stage, const sdf::SdfPath& path);

    /// Get the order attribute
    usd::UsdAttribute GetOrderAttr() const;

    /// Create the order attribute
    usd::UsdAttribute CreateOrderAttr();

    /// Get the knots attribute
    usd::UsdAttribute GetKnotsAttr() const;

    /// Create the knots attribute
    usd::UsdAttribute CreateKnotsAttr();

    /// Get the ranges attribute
    usd::UsdAttribute GetRangesAttr() const;

    /// Create the ranges attribute
    usd::UsdAttribute CreateRangesAttr();

protected:
    usd::UsdSchemaKind _GetSchemaKind() const override;
};

// ============================================================================
// UsdGeomCamera - Camera prim
// ============================================================================

/// UsdGeomCamera - Camera for scene viewing.
class UsdGeomCamera : public UsdGeomXformable {
public:
    explicit UsdGeomCamera(const usd::UsdPrim& prim = usd::UsdPrim());
    explicit UsdGeomCamera(const usd::UsdSchemaBase& schemaObj);
    virtual ~UsdGeomCamera();

    /// Get this prim at path on stage
    static UsdGeomCamera Get(const usd::UsdStage& stage, const sdf::SdfPath& path);

    /// Define a UsdGeomCamera at the given path
    static UsdGeomCamera Define(usd::UsdStage& stage, const sdf::SdfPath& path);

    // ----- Projection -----

    /// Get the projection attribute
    usd::UsdAttribute GetProjectionAttr() const;

    /// Create the projection attribute
    usd::UsdAttribute CreateProjectionAttr();

    // ----- Lens -----

    /// Get the focalLength attribute (in tenths of a world unit)
    usd::UsdAttribute GetFocalLengthAttr() const;

    /// Create the focalLength attribute
    usd::UsdAttribute CreateFocalLengthAttr();

    /// Get the horizontalAperture attribute
    usd::UsdAttribute GetHorizontalApertureAttr() const;

    /// Create the horizontalAperture attribute
    usd::UsdAttribute CreateHorizontalApertureAttr();

    /// Get the verticalAperture attribute
    usd::UsdAttribute GetVerticalApertureAttr() const;

    /// Create the verticalAperture attribute
    usd::UsdAttribute CreateVerticalApertureAttr();

    /// Get the horizontalApertureOffset attribute
    usd::UsdAttribute GetHorizontalApertureOffsetAttr() const;

    /// Create the horizontalApertureOffset attribute
    usd::UsdAttribute CreateHorizontalApertureOffsetAttr();

    /// Get the verticalApertureOffset attribute
    usd::UsdAttribute GetVerticalApertureOffsetAttr() const;

    /// Create the verticalApertureOffset attribute
    usd::UsdAttribute CreateVerticalApertureOffsetAttr();

    // ----- Clipping -----

    /// Get the clippingRange attribute
    usd::UsdAttribute GetClippingRangeAttr() const;

    /// Create the clippingRange attribute
    usd::UsdAttribute CreateClippingRangeAttr();

    /// Get the clippingPlanes attribute
    usd::UsdAttribute GetClippingPlanesAttr() const;

    /// Create the clippingPlanes attribute
    usd::UsdAttribute CreateClippingPlanesAttr();

    // ----- Focus -----

    /// Get the fStop attribute
    usd::UsdAttribute GetFStopAttr() const;

    /// Create the fStop attribute
    usd::UsdAttribute CreateFStopAttr();

    /// Get the focusDistance attribute
    usd::UsdAttribute GetFocusDistanceAttr() const;

    /// Create the focusDistance attribute
    usd::UsdAttribute CreateFocusDistanceAttr();

    // ----- Shutter -----

    /// Get the shutterOpen attribute
    usd::UsdAttribute GetShutterOpenAttr() const;

    /// Create the shutterOpen attribute
    usd::UsdAttribute CreateShutterOpenAttr();

    /// Get the shutterClose attribute
    usd::UsdAttribute GetShutterCloseAttr() const;

    /// Create the shutterClose attribute
    usd::UsdAttribute CreateShutterCloseAttr();

    // ----- Computed Values -----

    /// Get the camera parameters (view/projection matrices, etc.)
    gf::Matrix4d GetCamera(usd::UsdTimeCode time = usd::UsdTimeCode::Default()) const;

protected:
    usd::UsdSchemaKind _GetSchemaKind() const override;
};

// ============================================================================
// Geometric Primitives - Cube, Sphere, Cylinder, Capsule, Cone
// ============================================================================

/// UsdGeomCube - A cube centered at the origin.
class UsdGeomCube : public UsdGeomGprim {
public:
    explicit UsdGeomCube(const usd::UsdPrim& prim = usd::UsdPrim());
    explicit UsdGeomCube(const usd::UsdSchemaBase& schemaObj);
    virtual ~UsdGeomCube();

    static UsdGeomCube Get(const usd::UsdStage& stage, const sdf::SdfPath& path);
    static UsdGeomCube Define(usd::UsdStage& stage, const sdf::SdfPath& path);

    /// Get the size attribute
    usd::UsdAttribute GetSizeAttr() const;

    /// Create the size attribute
    usd::UsdAttribute CreateSizeAttr();

protected:
    usd::UsdSchemaKind _GetSchemaKind() const override;
};

/// UsdGeomSphere - A sphere centered at the origin.
class UsdGeomSphere : public UsdGeomGprim {
public:
    explicit UsdGeomSphere(const usd::UsdPrim& prim = usd::UsdPrim());
    explicit UsdGeomSphere(const usd::UsdSchemaBase& schemaObj);
    virtual ~UsdGeomSphere();

    static UsdGeomSphere Get(const usd::UsdStage& stage, const sdf::SdfPath& path);
    static UsdGeomSphere Define(usd::UsdStage& stage, const sdf::SdfPath& path);

    /// Get the radius attribute
    usd::UsdAttribute GetRadiusAttr() const;

    /// Create the radius attribute
    usd::UsdAttribute CreateRadiusAttr();

protected:
    usd::UsdSchemaKind _GetSchemaKind() const override;
};

/// UsdGeomCylinder - A cylinder along an axis.
class UsdGeomCylinder : public UsdGeomGprim {
public:
    explicit UsdGeomCylinder(const usd::UsdPrim& prim = usd::UsdPrim());
    explicit UsdGeomCylinder(const usd::UsdSchemaBase& schemaObj);
    virtual ~UsdGeomCylinder();

    static UsdGeomCylinder Get(const usd::UsdStage& stage, const sdf::SdfPath& path);
    static UsdGeomCylinder Define(usd::UsdStage& stage, const sdf::SdfPath& path);

    usd::UsdAttribute GetRadiusAttr() const;
    usd::UsdAttribute CreateRadiusAttr();

    usd::UsdAttribute GetHeightAttr() const;
    usd::UsdAttribute CreateHeightAttr();

    usd::UsdAttribute GetAxisAttr() const;
    usd::UsdAttribute CreateAxisAttr();

protected:
    usd::UsdSchemaKind _GetSchemaKind() const override;
};

/// UsdGeomCapsule - A capsule along an axis.
class UsdGeomCapsule : public UsdGeomGprim {
public:
    explicit UsdGeomCapsule(const usd::UsdPrim& prim = usd::UsdPrim());
    explicit UsdGeomCapsule(const usd::UsdSchemaBase& schemaObj);
    virtual ~UsdGeomCapsule();

    static UsdGeomCapsule Get(const usd::UsdStage& stage, const sdf::SdfPath& path);
    static UsdGeomCapsule Define(usd::UsdStage& stage, const sdf::SdfPath& path);

    usd::UsdAttribute GetRadiusAttr() const;
    usd::UsdAttribute CreateRadiusAttr();

    usd::UsdAttribute GetHeightAttr() const;
    usd::UsdAttribute CreateHeightAttr();

    usd::UsdAttribute GetAxisAttr() const;
    usd::UsdAttribute CreateAxisAttr();

protected:
    usd::UsdSchemaKind _GetSchemaKind() const override;
};

/// UsdGeomCone - A cone along an axis.
class UsdGeomCone : public UsdGeomGprim {
public:
    explicit UsdGeomCone(const usd::UsdPrim& prim = usd::UsdPrim());
    explicit UsdGeomCone(const usd::UsdSchemaBase& schemaObj);
    virtual ~UsdGeomCone();

    static UsdGeomCone Get(const usd::UsdStage& stage, const sdf::SdfPath& path);
    static UsdGeomCone Define(usd::UsdStage& stage, const sdf::SdfPath& path);

    usd::UsdAttribute GetRadiusAttr() const;
    usd::UsdAttribute CreateRadiusAttr();

    usd::UsdAttribute GetHeightAttr() const;
    usd::UsdAttribute CreateHeightAttr();

    usd::UsdAttribute GetAxisAttr() const;
    usd::UsdAttribute CreateAxisAttr();

protected:
    usd::UsdSchemaKind _GetSchemaKind() const override;
};

// ============================================================================
// UsdGeomPrimvar - Primvar attribute wrapper
// ============================================================================

/// Interpolation types for primvars
enum class UsdGeomPrimvarInterpolation {
    Constant,    // One value for the entire prim
    Uniform,     // One value per face
    Varying,     // One value per point (linear interpolation)
    Vertex,      // One value per point (subdivision interpolation)
    FaceVarying  // One value per face-vertex
};

/// UsdGeomPrimvar - Wraps a UsdAttribute to provide primvar API.
class UsdGeomPrimvar {
public:
    UsdGeomPrimvar();
    UsdGeomPrimvar(const UsdGeomPrimvar& other);
    UsdGeomPrimvar(UsdGeomPrimvar&& other) noexcept;
    ~UsdGeomPrimvar();

    UsdGeomPrimvar& operator=(const UsdGeomPrimvar& other);
    UsdGeomPrimvar& operator=(UsdGeomPrimvar&& other) noexcept;

    /// Check if this is a valid primvar
    bool IsValid() const;
    explicit operator bool() const { return IsValid(); }

    /// Get the underlying attribute
    usd::UsdAttribute GetAttr() const;

    /// Get the name of the primvar (without namespace prefix)
    tf::TfToken GetPrimvarName() const;

    /// Get the interpolation
    tf::TfToken GetInterpolation() const;

    /// Set the interpolation
    bool SetInterpolation(const tf::TfToken& interpolation);

    /// Get the element size (for indexed primvars)
    int GetElementSize() const;

    /// Set the element size
    bool SetElementSize(int size);

    /// Check if this primvar is indexed
    bool IsIndexed() const;

    /// Get the value at the given time
    bool Get(vt::VtValue* value, usd::UsdTimeCode time = usd::UsdTimeCode::Default()) const;

    /// Set the value at the given time
    bool Set(const vt::VtValue& value, usd::UsdTimeCode time = usd::UsdTimeCode::Default());

    void* GetImpl() const { return impl_; }
    static UsdGeomPrimvar FromImpl(void* impl);

private:
    void* impl_;
};

// ============================================================================
// UsdGeomPrimvarsAPI - API for managing primvars on a prim
// ============================================================================

/// UsdGeomPrimvarsAPI - API for creating and managing primvars on prims.
class UsdGeomPrimvarsAPI : public usd::UsdAPISchemaBase {
public:
    explicit UsdGeomPrimvarsAPI(const usd::UsdPrim& prim = usd::UsdPrim());
    explicit UsdGeomPrimvarsAPI(const usd::UsdSchemaBase& schemaObj);
    virtual ~UsdGeomPrimvarsAPI();

    /// Get this API on the prim at path on stage
    static UsdGeomPrimvarsAPI Get(const usd::UsdStage& stage, const sdf::SdfPath& path);

    /// Create a new primvar
    UsdGeomPrimvar CreatePrimvar(const tf::TfToken& name,
                                  const std::string& typeName,
                                  const tf::TfToken& interpolation = tf::TfToken());

    /// Get a primvar by name
    UsdGeomPrimvar GetPrimvar(const tf::TfToken& name) const;

    /// Get all primvars on this prim
    std::vector<UsdGeomPrimvar> GetPrimvars() const;

    /// Get only authored primvars on this prim
    std::vector<UsdGeomPrimvar> GetAuthoredPrimvars() const;

    /// Check if a primvar exists
    bool HasPrimvar(const tf::TfToken& name) const;

    /// Remove a primvar
    bool RemovePrimvar(const tf::TfToken& name);

protected:
    usd::UsdSchemaKind _GetSchemaKind() const override;
};

// ============================================================================
// Token Constants
// ============================================================================

/// Common tokens used in UsdGeom
namespace UsdGeomTokens {
    // Visibility
    tf::TfToken inherited();
    tf::TfToken invisible();

    // Purpose
    tf::TfToken default_();
    tf::TfToken render();
    tf::TfToken proxy();
    tf::TfToken guide();

    // Orientation
    tf::TfToken rightHanded();
    tf::TfToken leftHanded();

    // Subdivision schemes
    tf::TfToken catmullClark();
    tf::TfToken loop();
    tf::TfToken bilinear();
    tf::TfToken none();

    // Interpolation
    tf::TfToken constant();
    tf::TfToken uniform();
    tf::TfToken varying();
    tf::TfToken vertex();
    tf::TfToken faceVarying();

    // Camera
    tf::TfToken perspective();
    tf::TfToken orthographic();

    // Axis
    tf::TfToken x();
    tf::TfToken y();
    tf::TfToken z();

    // Basis curve types
    tf::TfToken linear();
    tf::TfToken cubic();

    // Basis curve bases
    tf::TfToken bezier();
    tf::TfToken bspline();
    tf::TfToken catmullRom();

    // Wrap modes
    tf::TfToken nonperiodic();
    tf::TfToken periodic();
    tf::TfToken pinned();
}

} // namespace usdGeom
} // namespace cxxfacade
