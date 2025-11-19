# OpenUSD 25.11 Migration Guide

This guide helps you migrate SwiftUSD projects from version **24.8.14** to **25.11**.

## Overview

OpenUSD 25.11 represents a significant upgrade from 24.8.14, spanning **5 major releases** (24.11, 25.02, 25.05, 25.08, 25.11) over 16 months. This migration includes critical security fixes, breaking API changes, new features, and performance improvements.

## Critical Breaking Changes

### 1. Removed File Format Classes (v25.11)

**Impact**: HIGH - These classes have been completely removed from the USD C++ API.

**Removed Classes**:
- `UsdCrateInfo` → Use `SdfCrateInfo` instead
- `UsdUsdFileFormat` → Use `SdfFileFormat` for .usd files
- `UsdUsdaFileFormat` → Use `SdfFileFormat` for .usda files
- `UsdUsdcFileFormat` → Use `SdfFileFormat` for .usdc files
- `UsdUsdzFileFormat` → Use `SdfFileFormat` for .usdz files
- `UsdZipFile` → Use `SdfZipFile` instead

**Migration**:

```swift
// OLD (24.8.14):
import PixarUSD

let crateInfo = UsdCrateInfo.Open("file.usdc")
let zipFile = UsdZipFile.Open("package.usdz")

// NEW (25.11):
import PixarUSD

let crateInfo = SdfCrateInfo.Open("file.usdc")
let zipFile = SdfZipFile.Open("package.usdz")
```

**Why this change?**:
These classes were redundant - USD's layered architecture already provides these capabilities through the Sdf layer. Moving them to Sdf improves consistency and reduces API surface area.

### 2. Removed Ndr Library (v25.08)

**Impact**: MEDIUM - Ndr (Node Definition Registry) has been completely removed.

**Replacement**: Use `Sdr` (Shader Definition Registry) instead.

**Migration**:

```swift
// OLD (24.8.14):
import PixarUSD

let ndrRegistry = NdrRegistry.GetInstance()
let node = ndrRegistry.GetNodeByIdentifier("MyShader")

// NEW (25.11):
import PixarUSD

let sdrRegistry = SdrRegistry.GetInstance()
let node = sdrRegistry.GetShaderNodeByIdentifier("MyShader")
```

**Why this change?**:
Ndr was deprecated in favor of the more shader-focused Sdr library. Sdr provides better support for modern shading workflows.

### 3. SdfPropertySpec API Changes (v25.02)

**Impact**: MEDIUM - Method moved to more specific subclass.

**Removed**: `SdfPropertySpec::GetTimeSampleMap()`

**Replacement**: `SdfAttributeSpec::GetTimeSampleMap()`

**Migration**:

```swift
// OLD (24.8.14):
let propSpec: SdfPropertySpec = ...
let samples = propSpec.GetTimeSampleMap()

// NEW (25.11):
let attrSpec: SdfAttributeSpec = ...
let samples = attrSpec.GetTimeSampleMap()
```

**Why this change?**:
Time samples only apply to attributes, not relationships. Moving the method to `SdfAttributeSpec` makes the API more type-safe.

### 4. Negative Layer Offset Scale Removed (v25.11)

**Impact**: LOW - Only affects advanced composition workflows.

**Removed**: Support for negative `layerOffset.scale` values

**Migration**:

If you were using negative scale values for layer offsets, you'll need to restructure your composition hierarchy. This was an obscure feature with limited use cases.

```swift
// OLD (24.8.14):
let layerOffset = SdfLayerOffset(offset: 0.0, scale: -1.0)  // Negative scale

// NEW (25.11):
// Negative scale is no longer supported - restructure your layer composition
let layerOffset = SdfLayerOffset(offset: 0.0, scale: 1.0)
```

## Deprecations (Still Work, But Will Warn)

### 1. Legacy UI Metadata (v25.11)

