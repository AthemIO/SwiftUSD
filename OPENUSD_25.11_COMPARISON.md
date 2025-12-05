# SwiftUSD vs OpenUSD 25.11 Comprehensive Comparison Report

**Generated**: December 5, 2025
**SwiftUSD Version**: 25.11.15 (SWIFTUSD_EVOLUTION: 15)
**OpenUSD Version**: 25.11 (PXR_VERSION: 2511)

---

## Executive Summary

SwiftUSD is well-aligned with OpenUSD 25.11, including the new **OpenExec framework**. The primary gap is the **complete absence of the usdValidation framework**, a new validation system introduced in OpenUSD 25.11. File count differences in other modules are primarily due to excluded Python bindings (`PXR_PYTHON_SUPPORT_ENABLED=0`).

### Alignment Status
| Category | Status |
|----------|--------|
| Base Libraries | :white_check_mark: Aligned |
| USD Core | :white_check_mark: Aligned |
| USD Schemas | :white_check_mark: Aligned |
| Imaging/Hydra | :white_check_mark: Aligned |
| OpenExec | :white_check_mark: Integrated |
| usdValidation | :x: **MISSING** |

---

## 1. Module Comparison

### 1.1 Base Libraries (pxr/base/)

| Module | OpenUSD | SwiftUSD | OpenUSD .cpp | SwiftUSD .cpp | Notes |
|--------|---------|----------|--------------|---------------|-------|
| **Arch** | pxr/base/arch | Sources/Arch | 23 | 23 | Aligned |
| **Tf** | pxr/base/tf | Sources/Tf | 137 | 105 | -32 (Python bindings) |
| **Gf** | pxr/base/gf | Sources/Gf | 126 | 57 | -69 (Python bindings) |
| **Vt** | pxr/base/vt | Sources/Vt | 26 | 12 | -14 (Python bindings) |
| **Work** | pxr/base/work | Sources/Work | - | - | Present |
| **Plug** | pxr/base/plug | Sources/Plug | - | - | Present |
| **Trace** | pxr/base/trace | Sources/Trace | - | - | Present |
| **Js** | pxr/base/js | Sources/Js | - | - | Present |
| **Ts** | pxr/base/ts | Sources/Ts | - | - | Present |
| **Pegtl** | pxr/base/pegtl | Sources/Pegtl | - | - | Present |

### 1.2 USD Core (pxr/usd/)

| Module | OpenUSD .cpp | SwiftUSD .cpp | Difference | Notes |
|--------|--------------|---------------|------------|-------|
| **Ar** | - | - | - | Present |
| **Sdf** | 146 | 107 | -39 | Python bindings excluded |
| **Pcp** | 60 | 40 | -20 | Python bindings excluded |
| **Usd** | 107 | 73 | -34 | Python bindings excluded |
| **Kind** | - | - | - | Present |
| **Ndr** | - | - | - | Present |
| **Sdr** | - | - | - | Present |

### 1.3 USD Schema Libraries (pxr/usd/)

| Module | OpenUSD .cpp | SwiftUSD .cpp | Difference | Notes |
|--------|--------------|---------------|------------|-------|
| **UsdGeom** | 81 | 42 | -39 | Python bindings excluded |
| **UsdShade** | 31 | 17 | -14 | Python bindings excluded |
| **UsdLux** | 48 | 25 | -23 | Python bindings excluded |
| **UsdSkel** | 38 | 21 | -17 | Python bindings excluded |
| **UsdPhysics** | 41 | 20 | -21 | Python bindings excluded |
| **UsdVol** | 13 | 6 | -7 | Python bindings excluded |
| **UsdMedia** | 7 | 3 | -4 | Python bindings excluded |
| **UsdRender** | 14 | 8 | -6 | Python bindings excluded |
| **UsdProc** | 5 | 2 | -3 | Python bindings excluded |
| **UsdHydra** | 6 | 3 | -3 | Python bindings excluded |
| **UsdUI** | 19 | 9 | -10 | Python bindings excluded |
| **UsdRi** | 12 | 7 | -5 | Python bindings excluded |
| **UsdSemantics** | 7 | 3 | -4 | Python bindings excluded |
| **UsdUtils** | - | - | - | Present |
| **UsdMtlx** | 13 | 10 | -3 | Python bindings excluded |

