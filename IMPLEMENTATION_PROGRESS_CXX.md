# SwiftUSD C++ Interop Implementation Progress

Last updated: 2025-12-25

**Architecture**: 2-layer (Swift → C++ with SWIFT_* annotations)

**Goal**: Complete USD pipeline with Hydra/Storm rendering via Metal/Vulkan

**Progress**: 11/28 modules completed

**Current Focus**: Phase 5 - IMAGING_FOUNDATION (PxOsd)

---

## Phase 0: ARCHITECTURE (1/1)
Setup Package.swift with C++ interop

| Module | Status | Type | Platform | Description |
|--------|--------|------|----------|-------------|
| setup_cxx_interop | ✅ | SHARED_REFERENCE | All | Setup Package.swift with C++ interop, create USDCxx structure |

## Phase 1: FOUNDATION (4/4)
Essential types used everywhere

| Module | Status | Type | Platform | Description |
|--------|--------|------|----------|-------------|
| Arch | ✅ | SELF_CONTAINED | All | Platform detection, timing, filesystem APIs |
| Tf | ✅ | SELF_CONTAINED | All | TfToken (interned strings for identifiers) |
| Gf | ✅ | SELF_CONTAINED | All | Vec2f/3f/4f, Vec3d, Matrix4d, Quatd, Range, BBox, Frustum |
| Vt | ✅ | SHARED_REFERENCE | All | VtArray (IntArray, FloatArray, Vec3fArray, Matrix4dArray) |

## Phase 2: SCENE_DESCRIPTION (2/2)
Layer and path fundamentals

| Module | Status | Type | Platform | Description |
|--------|--------|------|----------|-------------|
| Sdf | ✅ | SHARED_REFERENCE | All | SdfPath (Path value type), SdfLayer (LayerRef reference type) |
| Kind | ✅ | SELF_CONTAINED | All | Model hierarchy kinds (model, group, assembly, component) |

## Phase 3: USD_CORE (1/1)
The heart of USD

| Module | Status | Type | Platform | Description |
|--------|--------|------|----------|-------------|
| Usd | ✅ | SHARED_REFERENCE | All | UsdStage, UsdPrim, UsdAttribute, UsdTimeCode, UsdRelationship |

## Phase 4: USD_SCHEMAS (0/5)
Scene description schemas

| Module | Status | Type | Platform | Description |
|--------|--------|------|----------|-------------|
| UsdGeom | ⏸️ | SHARED_REFERENCE | All | Mesh, Xform, Points, BasisCurves, Camera, Primvars, BoundingBox |
| UsdShade | ⏸️ | SHARED_REFERENCE | All | Material, Shader, MaterialBindingAPI, Input, Output, NodeGraph |
| UsdLux | ⏸️ | SHARED_REFERENCE | All | DistantLight, DomeLight, RectLight, SphereLight, LightFilter |
| UsdSkel | ⏸️ | SHARED_REFERENCE | All | Skeleton, SkelRoot, SkelAnimation, BlendShape, SkelBindingAPI |
| UsdVol | ⏸️ | SHARED_REFERENCE | All | Volume, OpenVDBAsset, Field3DAsset |

## Phase 5: IMAGING_FOUNDATION (3/5)
Graphics abstraction layer

| Module | Status | Type | Platform | Description |
|--------|--------|------|----------|-------------|
| Hgi | ✅ | SHARED_REFERENCE | All | GPU abstraction interface (HgiCmds, HgiBuffer, HgiTexture, HgiPipeline) |
| HgiMetal | ✅ | SHARED_REFERENCE | Apple | Metal implementation of Hgi interfaces |
| HgiVulkan | ⏭️ | SHARED_REFERENCE | Linux/Win | Vulkan implementation of Hgi interfaces |
| Hio | ✅ | SHARED_REFERENCE | All | Image/texture loading (OpenEXR, PNG, JPEG) |
| PxOsd | 🔶 | SHARED_REFERENCE | All | OpenSubdiv integration, subdivision surfaces |

## Phase 6: HYDRA_CORE (0/4)
Render framework

| Module | Status | Type | Platform | Description |
|--------|--------|------|----------|-------------|
| Hd | ⬜ | SHARED_REFERENCE | All | HdRenderIndex, HdRprim, HdSprim, HdBprim, HdSceneDelegate, HdRenderDelegate |
| HdSt | ⬜ | SHARED_REFERENCE | All | Storm renderer (HdStRenderDelegate, HdStMesh, HdStMaterial) |
| Hdx | ⬜ | SHARED_REFERENCE | All | HdxTaskController, HdxRenderTask, HdxSelectionTask, HdxPickTask |
| HdEngine | ⬜ | SHARED_REFERENCE | All | HdEngine for executing render tasks |

## Phase 7: USD_IMAGING (0/4)
USD to Hydra bridge

| Module | Status | Type | Platform | Description |
|--------|--------|------|----------|-------------|
| UsdImaging | ⬜ | SHARED_REFERENCE | All | UsdImagingDelegate, adapters for USD schema types |
| UsdSkelImaging | ⬜ | SHARED_REFERENCE | All | Skeleton visualization, skinning adapters |
| UsdVolImaging | ⬜ | SHARED_REFERENCE | All | Volume rendering adapters |
| UsdAppUtils | ⬜ | SHARED_REFERENCE | All | FrameRecorder, ColorCorrection, camera utilities |

## Phase 8: SWIFT_INTEGRATION (0/2)
Platform-native Swift layer

| Module | Status | Type | Platform | Description |
|--------|--------|------|----------|-------------|
| SwiftRendering | ⬜ | SHARED_REFERENCE | All | HydraRenderer facade, Metal/Vulkan platform views |
| SwiftMath | ⬜ | SELF_CONTAINED | All | SIMD <-> Gf extensions, simd_float3 <-> Vec3f conversions |

---

## Type Mapping Strategy

| USD C++ Type | Swift Type | Annotation |
|--------------|------------|------------|
| TfToken | Token | SWIFT_SELF_CONTAINED |
| SdfPath | Path | SWIFT_SELF_CONTAINED |
| GfVec3f | Vec3f | SWIFT_SELF_CONTAINED |
| GfMatrix4d | Matrix4d | SWIFT_SELF_CONTAINED |
| VtArray<T> | TArray | SWIFT_SHARED_REFERENCE |
| UsdStageRefPtr | StageRef | SWIFT_SHARED_REFERENCE |
| UsdPrim | PrimRef | SWIFT_SHARED_REFERENCE |
| SdfLayerHandle | LayerRef | SWIFT_SHARED_REFERENCE |
| Hgi | HgiRef | SWIFT_SHARED_REFERENCE |

---

## Platform Support

| Platform | GPU Backend | Status |
|----------|-------------|--------|
| macOS | HgiMetal | Primary |
| iOS | HgiMetal | Supported |
| visionOS | HgiMetal | Supported |
| Linux | HgiVulkan | Supported |
| Windows | HgiVulkan | Supported |