**Impact**: LOW - Old metadata still works but is deprecated.

**Deprecated**:
- `displayName` metadata field
- `displayGroup` metadata field
- `hidden` metadata field

**New Approach**: Use the `uiHints` dictionary with structured UI metadata classes.

**Migration**:

```swift
// OLD (24.8.14):
attribute.SetMetadata("displayName", "My Attribute")
attribute.SetMetadata("displayGroup", "Transform")
attribute.SetMetadata("hidden", false)

// NEW (25.11):
let hints = UsdUIPropertyHints(attribute)
hints.SetDisplayName("My Attribute")
hints.SetDisplayGroup("Transform")
hints.SetHidden(false)
```

**Environment Variables** for transition period:
- `SDF_LEGACY_UI_HINTS_WARN_ON_WRITE` (default: false) - Warn when writing legacy metadata
- `USD_POPULATE_LEGACY_ALLOWED_TOKENS` (default: true) - Keep legacy tokens registered

### 2. Hydra Material Network Schema (v25.11)

**Impact**: LOW - Old API still works but is deprecated.

**Deprecated**: `HdMaterialNetworkSchema::GetInterfaceMappings()`

**New API**: `HdMaterialNetworkSchema::GetInterface()`

**Migration**:

```swift
// OLD (24.8.14):
let network: HdMaterialNetworkSchema = ...
let mappings = network.GetInterfaceMappings()

// NEW (25.11):
let network: HdMaterialNetworkSchema = ...
let interface = network.GetInterface()
let params = interface.GetParameters()
```

## New Features & APIs

### 1. UI Hints Framework (v25.11)

**New Classes**:
- `UsdUIObjectHints` - Base class for UI hints
- `UsdUIPrimHints` - UI hints for prims
- `UsdUIPropertyHints` - UI hints for properties
- `UsdUIAttributeHints` - UI hints for attributes

**Features**:
- Conditional visibility with `shownIf` (using `SdfBooleanExpression`)
- Value label customization
- Display group expansion state
- Min/max value constraints via `limits` metadata

**Example**:

```swift
import PixarUSD

// Get hints for an attribute
let attr = prim.GetAttribute(TfToken("radius"))
let hints = UsdUIAttributeHints(attr)

// Set display properties
hints.SetDisplayName("Sphere Radius")
hints.SetDisplayGroup("Geometry")
hints.SetTooltip("The radius of the sphere in world units")

// Set value constraints
attr.SetMetadata("limits", VtDictionary([
  "min": 0.1,
  "max": 100.0
]))

// Conditional visibility
hints.SetShownIf("$prim:type == 'Sphere'")
```

### 2. Validation Framework with Automated Fixing (v25.11)

**New Classes**:
- `UsdValidationContext` - Validation execution context
- `UsdValidationFixer` - Automated error correction
- `UsdValidationError::metadata` - Fixer data passing
- Domain-specific validators (physics, rendering, etc.)

**Example**:

```swift
import PixarUSD

// Run validation
let context = UsdValidationContext()
let errors = context.Validate(stage)

// Automatically fix errors
for error in errors {
  if let fixer = error.GetFixer() {
    fixer.Fix(error)
  }
}
```

### 3. Animation & Spline Enhancements (v25.08, v25.11)

**New Classes**:
- `SdfAnimationBlock` - Explicit animation opinions
- `TsSpline::Breakdown()` - Insert knots while minimizing changes
- `TsSpline::BakeInnerLoops()` - Bake loop regions
- `TsSpline::Sample()` - Piecewise-linear approximation

**New Methods**:
- `UsdAttribute::AnimationBlock()` - Create animation blocks
- `SdfAttributeSpec::GetSpline()` / `SetSpline()` / `ClearSpline()` - Spline management

**Example**:

