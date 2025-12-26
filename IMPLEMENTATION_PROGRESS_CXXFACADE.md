# CxxFacade + SwiftUSD Implementation Progress

Last updated: 2025-12-26 17:00

## Architecture

```
SwiftUSD (Swift API)
    ↓
CxxFacade (C++ pimpl - clean headers)
    ↓
Vendor/USD/*.dylib (Pixar OpenUSD)
```

**Progress**: 11/11 modules


## Phase 0: SETUP

| Module | Status | Types | Description |
|--------|--------|-------|-------------|
| setup | ✅ |  | Create CxxFacade directory structure, module.modul |

## Phase 1: FOUNDATION

| Module | Status | Types | Description |
|--------|--------|-------|-------------|
| Tf | ✅ | Token | TfToken - interned strings used as identifiers thr |
| Gf | ✅ | Vec2f, Vec3f, Vec4f... | Graphics foundation math types |
| Vt | ✅ | IntArray, FloatArray, DoubleArray... | Value type arrays with copy-on-write semantics |

## Phase 2: SCENE_DESCRIPTION

| Module | Status | Types | Description |
|--------|--------|-------|-------------|
| Sdf | ✅ | Path, Layer, AssetPath... | Scene description foundation - paths and layers |
| Kind | ✅ | Registry | Model hierarchy kinds for scene organization |

## Phase 3: USD_CORE

| Module | Status | Types | Description |
|--------|--------|-------|-------------|
| Usd | ✅ | TimeCode, Stage, Prim... | Core USD API - Stage, Prim, Property, Attribute, R |

## Phase 4: USD_SCHEMAS

| Module | Status | Types | Description |
|--------|--------|-------|-------------|
| UsdGeom | ✅ | Xformable, Xform, Scope... | Geometry schemas - transforms, meshes, curves, cam |
| UsdShade | ✅ | Shader, Material, NodeGraph... | Shading schemas - materials, shaders, node graphs |
| UsdLux | ✅ | LightAPI, MeshLightAPI, VolumeLightAPI... | Lighting schemas |
| UsdSkel | ✅ | Skeleton, Root, Animation... | Skeletal animation schemas |

## Phase 5: IMAGING

| Module | Status | Types | Description |
|--------|--------|-------|-------------|
| Hydra | ✅ | RenderSettings, CameraSettings, Engine | High-level Hydra rendering facade for USD scenes |
