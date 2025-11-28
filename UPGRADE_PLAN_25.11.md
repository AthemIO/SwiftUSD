# SwiftUSD OpenUSD 25.11 Upgrade Implementation Plan

**Document Version:** 1.0
**Target:** OpenUSD 25.11
**Current:** OpenUSD 24.08 (SWIFTUSD_EVOLUTION 14)
**Date:** 2025-11-28

---

## Executive Summary

This document provides a comprehensive implementation plan for upgrading SwiftUSD from OpenUSD 24.08 to OpenUSD 25.11. The upgrade involves:

- Version constant updates
- Source synchronization from OpenUSD 25.11 release
- Removal of deprecated APIs that were deleted in 25.11
- Swift compatibility fixes for new code
- MetaverseKit dependency updates (if required)
- Swift wrapper updates for new APIs
- Comprehensive testing and validation

**Estimated Timeline:** 2-3 weeks
**Risk Level:** Medium-High (Breaking API changes)

---

## Research Summary

### OpenUSD 25.11 Release Information

**Release Date:** October 24, 2024
**VFX Platform Alignment:** CY2025 (partial)

**Key Sources:**
- [OpenUSD 25.11 Release Notes](https://openusd.org/release/index.html)
- [OpenUSD Changelog](https://github.com/PixarAnimationStudios/OpenUSD/blob/release/CHANGELOG.md)
- [VFX Reference Platform](https://vfxplatform.com/)
- [OpenUSD 24.08 Announcement](https://aousd.org/blog/new-release-of-openusd/)

### Major Changes in 25.11

#### 1. Breaking API Changes

**File Format Utilities (CRITICAL - REMOVED):**
- ❌ `UsdCrateInfo` → Moved to `SdfCrateInfo`
- ❌ `UsdUsdFileFormat` → Moved to `SdfUsdFileFormat`
- ❌ `UsdUsdaFileFormat` → Moved to `SdfUsdaFileFormat`
- ❌ `UsdUsdcFileFormat` → Moved to `SdfUsdcFileFormat`
- ❌ `UsdUsdzFileFormat` → Moved to `SdfUsdzFileFormat`
- ❌ `UsdZipFile` → Moved to `SdfZipFile`

**Other Removals:**
- ❌ `SdfPropertySpec::GetTimeSampleMap()` → Use `SdfAttributeSpec::GetTimeSampleMap()`
- ❌ `TfTemplateString` utility class removed
- ❌ `TraceEventId` removed (previously deprecated)
- ❌ `PCP_DISABLE_TIME_SCALING_BY_LAYER_TCPS` flag removed
- ❌ Negative layer offset scale support removed
- ❌ `HdPrimDataSourceOverlayCache` removed (Hydra)
- ❌ `HdSceneIndexAdapterSceneDelegate::AppendDefaultSceneFilters()` removed

**Hydra API Changes:**
- HD_API_VERSION bumped from 84 to 85
- `GetInterfaceMappings()` deprecated in favor of `GetInterface()`

#### 2. New Features & APIs

**UI Hints System:**
- ✅ New `uiHints` dictionary metadata (replaces `displayName`, `displayGroup`, `hidden`)
- ✅ `UsdUIObjectHints`, `UsdUIPrimHints`, `UsdUIPropertyHints`, `UsdUIAttributeHints` classes
- ✅ `SdfBooleanExpression` for conditional UI visibility

**Spline Enhancements (Ts module):**
- ✅ `TsSpline::BakeInnerLoops()`
- ✅ `TsSpline::GetKnotsWithInnerLoopsBaked()`
- ✅ `TsSpline::GetKnotsWithLoopsBaked()`
- ✅ `TsSpline::Breakdown()` for knot insertion

**Validation Framework:**
- ✅ `UsdValidationFixer` for applying fixes
- ✅ Metadata member on `UsdValidationError`
- ✅ MaterialBindingAPI validator with auto-fix

**Array Handling:**
- ✅ `VtArray::MakeUnique()` for ensuring unique data copies
- ✅ `VtArrayEdit` support in .usda/.usdc files
- ✅ `arraySizeConstraint` metadata for `UsdAttribute`

**Material Interface (Hydra):**
- ✅ `HdMaterialInterfaceSchema`
- ✅ `HdMaterialInterfaceParameterSchema`
- ✅ `GetReverseInterfaceMappings()`
- ✅ `parameterValues` data source in `MaterialOverrideSchema`

**Graphics:**
- ✅ Cubemap support in Storm renderer
- ✅ UMA and ReBAR support for HgiVulkan
- ✅ Colored debug groups in HgiVulkan

**Variable Expressions:**
- ✅ `SdfVariableExpression::Make...` family
- ✅ `SdfVariableExpressionAST`

**Physics:**
- ✅ Nested rigid bodies support
- ✅ Updated mass computation for kinematic bodies

#### 3. Build Dependency Updates

**Updated Libraries:**
- OpenSubdiv: 3.6.0 → **3.6.1** ✅
- MaterialX: 1.38.8 → **1.39.3** ✅ (REQUIRED)
- Vulkan SDK: → **1.4.321.0** ✅
- OpenEXR (bundled with Hydra): → **3.4** ✅

**Build System:**
- Restructured monolithic build (Windows compatibility)
- Emscripten compiler support (pxr/base subset)
- `BUILD_SHARED_LIBS` variable support

**VFX Platform CY2025 Alignment:**
- Python 3.11 (reverted from 3.12 due to Qt 6.5 incompatibility) ✅
- OneTBB (Intel's oneAPI TBB) ✅
- C++17 standard ✅

### Current MetaverseKit State

**Version:** 1.8.5
**Repository:** `~/dev/Athem/MetaverseKit` (LynrAI fork)

**Current Library Versions:**
| Library | Current | OpenUSD 25.11 Requires | Status |
|---------|---------|------------------------|--------|
| OneTBB | 2021.10.0 | OneTBB (oneAPI) | ✅ Compatible |
| OpenSubdiv | 3.6.0 | 3.6.1 | ⚠️ Update needed |
| OpenImageIO | master (a2f044a) | 2.5.4+ | ✅ Likely compatible |
| OpenColorIO | 2.3.0 | 2.3.0+ | ✅ Compatible |
| OpenEXR | 3.2.1 | 3.4 (Hydra) | ⚠️ Consider update |
| Imath | 3.1.9 | 3.1.9+ | ✅ Compatible |
| MaterialX | 1.38.8 | **1.39.3** | ❌ **MUST UPDATE** |
| OpenVDB | 10.1.0 | 10.1.0+ | ✅ Compatible |
| Alembic | 1.8.5 | 1.8.5+ | ✅ Compatible |
| Ptex | 2.4.2 | 2.4.2+ | ✅ Compatible |
| Draco | 1.5.6 | 1.5.6+ | ✅ Compatible |

**Recommendation:**
- **REQUIRED:** MaterialX 1.38.8 → 1.39.3 (OpenUSD 25.11 explicitly requires 1.39.3)
- **REQUIRED:** OpenSubdiv 3.6.0 → 3.6.1
- **Optional:** OpenEXR 3.2.1 → 3.4 (for Hydra improvements)

**MaterialX 1.39.3 Breaking Changes:**
- C++17 minimum (was C++14) - Already satisfied by SwiftUSD
- CMake 3.24+ minimum
- `swizzle`, `arrayappend` nodes removed (automatic upgrade available)
- `atan2` node inputs renamed (`iny`/`inx` → `in1`/`in2`)
- New: OpenPBR Surface, Disney Principled shading models

---

## Phase 1: Pre-Upgrade Preparation

**Complexity:** Low
**Duration:** 1-2 days
**Dependencies:** None

### 1.1 Create Backup Branch

**Action:**
```bash
cd /Users/martistaerfeldt/dev/Athem/SwiftUSD
git checkout -b backup-24.08
git tag v24.08-EVOLUTION-14
git push origin backup-24.08
git push origin v24.08-EVOLUTION-14
git checkout main
```

**Verification:**
- Backup branch created
- Tag created for rollback capability

### 1.2 Document Current API Usage

**Files to analyze:**
```bash
# Search for all usage of deprecated APIs
grep -r "UsdCrateInfo\|UsdUsdFileFormat\|UsdUsdaFileFormat\|UsdUsdcFileFormat\|UsdUsdzFileFormat\|UsdZipFile" Sources/
grep -r "GetTimeSampleMap\|TfTemplateString\|TraceEventId" Sources/
```

**Output:** Create `DEPRECATED_API_USAGE.md` documenting:
- All occurrences of deprecated APIs
- Replacement strategy for each
- Risk assessment (Swift wrappers affected?)

### 1.3 Update MetaverseKit Dependencies (REQUIRED)

**Location:** `/Users/martistaerfeldt/dev/Athem/MetaverseKit`

**Priority Updates:**
1. **MaterialX 1.38.8 → 1.39.3** (REQUIRED - OpenUSD 25.11 mandates this)
2. **OpenSubdiv 3.6.0 → 3.6.1** (Required)
3. **OpenEXR 3.2.1 → 3.4** (Recommended for Hydra improvements)

---

#### 1.3.1 MaterialX 1.39.3 Update (CRITICAL)

**Why Required:** OpenUSD 25.11 explicitly requires MaterialX 1.39.3. Building with 1.38.x will cause:
- Compilation errors
- Runtime crashes from ABI mismatches
- Missing shader generation features
- Metal shader errors

**Files to update:**

1. **Replace source directory:**
   ```bash
   cd ~/dev/Athem/MetaverseKit

   # Backup
   mv Sources/MaterialX Sources/MaterialX.backup

   # Download MaterialX 1.39.3
   wget https://github.com/AcademySoftwareFoundation/MaterialX/archive/refs/tags/v1.39.3.tar.gz
   tar xzf v1.39.3.tar.gz

   # Copy sources (adapt directory structure as needed)
   mkdir -p Sources/MaterialX
   cp -r MaterialX-1.39.3/source/* Sources/MaterialX/
   cp -r MaterialX-1.39.3/resources Sources/MXResources/Resources/
   ```

2. **Update version constants in `Sources/MaterialX/include/MaterialX/MXCoreGenerated.h`:**
   ```cpp
   #define MATERIALX_MAJOR_VERSION 1
   #define MATERIALX_MINOR_VERSION 39
   #define MATERIALX_BUILD_VERSION 3
   namespace MaterialX_v1_39_3
   ```

3. **Verify C++17 support:**
   MaterialX 1.39 requires C++17 (SwiftUSD already uses C++17, so this should be fine)

**MaterialX API Changes to Handle:**
- `swizzle` node removed → Use `separate3`/`combine3`
- `atan2` inputs renamed: `iny`/`inx` → `in1`/`in2`
- Namespace change: `MaterialX_v1_38_8` → `MaterialX_v1_39_3`

**Test MaterialX independently:**
```bash
cd ~/dev/Athem/MetaverseKit
swift build --target MaterialX
```

---

#### 1.3.2 OpenSubdiv 3.6.1 Update

**Changes in MetaverseKit/Package.swift:**
```swift
// Update OpenSubdiv section (around line 378-387)
.target(
  name: "OpenSubdiv",
  dependencies: [
    .target(name: "MetaTBB"),
    .target(name: "OpenMP", condition: .when(platforms: Arch.OS.nodroid.platform)),
  ],
  exclude: getConfig(for: .osd).exclude,
  publicHeadersPath: "include",
  cxxSettings: getConfig(for: .osd).cxxSettings
)
```

**Process:**
1. Download OpenSubdiv 3.6.1 sources
2. Replace `Sources/OpenSubdiv/` contents
3. Update version in README.md (line 55)
4. Test build: `swift build --target OpenSubdiv`

---

#### 1.3.3 Full MetaverseKit Testing

**After all dependency updates:**
```bash
cd ~/dev/Athem/MetaverseKit

# Full build test
swift build -c release

# Run MetaversalDemo if available
swift bundler run -p macOS MetaversalDemo

# Commit changes
git add .
git commit -m "Update MaterialX to 1.39.3 and OpenSubdiv to 3.6.1 for OpenUSD 25.11"
git push origin main
```

### 1.4 Create VERSIONS.md in MetaverseKit

**File:** `/Users/martistaerfeldt/dev/Athem/MetaverseKit/VERSIONS.md`

**Content:**
```markdown
# MetaverseKit Library Versions

## VFX Reference Platform Alignment: CY2025

| Library | Version | VFX Platform CY2025 | Notes |
|---------|---------|---------------------|-------|
| OneTBB | 2021.10.0 | ✅ | Intel oneAPI TBB |
| OpenSubdiv | 3.6.1 | ✅ | Updated for OpenUSD 25.11 |
| OpenEXR | 3.4 | ✅ | Security & performance fixes |
| Imath | 3.1.9 | ✅ | |
| OpenImageIO | master (a2f044a) | ✅ | |
| OpenColorIO | 2.3.0 | ✅ | |
| MaterialX | 1.38.8 | ✅ | |
| OpenVDB | 10.1.0 | ✅ | |
| Alembic | 1.8.5 | ✅ | |
| Ptex | 2.4.2 | ✅ | |
| Draco | 1.5.6 | ✅ | |

## Compatibility

- OpenUSD: 25.11+
- Swift: 5.10+
- C++: C++17
- Python: 3.11 (VFX Platform CY2025)
```

---

## Phase 2: Version Constants Update

**Complexity:** Low
**Duration:** 0.5 days
**Dependencies:** Phase 1 complete

### 2.1 Update pxrns.h

**File:** `/Users/martistaerfeldt/dev/Athem/SwiftUSD/Sources/pxr/include/pxr/pxrns.h`

**Changes:**
```cpp
// Line 17-21: Update version constants
#define PXR_MAJOR_VERSION 0
#define PXR_MINOR_VERSION 25      // Changed from 24
#define PXR_PATCH_VERSION 11      // Changed from 8
#define PXR_VERSION 2511          // Changed from 2408

// Line 32: Increment evolution
#define SWIFTUSD_EVOLUTION 15     // Changed from 14
```

**Verification:**
```bash
grep "PXR_VERSION\|SWIFTUSD_EVOLUTION" Sources/pxr/include/pxr/pxrns.h
```

### 2.2 Update Swift Version String

**File:** `/Users/martistaerfeldt/dev/Athem/SwiftUSD/Sources/PixarUSD/Pixar.swift`

**Change:**
```swift
// Update version string to "25.11.15"
public static let version = "25.11.15"
```

### 2.3 Update Documentation

**File:** `/Users/martistaerfeldt/dev/Athem/SwiftUSD/CLAUDE.md`

**Changes:**
- Line 9: Update "Current OpenUSD Version Target: 25.11"
- Line 18: Update version constant examples
- Line 223-231: Update version tracking example

---

## Phase 3: OpenUSD Source Synchronization

**Complexity:** High
**Duration:** 3-5 days (reduced with upgrade tool)
**Dependencies:** Phase 2 complete

### 3.0 Enable and Use the OpenUSD Upgrade Tool (RECOMMENDED)

SwiftUSD includes a built-in upgrade tool at `Sources/OpenUSD/` that automates source synchronization.

**First, enable the tool in Package.swift:**

The OpenUSD target is currently commented out. Uncomment lines ~1733-1756:
```swift
.executableTarget(
  name: "OpenUSD",
  dependencies: [
    .product(name: "ArgumentParser", package: "swift-argument-parser"),
    .product(name: "Version", package: "Version"),
    .product(name: "Rainbow", package: "Rainbow"),
    .product(name: "Logging", package: "swift-log"),
  ],
  resources: [
    .copy("Resources")
  ]
),
```

**Run the automated upgrade:**
```bash
cd /Users/martistaerfeldt/dev/Athem/SwiftUSD

# Build the upgrade tool
swift build --product OpenUSD

# Run the upgrade (clones from Pixar's repo and updates sources)
swift run OpenUSD update
```

**What the tool does automatically:**
1. Clones official OpenUSD repository to `.build/OpenUSD/`
2. Updates all modules in parallel:
   - `pxr/base/*` → `Sources/` (Arch, Tf, Gf, Vt, Work, Plug, Trace, Js, Pegtl)
   - `pxr/imaging/*` → `Sources/` (Hd, HdSt, Hgi*, Hio, etc.)
   - `pxr/usd/*` → `Sources/` (Ar, Sdf, Pcp, Usd, UsdGeom, etc.)
   - `pxr/usdImaging/*` → `Sources/` (UsdImaging, UsdImagingGL, etc.)
3. Applies SwiftUSD patches automatically:
   - Fixes `pxr/pxr.h` → `pxr/pxrns.h`
   - Converts TBB headers to MetaverseKit paths (`<tbb/>` → `<OneTBB/tbb/>`)
   - Converts MaterialX headers to MetaverseKit format
   - Adds `noexcept` to destructors for RefPtr compatibility
4. Generates umbrella headers for each module
5. Preserves custom patches from `Sources/OpenUSD/Resources/`

**Add patches for 25.11-specific fixes:**

Create patch files in `Sources/OpenUSD/Resources/<Target>/` to override upstream files:
```bash
# Example: If schema.h needs Swift compatibility fix
mkdir -p Sources/OpenUSD/Resources/Usd/
cp Sources/Usd/include/Usd/schema.h Sources/OpenUSD/Resources/Usd/schema.h
# Edit the patch file with your fixes
```

**Note:** The tool currently clones from `main` branch. For a specific version, you may need to:
```bash
cd .build/OpenUSD
git checkout v25.11
cd ../..
# Re-run without the clone step
```

---

### 3.1 Manual Download (Alternative Method)

If not using the upgrade tool:

**Action:**
```bash
# Download from GitHub releases
cd /tmp
wget https://github.com/PixarAnimationStudios/OpenUSD/archive/refs/tags/v25.11.tar.gz
tar -xzf v25.11.tar.gz
```

### 3.2 Module-by-Module Synchronization

**Strategy:** For each module, preserve SwiftUSD-specific modifications while updating to 25.11 sources.

**Critical Modules with Deprecated APIs (Priority 1):**

1. **Usd** - Contains removed file format classes
   - Files affected: ~35 files (from grep results)
   - Replace: `crateInfo.cpp`, `usd*FileFormat.cpp`, `zipFile.cpp`, etc.
   - Preserve: Swift compatibility fixes (TF_FATAL_ERROR format strings)

2. **Sdf** - New home for file format utilities
   - Add: `SdfCrateInfo`, `SdfUsd*FileFormat`, `SdfZipFile`
   - Update: `propertySpec.cpp` (GetTimeSampleMap removal)

3. **Tf** - TfTemplateString removed
   - Remove: `templateString.cpp`, `include/Tf/templateString.h`
   - Update: Dependent code

4. **Trace** - TraceEventId removed
   - Update: Remove deprecated TraceEventId usage

**All Modules to Update (in dependency order):**

**Base Libraries:**
1. Arch
2. Tf
3. Gf
4. Vt
5. Work
6. Plug
7. Trace
8. Js
9. Pegtl

**USD Core:**
10. Ar
11. Sdf (CRITICAL - new file format utilities)
12. Pcp
13. Usd (CRITICAL - file format utilities moved)
14. Kind
15. Ndr
16. Sdr

**USD Schemas:**
17. UsdGeom
18. UsdShade
19. UsdLux
20. UsdSkel
21. UsdPhysics (NEW FEATURES)
22. UsdVol
23. UsdMedia
24. UsdUI (NEW FEATURES - UI hints)
25. UsdRender
26. UsdRi
27. UsdProc
28. UsdHydra
29. UsdUtils

**USD Format Plugins:**
30. UsdAbc
31. UsdDraco
32. UsdMtlx

**Imaging/Hydra:**
33. Garch
34. Hf
35. Hd (CRITICAL - API_VERSION 85)
36. HdAr
37. HdGp
38. HdMtlx
39. HdSi
40. HdSt
41. HdStorm
42. Hdx
43. Hgi
44. HgiGL
45. HgiMetal
46. HgiInterop
47. Hio
48. HioOpenVDB
49. Glf
50. PxOsd
51. CameraUtil
52. GeomUtil

**USD Imaging:**
53. UsdImaging
54. UsdImagingGL
55. UsdShaders
56. UsdAppUtils
57. UsdProcImaging
58. UsdRiPxrImaging
59. UsdSkelImaging
60. UsdVolImaging
61. UsdViewQ

**Process for Each Module:**
```bash
#!/bin/bash
MODULE="ModuleName"
OPENUSD_SRC="/tmp/OpenUSD-25.11/pxr/..."
SWIFTUSD_SRC="/Users/martistaerfeldt/dev/Athem/SwiftUSD/Sources/$MODULE"

# 1. Backup current SwiftUSD sources
cp -r "$SWIFTUSD_SRC" "$SWIFTUSD_SRC.backup"

# 2. Copy new OpenUSD sources
cp -r "$OPENUSD_SRC"/* "$SWIFTUSD_SRC/"

# 3. Re-apply SwiftUSD-specific fixes (manual review required)
# - TF_FATAL_ERROR format string conversions
# - Swift module compatibility fixes
# - Platform-specific guards

# 4. Update umbrella header if new headers added
# Edit: Sources/$MODULE/include/$MODULE/$MODULE.h

# 5. Test compilation
cd /Users/martistaerfeldt/dev/Athem/SwiftUSD
swift bundler build -c release
```

**Key Files to Watch:**
- `umbrella.h` files in each module
- `CMakeLists.txt` → Not used by SwiftUSD, but useful for finding new files
- `plugInfo.json` → Must be copied to `Resources/` directory

### 3.3 Apply Swift Compatibility Fixes

**Common Fixes Needed:**

**Fix 1: TF_FATAL_ERROR Format Strings**
```cpp
// Before (iostream style - fails in Swift modules)
TF_FATAL_ERROR("Key " << key << " not found");

// After (printf style - works in Swift modules)
TF_FATAL_ERROR("Key '%s' not found", key.c_str());
```

**Files likely needing this fix:** Search for `TF_FATAL_ERROR` with `<<`
```bash
grep -r "TF_FATAL_ERROR.*<<" Sources/
```

**Fix 2: Bool Type Conflicts**
- Watch for types named `Bool` that conflict with Swift's `Bool`
- Rename to `CrateBool`, `UsdBool`, etc.

**Fix 3: Forward Declarations**
- Ensure full type definitions are visible in headers
- Swift modules cannot handle forward declarations across boundaries

**Fix 4: Platform-Specific Code**
```cpp
#if defined(__APPLE__)
  // macOS/iOS/visionOS specific
#elif defined(_WIN32)
  // Windows specific
#elif defined(__linux__)
  // Linux specific
#endif
```

**Fix 5: Umbrella Header Updates**

Each module's umbrella header must include all public headers:

Example: `Sources/Usd/include/Usd/Usd.h`
```cpp
#ifndef PXR_USD_USD_USD_H
#define PXR_USD_USD_USD_H

// Include all public headers (alphabetically)
#include <Usd/attribute.h>
#include <Usd/attributeQuery.h>
// ... all other headers ...
#include <Usd/validator.h>  // NEW in 25.11

// Exclude headers that cannot be made Swift-compatible
// Document WHY each is excluded
// #include <Usd/problematicHeader.h>  // Excluded: complex template metaprogramming

#endif // PXR_USD_USD_USD_H
```

---

## Phase 4: Deprecated API Removal and Migration

**Complexity:** High
**Duration:** 2-3 days
**Dependencies:** Phase 3 complete

### 4.1 File Format Utilities Migration

**Affected Files (35 total):**

**C++ Sources:**
- `Sources/Usd/crateInfo.cpp` → Move to `Sources/Sdf/crateInfo.cpp`
- `Sources/Usd/usd*FileFormat.cpp` → Move to `Sources/Sdf/`
- `Sources/Usd/zipFile.cpp` → Move to `Sources/Sdf/zipFile.cpp`

**Headers:**
- `Sources/Usd/include/Usd/crateInfo.h` → Remove (replaced by Sdf version)
- `Sources/Usd/include/Usd/usd*FileFormat.h` → Remove
- `Sources/Usd/include/Usd/zipFile.h` → Remove

**Update All References:**

Files with references (from grep):
```
Sources/UsdUtils/usdzPackage.cpp
Sources/UsdUtils/assetLocalizationPackage.cpp
Sources/UsdMtlx/fileFormat.cpp
Sources/UsdDraco/fileFormat.cpp
Sources/UsdAbc/alembicFileFormat.cpp
Sources/Usd/usdzFileFormat.cpp
Sources/Usd/usdcFileFormat.cpp
Sources/Usd/usdaFileFormat.cpp
Sources/Usd/usdFileFormat.cpp
Sources/Usd/stage.cpp
Sources/Usd/clipSet.cpp
Sources/Usd/clip.cpp
Sources/Plugin/fileFormat.cpp
Sources/Plugin/alembicFileFormat.cpp
Sources/Extras/usd/examples/usdRecursivePayloadsExample/fileFormat.cpp
Sources/Extras/usd/examples/usdObj/fileFormat.cpp
Sources/Extras/usd/examples/usdDancingCubesExample/fileFormat.cpp
Sources/Bin/usdcat.cpp
Sources/Usd/usdzResolver.cpp
Sources/Usd/Resources/plugInfo.json
```

**Migration Pattern:**
```cpp
// Before
#include <Usd/crateInfo.h>
UsdCrateInfo info;

// After
#include <Sdf/crateInfo.h>
SdfCrateInfo info;
```

**Automated Migration Script:**
```bash
#!/bin/bash
# Run from SwiftUSD root

# Replace includes
find Sources/ -type f \( -name "*.cpp" -o -name "*.h" \) -exec sed -i '' \
  -e 's|#include <Usd/crateInfo.h>|#include <Sdf/crateInfo.h>|g' \
  -e 's|#include <Usd/usdFileFormat.h>|#include <Sdf/usdFileFormat.h>|g' \
  -e 's|#include <Usd/usdaFileFormat.h>|#include <Sdf/usdaFileFormat.h>|g' \
  -e 's|#include <Usd/usdcFileFormat.h>|#include <Sdf/usdcFileFormat.h>|g' \
  -e 's|#include <Usd/usdzFileFormat.h>|#include <Sdf/usdzFileFormat.h>|g' \
  -e 's|#include <Usd/zipFile.h>|#include <Sdf/zipFile.h>|g' \
  {} \;

# Replace class names
find Sources/ -type f \( -name "*.cpp" -o -name "*.h" \) -exec sed -i '' \
  -e 's/\bUsdCrateInfo\b/SdfCrateInfo/g' \
  -e 's/\bUsdUsdFileFormat\b/SdfUsdFileFormat/g' \
  -e 's/\bUsdUsdaFileFormat\b/SdfUsdaFileFormat/g' \
  -e 's/\bUsdUsdcFileFormat\b/SdfUsdcFileFormat/g' \
  -e 's/\bUsdUsdzFileFormat\b/SdfUsdzFileFormat/g' \
  -e 's/\bUsdZipFile\b/SdfZipFile/g' \
  {} \;

echo "Migration complete. Review changes with: git diff"
```

**Verification:**
```bash
# Ensure no old references remain
grep -r "UsdCrateInfo\|UsdUsdFileFormat\|UsdUsdaFileFormat\|UsdUsdcFileFormat\|UsdUsdzFileFormat\|UsdZipFile" Sources/ --include="*.cpp" --include="*.h"
# Should return no results
```

### 4.2 Remove TfTemplateString

**Files to update:**

**Remove:**
- `Sources/Tf/templateString.cpp`
- `Sources/Tf/include/Tf/templateString.h`

**Update Package.swift:**
```swift
// Remove from Tf target sources if explicitly listed
// Check target definition around line ~800
```

**Find usages:**
```bash
grep -r "TfTemplateString\|#include <Tf/templateString.h>" Sources/
```

**Migration:** If any usages found, replace with `std::string` or `TfStringPrintf`.

### 4.3 Remove TraceEventId

**Files to check:**
- `Sources/Trace/` module

**Action:** OpenUSD 25.11 sources should already have this removed. Verify:
```bash
grep -r "TraceEventId" Sources/Trace/
# Should return no results after Phase 3 sync
```

### 4.4 Update SdfPropertySpec::GetTimeSampleMap

**File:** `Sources/Sdf/propertySpec.cpp`

**Change:** Replace `SdfPropertySpec::GetTimeSampleMap()` with `SdfAttributeSpec::GetTimeSampleMap()`

**Verification:**
```bash
grep -r "SdfPropertySpec::GetTimeSampleMap" Sources/
# Should return no results
```

### 4.5 Update Hydra API Version

**Files to check:**
- All Hydra-related modules (Hd, HdSt, HdSi, etc.)

**Look for:**
```cpp
#define HD_API_VERSION 84
```

**Update to:**
```cpp
#define HD_API_VERSION 85
```

**Replace GetInterfaceMappings with GetInterface:**
```bash
# Find usages
grep -r "GetInterfaceMappings" Sources/Hd* Sources/UsdImaging/
# Update to GetInterface() as appropriate
```

### 4.6 Remove Python Bindings for Deprecated APIs

**Directory:** `Python/PyUsd/`

**Remove files:**
- `Python/PyUsd/wrapCrateInfo.cpp` (if exists)
- `Python/PyUsd/wrapUsdFileFormat.cpp` (if exists)
- `Python/PyUsd/wrapZipFile.cpp` (if exists)

**Update:** `Python/PyUsd/module.cpp` to remove references

**Note:** SwiftUSD has `PXR_PYTHON_SUPPORT_ENABLED=0`, so Python bindings may already be excluded. Verify:
```bash
ls Python/PyUsd/
# Check if directory exists and contains deprecated wrappers
```

---

## Phase 5: New API Additions

**Complexity:** Medium
**Duration:** 2-3 days
**Dependencies:** Phase 4 complete

### 5.1 UI Hints System (UsdUI)

**New Classes in OpenUSD 25.11:**
- `UsdUIObjectHints`
- `UsdUIPrimHints`
- `UsdUIPropertyHints`
- `UsdUIAttributeHints`

**Action:**
1. Verify these are included in `Sources/UsdUI/include/UsdUI/UsdUI.h` umbrella header
2. Test Swift interop compilation

**New in Sdf:**
- `SdfBooleanExpression` for conditional UI hints

**Verify:** `Sources/Sdf/include/Sdf/Sdf.h` includes `booleanExpression.h`

### 5.2 Spline Enhancements (Ts)

**Note:** Ts module is currently NOT in Package.swift (line 59 of CLAUDE.md notes it exists but is not built)

**Decision Point:** Should we enable Ts module?

**If YES:**
1. Add Ts module to Package.swift
2. Resolve any Swift compatibility issues (reason it was excluded originally)
3. Add Swift wrappers for new APIs

**If NO:**
- Document in CLAUDE.md that Ts module remains disabled

**New APIs (if enabled):**
- `TsSpline::BakeInnerLoops()`
- `TsSpline::GetKnotsWithInnerLoopsBaked()`
- `TsSpline::GetKnotsWithLoopsBaked()`
- `TsSpline::Breakdown()`

### 5.3 Validation Framework (Usd)

**New Classes:**
- `UsdValidationFixer`
- Enhanced `UsdValidationError` with metadata

**Verify in:**
- `Sources/Usd/include/Usd/Usd.h` umbrella header
- Look for `validator.h`, `validationFixer.h`, `validationError.h`

### 5.4 Array Enhancements (Vt)

**New APIs:**
- `VtArray::MakeUnique()`
- `VtArrayEdit` support

**Verify in:**
- `Sources/Vt/include/Vt/array.h`
- `Sources/Vt/include/Vt/arrayEdit.h`

**New Metadata:**
- `arraySizeConstraint` for `UsdAttribute`

### 5.5 Material Interface (Hydra)

**New Classes:**
- `HdMaterialInterfaceSchema`
- `HdMaterialInterfaceParameterSchema`

**New Methods:**
- `GetReverseInterfaceMappings()`

**New Data Sources:**
- `parameterValues` in `MaterialOverrideSchema`

**Verify in:**
- `Sources/Hd/include/Hd/Hd.h`
- Look for materialInterface-related headers

### 5.6 Variable Expressions (Sdf)

**New APIs:**
- `SdfVariableExpression::Make...` family
- `SdfVariableExpressionAST`

**Verify in:**
- `Sources/Sdf/include/Sdf/Sdf.h`
- Look for `variableExpression.h`

### 5.7 Physics Updates (UsdPhysics)

**New Features:**
- Nested rigid bodies support
- Kinematic body mass computation

**Verify:**
- No specific new classes, but internal implementation changes
- Check `Sources/UsdPhysics/include/UsdPhysics/UsdPhysics.h`

### 5.8 Graphics Enhancements

**Storm Renderer (HdSt):**
- Cubemap support

**HgiVulkan:**
- UMA and ReBAR support (environment variables)
- Colored debug groups

**Verify:**
- Review `Sources/HdSt/` and `Sources/Hgi/` for new capabilities
- These are likely internal and may not need Swift wrappers

---

## Phase 6: Swift Wrapper Updates

**Complexity:** Medium
**Duration:** 3-4 days
**Dependencies:** Phase 5 complete

### 6.1 Update Type Aliases for Moved APIs

**File:** `Sources/PixarUSD/Usd/Sdf.swift`

**Remove (old Usd aliases):**
```swift
// REMOVE - these no longer exist
public typealias UsdCrateInfo = Pixar.UsdCrateInfo
public typealias UsdUsdFileFormat = Pixar.UsdUsdFileFormat
// ... etc
```

**Add (new Sdf aliases):**
```swift
// ADD - new home in Sdf
public extension Sdf {
  typealias CrateInfo = Pixar.SdfCrateInfo
  typealias UsdFileFormat = Pixar.SdfUsdFileFormat
  typealias UsdaFileFormat = Pixar.SdfUsdaFileFormat
  typealias UsdcFileFormat = Pixar.SdfUsdcFileFormat
  typealias UsdzFileFormat = Pixar.SdfUsdzFileFormat
  typealias ZipFile = Pixar.SdfZipFile
}
```

### 6.2 Add UI Hints Wrappers

**File:** `Sources/PixarUSD/Usd/UsdUI.swift` (create if doesn't exist)

```swift
import CxxStdlib
import pxr

public enum UsdUI {}

public extension UsdUI {
  typealias ObjectHints = Pixar.UsdUIObjectHints
  typealias PrimHints = Pixar.UsdUIPrimHints
  typealias PropertyHints = Pixar.UsdUIPropertyHints
  typealias AttributeHints = Pixar.UsdUIAttributeHints
}

// Extensions for Swift-friendly access
public extension UsdUI.PrimHints {
  // Add convenience methods here
}

public extension UsdUI.AttributeHints {
  // Add convenience methods for getting hint values
}
```

### 6.3 Add Boolean Expression Wrappers

**File:** `Sources/PixarUSD/Usd/Sdf.swift`

```swift
public extension Sdf {
  typealias BooleanExpression = Pixar.SdfBooleanExpression
  typealias VariableExpression = Pixar.SdfVariableExpression
  typealias VariableExpressionAST = Pixar.SdfVariableExpressionAST
}

// Extensions for Swift-friendly construction
public extension Sdf.VariableExpression {
  // Wrap Make... family of functions
  static func make(/* parameters */) -> VariableExpression {
    // Call C++ Make... functions
  }
}
```

### 6.4 Add Validation Wrappers

**File:** `Sources/PixarUSD/Usd/Usd.swift`

```swift
public extension Usd {
  typealias ValidationFixer = Pixar.UsdValidationFixer
  typealias ValidationError = Pixar.UsdValidationError
}

// Extensions for Swift-friendly usage
public extension Usd.ValidationFixer {
  // Wrap fixer methods
}

public extension Usd.ValidationError {
  // Convenience accessors for metadata
}
```

### 6.5 Add Array Enhancement Wrappers

**File:** `Sources/PixarUSD/Base/Vt.swift`

```swift
public extension Vt.Array where T: /* appropriate constraint */ {
  // Add MakeUnique() wrapper if needed
  func makeUnique() {
    // Call C++ MakeUnique()
  }
}

public extension Vt {
  typealias ArrayEdit = Pixar.VtArrayEdit
}
```

### 6.6 Update Existing Wrappers

**Review and update:**
- `Sources/PixarUSD/Base/` - Check for any base library changes
- `Sources/PixarUSD/Usd/` - Update USD core wrappers
- `Sources/PixarUSD/Imaging/` - Update Hydra wrappers for API_VERSION 85

**Specific files to check:**
- `Sources/PixarUSD/Imaging/Hd.swift` - Update for GetInterface vs GetInterfaceMappings
- `Sources/PixarUSD/Usd/UsdAttribute.swift` - Add arraySizeConstraint metadata

### 6.7 Update Pixar.swift Version

**File:** `Sources/PixarUSD/Pixar.swift`

```swift
public enum Pixar {
  public static let version = "25.11.15"  // Updated

  // Update any version-dependent logic
}
```

---

## Phase 7: Testing and Validation

**Complexity:** High
**Duration:** 3-5 days
**Dependencies:** Phase 6 complete

### 7.1 Build Verification

**Test 1: Clean Build**
```bash
cd /Users/martistaerfeldt/dev/Athem/SwiftUSD

# Clean build directory
rm -rf .build

# Build with swift bundler
swift bundler build -c release

# Expected: Successful compilation with no errors
```

**Test 2: Module-by-Module Build**
```bash
# Test individual modules
swift build --target Usd
swift build --target Sdf
swift build --target Hd
# ... test all major modules
```

**Test 3: Debug Build**
```bash
swift bundler build -c debug
# Check for any debug-specific issues
```

### 7.2 Run Examples

**Test 1: Examples Target**
```bash
swift bundler run -c release Examples

# Expected: All examples run without crashes
```

**Test 2: UsdView Application**
```bash
swift bundler run -c release UsdView

# Expected: Application launches and can open USD files
```

**Test 3: Create and Open USD File**
```swift
// Test file creation with new API
import PixarUSD

Pixar.Bundler.shared.setup(.resources)

let stage = Usd.Stage.createNew("test_25_11.usda")
UsdGeom.Xform.define(stage, path: "/Root")
UsdGeom.Sphere.define(stage, path: "/Root/Sphere")
stage.save()

// Test file opening
let openedStage = Usd.Stage.open("test_25_11.usda")
print(openedStage.traverse())
```

### 7.3 Run Test Suite

**Test 1: Swift Tests**
```bash
swift bundler test
```

**Test 2: OpenUSD C++ Tests (if ported)**
```bash
# Check if any OpenUSD tests are available
ls Tests/OpenUSDTests/
# Run if available
```

### 7.4 API Migration Verification

**Test deprecated API removal:**
```bash
# These should fail to compile (proving old APIs are gone)
cat > test_deprecated.swift <<EOF
import PixarUSD
let info = UsdCrateInfo()  // Should fail - UsdCrateInfo doesn't exist
EOF

swift build test_deprecated.swift
# Expected: Compilation error - type not found

rm test_deprecated.swift
```

**Test new API availability:**
```bash
cat > test_new_api.swift <<EOF
import PixarUSD

// Test new Sdf location
let info = Sdf.CrateInfo()  // Should succeed

// Test new UI hints
let hints = UsdUI.ObjectHints()  // Should succeed

// Test validation
let fixer = Usd.ValidationFixer()  // Should succeed
EOF

swift build test_new_api.swift
# Expected: Successful compilation
```

### 7.5 Performance Testing

**Test 1: Stage Load Performance**
```swift
import Foundation
import PixarUSD

Pixar.Bundler.shared.setup(.resources)

let start = Date()
let stage = Usd.Stage.open("large_scene.usd")
let elapsed = Date().timeIntervalSince(start)

print("Stage loaded in \(elapsed) seconds")
// Compare with 24.08 baseline
```

**Test 2: Rendering Performance**
```bash
# Test UsdView rendering performance
# Load complex scene and measure FPS
swift bundler run -c release UsdView -- --benchmark
```

### 7.6 Platform Testing

**Test on all platforms:**
1. **macOS 14+** ✅ (primary development)
2. **visionOS 1.0+** ⚠️ (if available)
3. **iOS 17+** ⚠️ (simulator)
4. **tvOS 17+** ⚠️ (simulator)
5. **watchOS 10+** ⚠️ (simulator - limited testing)

**Basic platform test:**
```bash
# macOS
swift bundler build -c release

# iOS Simulator
swift bundler build -c release --platform iOS

# visionOS Simulator
swift bundler build -c release --platform visionOS
```

### 7.7 Memory Testing

**Test 1: Memory Leaks**
```bash
# Run with Instruments
instruments -t Leaks -D leaks_trace.trace \
  .build/release/UsdView

# Check for new leaks introduced in 25.11
```

**Test 2: Reference Counting**
```swift
// Test RefPtr behavior
weak var weakStage: UsdStageRefPtr?
do {
  let stage = Usd.Stage.createNew("temp.usda")
  weakStage = stage
  // stage should be released here
}
assert(weakStage == nil, "Stage should be deallocated")
```

### 7.8 Regression Testing

**Create regression test suite:**
```bash
# Compare outputs between 24.08 and 25.11
# 1. Export scene with 24.08
# 2. Import scene with 25.11
# 3. Verify identical scene graph

diff <(swift_usd_24.08 dump scene.usda) \
     <(swift_usd_25.11 dump scene.usda)
```

**Test specific regressions:**
- File format compatibility (usda, usdc, usdz)
- Material binding (new validators)
- Physics simulation (nested rigid bodies)
- Hydra rendering (Storm cubemaps)

---

## Phase 8: Documentation and Release

**Complexity:** Low
**Duration:** 1-2 days
**Dependencies:** Phase 7 complete

### 8.1 Update CLAUDE.md

**Sections to update:**
1. **Line 9:** "Current OpenUSD Version Target: 25.11"
2. **Architecture section:** Document any new modules enabled (e.g., Ts)
3. **API patterns:** Document new patterns for UI hints, validation, etc.
4. **Upgrade process:** Update with 25.11-specific learnings

### 8.2 Create Migration Guide

**File:** `MIGRATION_24.08_TO_25.11.md`

**Content:**
```markdown
# Migration Guide: OpenUSD 24.08 → 25.11

## Breaking Changes

### File Format Utilities Moved to Sdf

**Before (24.08):**
\`\`\`swift
import PixarUSD
let info = UsdCrateInfo()
\`\`\`

**After (25.11):**
\`\`\`swift
import PixarUSD
let info = Sdf.CrateInfo()
\`\`\`

### Complete API Renames

| Old API (24.08) | New API (25.11) |
|-----------------|-----------------|
| UsdCrateInfo | Sdf.CrateInfo |
| UsdUsdFileFormat | Sdf.UsdFileFormat |
| UsdUsdaFileFormat | Sdf.UsdaFileFormat |
| UsdUsdcFileFormat | Sdf.UsdcFileFormat |
| UsdUsdzFileFormat | Sdf.UsdzFileFormat |
| UsdZipFile | Sdf.ZipFile |

### Removed APIs

- TfTemplateString (no replacement)
- SdfPropertySpec::GetTimeSampleMap() → Use SdfAttributeSpec::GetTimeSampleMap()
- TraceEventId (deprecated, now removed)

## New Features

### UI Hints System
[Examples...]

### Validation Framework
[Examples...]

### Array Enhancements
[Examples...]
```

### 8.3 Update README

**Add to README:**
- OpenUSD 25.11 compatibility
- New features highlight
- Migration guide link

### 8.4 Create Release Notes

**File:** `RELEASE_NOTES_25.11.md`

**Content:**
```markdown
# SwiftUSD 25.11 Release Notes

**Release Date:** [DATE]
**OpenUSD Version:** 25.11
**SwiftUSD Evolution:** 15

## What's New

- ✅ Full OpenUSD 25.11 compatibility
- ✅ UI Hints system support
- ✅ Enhanced validation framework
- ✅ Array manipulation improvements
- ✅ Updated MetaverseKit dependencies
- ✅ OpenSubdiv 3.6.1

## Breaking Changes

**File format utilities moved to Sdf namespace.**
See [Migration Guide](MIGRATION_24.08_TO_25.11.md) for details.

## Bug Fixes

- Fixed TF_FATAL_ERROR format strings for Swift compatibility
- Updated Hydra API_VERSION to 85

## Upgrade Instructions

1. Update Package.swift dependency to SwiftUSD 25.11
2. Review [Migration Guide](MIGRATION_24.08_TO_25.11.md)
3. Update code for API renames (see guide)
4. Test thoroughly

## Known Issues

- [List any known issues]

## Credits

Based on OpenUSD 25.11 by Pixar Animation Studios.
```

### 8.5 Create Git Tag and Release

```bash
cd /Users/martistaerfeldt/dev/Athem/SwiftUSD

# Ensure all changes committed
git add .
git commit -m "Upgrade to OpenUSD 25.11 (SWIFTUSD_EVOLUTION 15)"

# Create tag
git tag -a v25.11-EVOLUTION-15 -m "SwiftUSD OpenUSD 25.11 Release"

# Push
git push origin main
git push origin v25.11-EVOLUTION-15

# Create GitHub release with RELEASE_NOTES_25.11.md
```

---

## Risk Assessment

### High Risk Items

**1. File Format API Migration**
- **Risk:** Breaking existing code that uses `UsdCrateInfo`, etc.
- **Impact:** HIGH - All code using file format utilities will break
- **Mitigation:**
  - Comprehensive migration guide
  - Provide deprecation warnings if possible
  - Automated migration script

**2. Hydra API Version Bump**
- **Risk:** Render delegates may break
- **Impact:** MEDIUM - Custom Hydra integrations affected
- **Mitigation:**
  - Test all Hydra code paths
  - Document GetInterface vs GetInterfaceMappings change

**3. Swift Compatibility Issues**
- **Risk:** New OpenUSD 25.11 code may not compile in Swift modules
- **Impact:** HIGH - Blocks upgrade if unfixable
- **Mitigation:**
  - Incremental module testing during Phase 3
  - Source-level fixes (TF_FATAL_ERROR, etc.)
  - Exclude problematic headers if necessary (document why)

### Medium Risk Items

**4. Build System Changes**
- **Risk:** Monolithic build restructuring may affect SwiftUSD
- **Impact:** MEDIUM - May require Package.swift updates
- **Mitigation:**
  - Review OpenUSD build changes
  - Test on all platforms early

**5. MetaverseKit Updates**
- **Risk:** OpenSubdiv 3.6.1 or OpenEXR 3.4 updates break
- **Impact:** MEDIUM - Blocks SwiftUSD until MetaverseKit fixed
- **Mitigation:**
  - Test MetaverseKit updates independently first
  - Have rollback plan for MetaverseKit

**6. Performance Regressions**
- **Risk:** New code may be slower
- **Impact:** MEDIUM - User experience degradation
- **Mitigation:**
  - Benchmark before/after
  - Profile hot paths
  - Report issues to OpenUSD if found

### Low Risk Items

**7. New Features Not Working**
- **Risk:** New APIs (UI hints, validation) don't work in Swift
- **Impact:** LOW - Can be added incrementally
- **Mitigation:**
  - Mark as experimental initially
  - Gather user feedback

**8. Platform-Specific Issues**
- **Risk:** iOS/visionOS/watchOS specific breaks
- **Impact:** LOW-MEDIUM - Depends on platform importance
- **Mitigation:**
  - Test on simulators early
  - Platform guards where needed

**9. Documentation Gaps**
- **Risk:** Users don't understand migration
- **Impact:** LOW - Support burden increases
- **Mitigation:**
  - Comprehensive migration guide
  - Examples for common patterns

---

## Breaking Changes for Downstream Users

**Severity: HIGH**

### Code Changes Required

All users of SwiftUSD will need to update code that uses:

1. **File format utilities:**
   ```swift
   // Old
   let info = UsdCrateInfo()

   // New
   let info = Sdf.CrateInfo()
   ```

2. **Time sample access:**
   ```swift
   // Old
   let samples = propertySpec.GetTimeSampleMap()

   // New
   let samples = attributeSpec.GetTimeSampleMap()
   ```

### Recommended Communication

**Before Release:**
- [ ] Blog post announcing upcoming 25.11 upgrade
- [ ] Migration guide preview
- [ ] Breaking changes announcement

**At Release:**
- [ ] Detailed migration guide
- [ ] Example code updates
- [ ] Version compatibility matrix

**After Release:**
- [ ] Monitor issues and questions
- [ ] Update FAQ with common migration issues
- [ ] Provide support in community channels

---

## Validation Checklist

Before declaring Phase 7 complete:

- [ ] All modules compile without errors
- [ ] All modules compile without warnings (or warnings documented)
- [ ] Examples run successfully
- [ ] UsdView launches and can open USD files
- [ ] No deprecated API references remain in codebase
- [ ] All new APIs accessible from Swift
- [ ] Tests pass (if test suite exists)
- [ ] No memory leaks introduced
- [ ] Performance within 10% of baseline
- [ ] Documentation updated
- [ ] Migration guide complete
- [ ] Release notes drafted

---

## Rollback Plan

If critical issues are discovered:

### Immediate Rollback (Emergency)

```bash
cd /Users/martistaerfeldt/dev/Athem/SwiftUSD

# Revert to backup branch
git checkout backup-24.08
git checkout -b main-rollback
git push origin main-rollback --force

# Tag the failed attempt
git tag v25.11-FAILED
git push origin v25.11-FAILED
```

### Partial Rollback (Specific Module)

```bash
# Restore single module from backup
git checkout backup-24.08 -- Sources/ModuleName/
git commit -m "Rollback ModuleName to 24.08"
```

### MetaverseKit Rollback

```bash
cd ~/dev/Athem/MetaverseKit
git revert <commit-sha>
# Or restore specific library version
```

---

## Post-Upgrade Tasks

After successful release:

1. **Monitor for Issues**
   - Watch GitHub issues
   - Track Stack Overflow questions
   - Monitor community Discord/Slack

2. **Performance Benchmarking**
   - Collect real-world performance data
   - Compare with 24.08 baseline
   - Optimize hotspots if needed

3. **Documentation Improvements**
   - Add FAQ entries based on user questions
   - Create video tutorials for major changes
   - Update examples repository

4. **Future Planning**
   - Start tracking OpenUSD 26.02 changes
   - Plan for next VFX Platform cycle
   - Identify technical debt introduced during migration

---

## Timeline Summary

| Phase | Duration | Complexity | Can Parallelize |
|-------|----------|------------|-----------------|
| 1. Pre-Upgrade Preparation | 1-2 days | Low | No |
| 2. Version Constants | 0.5 days | Low | No |
| 3. Source Synchronization | 3-5 days | High | Partially (by module) |
| 4. Deprecated API Removal | 2-3 days | High | Partially |
| 5. New API Additions | 2-3 days | Medium | Yes |
| 6. Swift Wrapper Updates | 3-4 days | Medium | Partially |
| 7. Testing & Validation | 3-5 days | High | Partially |
| 8. Documentation & Release | 1-2 days | Low | No |

**Total Estimated Time:** 16-24 days (2.5-3.5 weeks)

**With parallel work:** Could be compressed to 12-18 days (2-2.5 weeks)

---

## Resource Requirements

**Developer Time:**
- 1 primary developer: Full time for 3 weeks
- 1 reviewer: Part time (code review, testing)
- 1 documentation writer: Part time (final week)

**Infrastructure:**
- macOS development machine (primary)
- iOS/visionOS simulators (testing)
- CI/CD system (automated builds)

**Tools:**
- Xcode 15+
- swift-bundler
- Git
- Instruments (performance profiling)

---

## Success Criteria

The upgrade is successful when:

1. ✅ All modules compile without errors
2. ✅ UsdView application runs and can open USD files
3. ✅ Examples run without crashes
4. ✅ Test suite passes (if available)
5. ✅ No memory leaks vs. baseline
6. ✅ Performance within 10% of 24.08 baseline
7. ✅ Documentation complete and accurate
8. ✅ Migration guide tested by external developer
9. ✅ No critical bugs in first week after release
10. ✅ Community feedback is positive

---

## Appendix A: Quick Reference Commands

**Check current version:**
```bash
grep "PXR_VERSION\|SWIFTUSD_EVOLUTION" Sources/pxr/include/pxr/pxrns.h
```

**Build:**
```bash
swift bundler build -c release
```

**Run UsdView:**
```bash
swift bundler run -c release UsdView
```

**Run Examples:**
```bash
swift bundler run -c release Examples
```

**Find deprecated API usage:**
```bash
grep -r "UsdCrateInfo\|UsdUsdFileFormat" Sources/ --include="*.cpp" --include="*.h"
```

**Test single module:**
```bash
swift build --target ModuleName
```

---

## Appendix B: Important File Paths

**SwiftUSD:**
- Version constants: `/Users/martistaerfeldt/dev/Athem/SwiftUSD/Sources/pxr/include/pxr/pxrns.h`
- Package manifest: `/Users/martistaerfeldt/dev/Athem/SwiftUSD/Package.swift`
- Swift wrappers: `/Users/martistaerfeldt/dev/Athem/SwiftUSD/Sources/PixarUSD/`
- Documentation: `/Users/martistaerfeldt/dev/Athem/SwiftUSD/CLAUDE.md`

**MetaverseKit:**
- Package manifest: `/Users/martistaerfeldt/dev/Athem/MetaverseKit/Package.swift`
- README: `/Users/martistaerfeldt/dev/Athem/MetaverseKit/README.md`

---

## Appendix C: External Resources

**OpenUSD Documentation:**
- [OpenUSD 25.11 Docs](https://openusd.org/release/index.html)
- [API Documentation](https://openusd.org/release/apiDocs.html)
- [OpenUSD GitHub](https://github.com/PixarAnimationStudios/OpenUSD)
- [OpenUSD Changelog](https://github.com/PixarAnimationStudios/OpenUSD/blob/release/CHANGELOG.md)

**VFX Platform:**
- [VFX Reference Platform](https://vfxplatform.com/)
- [VFX Platform 2025](https://vfxplatform.com/) (CY2025)

**Alliance for OpenUSD:**
- [AOUSD Website](https://aousd.org/)
- [OpenUSD v24.08 Announcement](https://aousd.org/blog/new-release-of-openusd/)

**Swift/C++ Interop:**
- [Swift C++ Interop Documentation](https://www.swift.org/documentation/cxx-interop/)
- [Swift Evolution Proposals](https://github.com/apple/swift-evolution)

---

## Document History

| Version | Date | Author | Changes |
|---------|------|--------|---------|
| 1.0 | 2025-11-28 | Claude | Initial comprehensive plan |

---

**End of Upgrade Plan**