```swift
import PixarUSD

// Create animation block
let attr = prim.CreateAttribute(TfToken("visibility"), SdfValueTypeNames.Token)
attr.AnimationBlock()  // Mark as animated

// Work with splines
let spline = TsSpline()
spline.SetKnots(/* knot data */)

// Break down spline at specific time
spline.Breakdown(time: 24.0, amount: 0.5)

// Bake loops for export
let bakedSpline = spline.BakeInnerLoops()
```

### 4. Color Space Management (v25.05)

**New Classes**:
- `UsdColorSpaceAPI` - Specify subtree color space
- `UsdColorSpaceDefinitionAPI` - Define custom color spaces

**Example**:

```swift
import PixarUSD

// Apply color space to a subtree
let colorSpaceAPI = UsdColorSpaceAPI.Apply(prim)
colorSpaceAPI.GetColorSpaceAttr().Set(TfToken("lin_rec709"))

// Define custom color space
let colorSpaceDef = UsdColorSpaceDefinitionAPI.Apply(prim, name: TfToken("myColorSpace"))
colorSpaceDef.GetFamilyAttr().Set(TfToken("raw"))
```

### 5. VtArray Enhancements (v25.08)

**New Methods**:
- `VtArray::MakeUnique()` - Enforce copy-on-write semantics
- `VtArray::insert()` - Insert elements mid-array

**Example**:

```swift
import PixarUSD

var points = VtVec3fArray([Vec3f(0, 0, 0), Vec3f(1, 1, 1)])

// Ensure unique ownership before mutation
points.MakeUnique()

// Insert new point
points.insert(Vec3f(0.5, 0.5, 0.5), at: 1)
```

### 6. Camera Exposure Controls (v25.05)

**New Attributes** on `UsdGeomCamera`:
- `exposure` - EV100-based exposure
- `shutterClose` - Shutter timing
- `fStop` - Aperture size

**Example**:

```swift
import PixarUSD

let camera = UsdGeomCamera.Define(stage, path: "/World/MainCamera")
camera.GetExposureAttr().Set(0.0)  // EV100 = 0
camera.GetFStopAttr().Set(2.8)     // f/2.8 aperture
```

### 7. Scalar Transform Ops (v25.05)

**New Transform Operations**:
- `xformOp:translateX`, `translateY`, `translateZ` - Individual axis translation
- `xformOp:scaleX`, `scaleY`, `scaleZ` - Individual axis scale

**Example**:

```swift
import PixarUSD

let xform = UsdGeomXformable(prim)

// Add individual axis transforms
let translateX = xform.AddTranslateOp(precision: .Float, opSuffix: TfToken("X"))
let scaleY = xform.AddScaleOp(precision: .Float, opSuffix: TfToken("Y"))

translateX.Set(10.0)  // Move only on X axis
scaleY.Set(2.0)       // Scale only on Y axis
```

### 8. Nested Rigid Bodies in UsdPhysics (v25.11)

**New Features**:
- Nested rigid bodies support
- Articulation for kinematic bodies
- Enhanced physics validation with fixers

**Example**:

```swift
import PixarUSD

// Create nested rigid body hierarchy
let parentBody = UsdPhysicsRigidBodyAPI.Apply(parentPrim)
let childBody = UsdPhysicsRigidBodyAPI.Apply(childPrim)

// Child inherits parent's motion but can have local physics
parentBody.GetKinematicEnabledAttr().Set(false)  // Dynamic parent
childBody.GetKinematicEnabledAttr().Set(true)    // Kinematic child
```

## Build System & Dependency Updates

### CMake Minimum Version (v25.05)

**Old**: CMake 3.14+
**New**: CMake 3.26+

### Critical Security Fix

**OpenEXR 3.4** includes fix for CVE-2023-5841 (security vulnerability).
**Action**: Update immediately if processing untrusted EXR files.

### Emscripten/WebAssembly Support (v25.11)

USD can now compile to WebAssembly for browser-based USD viewers.
Limited to `pxr/base` and subset of C++ tests.