### 1.4 Imaging/Hydra (pxr/imaging/)

| Module | OpenUSD .cpp | SwiftUSD .cpp | OpenUSD .h | SwiftUSD .h | Status |
|--------|--------------|---------------|------------|-------------|--------|
| **Hd** | 194 | 192 | 204 | 202 | :white_check_mark: Aligned |
| **HdSt** | 122 | 121 | 127 | 126 | :white_check_mark: Aligned |
| **Hgi** | 25 | 24 | 31 | 30 | :white_check_mark: Aligned |
| **HgiMetal** | 19 | 19 | 20 | 21 | :white_check_mark: Aligned |
| **HgiGL** | 23 | 23 | - | - | :white_check_mark: Aligned |
| **HgiVulkan** | 27 | 28 | - | - | :white_check_mark: Aligned |
| **Hdx** | 37 | 35 | 41 | 39 | :white_check_mark: Aligned |
| **HdSi** | 30 | 30 | 33 | 33 | :white_check_mark: Aligned |
| **HdGp** | - | - | - | - | Present |
| **HdMtlx** | - | - | - | - | Present |
| **HdAr** | - | - | - | - | Present |
| **Hf** | - | - | - | - | Present |
| **Glf** | 23 | 17 | - | - | -6 (Python) |
| **Hio** | 10 | 11 | - | - | :white_check_mark: Aligned |
| **HioOpenVDB** | - | - | - | - | Present |
| **PxOsd** | 10 | 5 | - | - | -5 (Python) |
| **Garch** | - | - | - | - | Present |
| **GeomUtil** | - | - | - | - | Present |
| **CameraUtil** | - | - | - | - | Present |

### 1.5 USD Imaging (pxr/usdImaging/)

| Module | OpenUSD .cpp | SwiftUSD .cpp | Status |
|--------|--------------|---------------|--------|
| **UsdImaging** | 121 | 124 | :white_check_mark: Aligned |
| **UsdImagingGL** | 8 | 2 | -6 (Python) |
| **UsdSkelImaging** | 30 | 30 | :white_check_mark: Aligned |
| **UsdVolImaging** | - | - | Present |
| **UsdProcImaging** | - | - | Present |
| **UsdRiPxrImaging** | - | - | Present |
| **UsdViewQ** | 6 | 3 | -3 (Python) |
| **UsdAppUtils** | - | - | Present |

### 1.6 OpenExec Framework (pxr/exec/) - NEW IN 25.11

| Module | OpenUSD .cpp | SwiftUSD .cpp | OpenUSD .h | SwiftUSD .h | Status |
|--------|--------------|---------------|------------|-------------|--------|
| **Exec** | 46 | 46 | 57 | 2 | :white_check_mark: Integrated |
| **Vdf** | 84 | 84 | 133 | 134 | :white_check_mark: Integrated |
| **Ef** | 15 | 15 | 23 | 24 | :white_check_mark: Integrated |
| **Esf** | 9 | 9 | 13 | 14 | :white_check_mark: Integrated |
| **EsfUsd** | 8 | 8 | 10 | 11 | :white_check_mark: Integrated |
| **ExecGeom** | 2 | 2 | 3 | 5 | :white_check_mark: Integrated |
| **ExecUsd** | 5 | 6 | 8 | 3 | :white_check_mark: Integrated |

**SwiftUSD has fully integrated the OpenExec framework!**

---

## 2. MISSING Modules

### 2.1 usdValidation Framework :x:

The **entire usdValidation module tree** is missing from SwiftUSD:

| OpenUSD Module | Location | Description |
|----------------|----------|-------------|
| **usdValidation** | `pxr/usdValidation/usdValidation/` | Core validation framework (36 cpp, 24 h) |
| **usdGeomValidators** | `pxr/usdValidation/usdGeomValidators/` | Geometry validators |
| **usdPhysicsValidators** | `pxr/usdValidation/usdPhysicsValidators/` | Physics validators |
| **usdShadeValidators** | `pxr/usdValidation/usdShadeValidators/` | Shading validators |
| **usdSkelValidators** | `pxr/usdValidation/usdSkelValidators/` | Skeletal validators |
| **usdUtilsValidators** | `pxr/usdValidation/usdUtilsValidators/` | Utility validators |

**Impact**: SwiftUSD cannot leverage USD's new validation APIs for:
- Scene validation and error detection
- Automatic fixing of common issues
- Custom validator plugins
- Integration with asset pipelines

---

## 3. SwiftUSD-Specific Additions

These modules exist in SwiftUSD but NOT in upstream OpenUSD:

| Module | Purpose |
|--------|---------|
| **PixarUSD** | Swift wrapper library with typealiases and extensions |
| **PixarMacros** | Swift macros for USD interop |
| **UsdView** | Swift-native USD viewer application |
| **Examples** | Swift example code including OpenExec benchmarks |
| **MaterialX** | MaterialX integration module |
| **SdrGlslfx** | GLSLFX shader parser plugin |
| **SdrOsl** | OSL shader parser plugin |
| **HdStorm** | Storm renderer plugin |
| **UsdAbc** | Alembic file format support |
| **UsdDraco** | Draco compression support |
| **UsdShaders** | Built-in USD shaders |

---

## 4. VFX Platform Dependencies

### OpenUSD 25.11 Requirements

| Library | Required Version | MetaverseKit Version | Status |
|---------|------------------|---------------------|--------|
| **TBB** | 2021.x | 2021.10.0 | :white_check_mark: |
| **OpenSubdiv** | 3.6.1 | 3.6.0 | :warning: Update needed |
| **OpenImageIO** | 2.5.x | 2.5.4.0 | :white_check_mark: |
| **OpenColorIO** | 2.3.x | 2.3.0 | :white_check_mark: |
| **OpenVDB** | 10.x | 10.1.0 | :white_check_mark: |
| **Alembic** | 1.8.x | 1.8.5 | :white_check_mark: |
| **OpenEXR** | 3.2.x | 3.2.1 | :white_check_mark: |
| **Imath** | 3.1.x | 3.1.9 | :white_check_mark: |
| **MaterialX** | 1.38.x | 1.38.8 | :white_check_mark: |
| **Ptex** | 2.4.x | 2.4.2 | :white_check_mark: |
| **Draco** | 1.5.x | 1.5.6 | :white_check_mark: |
| **Eigen** | 3.4.x | 3.4.0 | :white_check_mark: |

### MetaverseKit Update Recommendation
- **OpenSubdiv**: Update from 3.6.0 to 3.6.1 (per OpenUSD 25.11 changelog)

---

## 5. New Features in OpenUSD 25.11

### 5.1 OpenExec Framework :white_check_mark:
- **Status**: Fully integrated in SwiftUSD
- **Purpose**: General-purpose framework for computational behaviors in USD scenes
- **Architecture**: Directed acyclic graph (DAG) for dataflow computation
- **Origin**: Pixar's internal "Presto Execution System"
- **Modules**: Exec, Vdf, Ef, Esf, EsfUsd, ExecGeom, ExecUsd

### 5.2 usdValidation Framework :x:
- **Status**: NOT integrated in SwiftUSD
- **Purpose**: Scene validation, error detection, automatic fixes
- **Use Cases**: Asset pipeline validation, scene debugging, compliance checking