### Python Support

**Added**: Python 3.13 support in PyPI wheels (v25.08)

## File Format Version Changes

### USDC Default Version (v25.05)

**Changed**: Default for new .usdc files reverted to v0.8.0 (was v0.10.0 in 24.11).
**Override**: Set `USD_WRITE_NEW_USDC_FILES_AS_VERSION` environment variable.

### New Format Support (v25.08)

**Binary v0.13.0**: Adds Hermite curve and spline support.
**ASCII v1.1**: Adds spline syntax to .usda format.

## Performance Improvements

### Specializes Arc Reimplementation (v25.08)

**~21% memory reduction** for scenes using `specializes` composition arc.
No API changes required - automatically benefits from internal improvements.

## Hydra 2.0 Changes

### Default Engine (v25.11)

UsdImaging now **defaults to Hydra 2** (scene index-based rendering).

**Environment Variable** to revert:
```bash
export USDIMAGINGGL_ENGINE_ENABLE_TASK_SCENE_INDEX=0
```

### New Scene Index Features (v25.05+)

- `UsdImagingStageSceneIndex` feature-complete with `UsdImagingDelegate`
- Scene index emulation for tasks
- Render pass pruning scene index
- Built-in material schema for overrides

## Testing & Validation

### UsdChecker Modernization (v25.11)

Now uses new validation framework by default.

```bash
# New behavior (validates with fixers):
usdchecker scene.usda

# Old behavior:
usdchecker --useOldComplianceCheckerInterface scene.usda
```

## SwiftUSD-Specific Considerations

### Resource Bundling

New modules require plugin registration. Ensure you call:

```swift
Pixar.Bundler.shared.setup(.resources)
```

before any USD operations.

### Swift 6.0/6.1 Compatibility

SwiftUSD 25.11 maintains full compatibility with Swift 6.0 and 6.1:
- Uses correct `.interoperabilityMode(.Cxx)`
- No experimental features required
- Swift 6.2 features remain optional

### Potential Issues

**Large Value Types**: New Hydra 2.0 schemas may trigger Swift 6.0/6.1 `.pointee` copying issues. Test thoroughly on visionOS/iOS.

**VtArray Copy-on-Write**: New `MakeUnique()` aligns perfectly with Swift's value semantics - use it!

## Migration Checklist

- [ ] Update Package.swift dependency: `from: "24.8.14"` → `from: "25.11"`
- [ ] Replace `UsdCrateInfo` with `SdfCrateInfo`
- [ ] Replace file format classes with `SdfFileFormat`
- [ ] Replace `UsdZipFile` with `SdfZipFile`
- [ ] Replace `Ndr` usage with `Sdr`
- [ ] Update `SdfPropertySpec::GetTimeSampleMap()` calls
- [ ] Migrate legacy UI metadata to `uiHints` (optional, but recommended)
- [ ] Test with new validation framework
- [ ] Review use of negative layer offset scale (if any)
- [ ] Update OpenEXR dependency for security fix
- [ ] Test Hydra 2.0 rendering (now default)
- [ ] Verify resource bundling works with new modules

## Getting Help

- **SwiftUSD Issues**: https://github.com/wabiverse/SwiftUSD/issues
- **OpenUSD Docs**: https://openusd.org/release/
- **SwiftUSD Docs**: https://wabiverse.github.io/SwiftUSD/documentation/pixarusd/
- **Discord**: https://discord.gg/bfW4NyKpuA

## Additional Resources

- **OpenUSD Changelog**: https://github.com/PixarAnimationStudios/OpenUSD/blob/release/CHANGELOG.md
- **OpenUSD Releases**: https://github.com/PixarAnimationStudios/OpenUSD/releases
- **VFX Reference Platform**: Updated to CY2023 in v25.08

---

**Last Updated**: November 2025
**SwiftUSD Version**: 25.11
**OpenUSD Version**: 25.11