### 5.3 API Changes in 25.11
- Removed deprecated Usd file format items (UsdCrateInfo, UsdUsdFileFormat, etc.)
- Deprecated `displayName`, `displayGroup`, `hidden` metadata in favor of `UsdUIObjectHints`
- UsdImaging now defaults to Hydra 2 scene index code path
- TsSpline updates with new knot baking methods

---

## 6. Directory Structure Comparison

### OpenUSD 25.11
```
pxr/
├── base/           # 11 modules (arch, gf, js, pegtl, plug, tf, trace, ts, vt, work)
├── exec/           # 7 modules (ef, esf, esfUsd, exec, execGeom, execUsd, vdf) [NEW]
├── external/       # boost
├── imaging/        # 22 modules
├── usd/            # 24 modules
├── usdImaging/     # 11 modules
└── usdValidation/  # 6 modules [NEW - MISSING in SwiftUSD]
```

### SwiftUSD
```
Sources/
├── Base modules        # 11 (Arch, Tf, Gf, Vt, Work, Plug, Trace, Js, Ts, Pegtl, pxr)
├── OpenExec modules    # 7 (Exec, Vdf, Ef, Esf, EsfUsd, ExecGeom, ExecUsd) [PRESENT]
├── USD modules         # 24+ (Sdf, Pcp, Usd, UsdGeom, UsdShade, etc.)
├── Imaging modules     # 22+ (Hd, HdSt, Hgi, HgiMetal, Hdx, etc.)
├── UsdImaging modules  # 11+ (UsdImaging, UsdImagingGL, UsdSkelImaging, etc.)
├── Swift-specific      # PixarUSD, PixarMacros, UsdView, Examples
├── Plugins             # SdrGlslfx, SdrOsl, HdStorm, UsdShaders
└── Format support      # UsdAbc, UsdDraco, MaterialX
```

---

## 7. Recommendations

### Priority 1: Critical
| Task | Description | Effort |
|------|-------------|--------|
| Add usdValidation | Integrate the new validation framework | High |

### Priority 2: Important
| Task | Description | Effort |
|------|-------------|--------|
| Update OpenSubdiv | Update MetaverseKit from 3.6.0 to 3.6.1 | Low |
| Verify headers | Some modules show header count differences | Medium |

### Priority 3: Maintenance
| Task | Description | Effort |
|------|-------------|--------|
| Review Python exclusions | Confirm all wrap*.cpp exclusions are intentional | Low |
| Deprecated API migration | Update code using deprecated metadata | Medium |

---

## 8. Integration Steps for usdValidation

To add the missing usdValidation framework:

### Step 1: Download Sources
```bash
# From OpenUSD 25.11
cp -r ~/dev/OpenUSD/pxr/usdValidation/* Sources/
```

### Step 2: Create Module Structure
```
Sources/
├── UsdValidation/
│   ├── include/UsdValidation/
│   │   └── UsdValidation.h  # Umbrella header
│   └── *.cpp
├── UsdGeomValidators/
├── UsdPhysicsValidators/
├── UsdShadeValidators/
├── UsdSkelValidators/
└── UsdUtilsValidators/
```

### Step 3: Fix Swift Compatibility
1. Apply Fix 7 (Python macros) to any `TF_PY_*` calls
2. Apply Fix 8 (`<cmath>`) if needed
3. Create umbrella headers for each module

### Step 4: Update Package.swift
Add targets for each validation module with appropriate dependencies.

### Step 5: Add Swift Wrappers
Create `Sources/PixarUSD/Validation/` with Swift-friendly APIs.

---

## 9. Conclusion

SwiftUSD is **well-aligned** with OpenUSD 25.11 for the core USD functionality. The integration of the OpenExec framework demonstrates the project's commitment to staying current with upstream changes. The primary gap is the **usdValidation framework**, which should be prioritized for integration to provide complete feature parity.

### Summary
- :white_check_mark: 73+ modules aligned
- :white_check_mark: OpenExec framework integrated
- :x: usdValidation framework missing (6 modules)
- :warning: OpenSubdiv version update recommended
