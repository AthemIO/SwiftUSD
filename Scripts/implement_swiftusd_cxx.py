#!/usr/bin/env python3
"""
SwiftUSD C++ Interop Implementation Agent

This script runs Claude Code in a loop to systematically implement
all SwiftUSD modules using the 2-layer C++ interop architecture.

Architecture: Swift → C++ (with SWIFT_* annotations)

Usage:
    python Scripts/implement_swiftusd_cxx.py [--start-phase N] [--module MODULE]
    python Scripts/implement_swiftusd_cxx.py --dry-run  # Preview prompts
    python Scripts/implement_swiftusd_cxx.py --list     # List all modules
    python Scripts/implement_swiftusd_cxx.py --clean    # Remove old architecture
"""

import subprocess
import os
import sys
import shutil
import argparse
import time
import platform
from pathlib import Path
from dataclasses import dataclass, field
from enum import Enum
from typing import Optional, List
from datetime import datetime


class Phase(Enum):
    ARCHITECTURE = 0       # setup_cxx_interop
    FOUNDATION = 1         # Arch, Tf, Gf, Vt
    SCENE_DESCRIPTION = 2  # Sdf, Kind
    USD_CORE = 3           # Usd
    USD_SCHEMAS = 4        # UsdGeom, UsdShade, UsdLux, UsdSkel, UsdVol
    IMAGING_FOUNDATION = 5 # Hgi, HgiMetal, HgiVulkan, Hio, PxOsd
    HYDRA_CORE = 6         # Hd, HdSt, Hdx, HdEngine
    USD_IMAGING = 7        # UsdImaging, UsdSkelImaging, UsdVolImaging, UsdAppUtils
    SWIFT_INTEGRATION = 8  # SwiftUSD.Rendering, SwiftUSD.Math


@dataclass
class Module:
    name: str
    phase: Phase
    category: Optional[str]  # "base", "usd", "imaging", or None for top-level schemas
    dependencies: List[str] = field(default_factory=list)
    description: str = ""
    swift_type: str = "SWIFT_SHARED_REFERENCE"  # or "SWIFT_SELF_CONTAINED"
    platform: str = "all"  # "all", "apple", "linux_windows"
    status: str = "pending"
    completed_at: Optional[str] = None


@dataclass
class ModuleFiles:
    """Expected files for a module implementation."""
    header_files: List[str]     # Required C++ headers in USDCxx
    swift_files: List[str]      # Required Swift files
    test_files: List[str]       # Required test files


@dataclass
class VerificationResult:
    """Result of module verification."""
    files_ok: bool
    build_ok: bool
    tests_ok: bool
    missing_files: List[str]
    build_output: str
    test_output: str

    @property
    def success(self) -> bool:
        return self.files_ok and self.build_ok and self.tests_ok


def is_apple_platform() -> bool:
    """Check if running on Apple platform."""
    return platform.system() == "Darwin"


# Complete module registry in implementation order (28 modules)
# Goal: Complete USD pipeline with Hydra/Storm rendering via Metal/Vulkan
MODULES = [
    # Phase 0: Architecture (1 module)
    Module("setup_cxx_interop", Phase.ARCHITECTURE, "meta", [],
           "Setup Package.swift with C++ interop, create USDCxx structure"),

    # Phase 1: Foundation (4 modules) - Essential types used everywhere
    Module("Arch", Phase.FOUNDATION, "base", [],
           "Platform detection, timing, filesystem APIs",
           swift_type="SWIFT_SELF_CONTAINED"),
    Module("Tf", Phase.FOUNDATION, "base", ["Arch"],
           "TfToken (interned strings for identifiers)",
           swift_type="SWIFT_SELF_CONTAINED"),
    Module("Gf", Phase.FOUNDATION, "base", ["Tf"],
           "Vec2f/3f/4f, Vec3d, Matrix4d, Quatd, Range, BBox, Frustum",
           swift_type="SWIFT_SELF_CONTAINED"),
    Module("Vt", Phase.FOUNDATION, "base", ["Gf"],
           "VtArray (IntArray, FloatArray, Vec3fArray, Matrix4dArray)",
           swift_type="SWIFT_SHARED_REFERENCE"),

    # Phase 2: Scene Description (2 modules) - Layer and path fundamentals
    Module("Sdf", Phase.SCENE_DESCRIPTION, "usd", ["Tf", "Vt"],
           "SdfPath (Path value type), SdfLayer (LayerRef reference type)"),
    Module("Kind", Phase.SCENE_DESCRIPTION, "usd", ["Tf"],
           "Model hierarchy kinds (model, group, assembly, component)",
           swift_type="SWIFT_SELF_CONTAINED"),

    # Phase 3: USD Core (1 module) - The heart of USD
    Module("Usd", Phase.USD_CORE, "usd", ["Sdf"],
           "UsdStage, UsdPrim, UsdAttribute, UsdTimeCode, UsdRelationship"),

    # Phase 4: USD Schemas (5 modules) - Scene description schemas
    Module("UsdGeom", Phase.USD_SCHEMAS, None, ["Usd", "Gf"],
           "Mesh, Xform, Points, BasisCurves, Camera, Primvars, BoundingBox"),
    Module("UsdShade", Phase.USD_SCHEMAS, None, ["Usd", "Sdf"],
           "Material, Shader, MaterialBindingAPI, Input, Output, NodeGraph"),
    Module("UsdLux", Phase.USD_SCHEMAS, None, ["UsdGeom", "UsdShade"],
           "DistantLight, DomeLight, RectLight, SphereLight, LightFilter"),
    Module("UsdSkel", Phase.USD_SCHEMAS, None, ["UsdGeom"],
           "Skeleton, SkelRoot, SkelAnimation, BlendShape, SkelBindingAPI"),
    Module("UsdVol", Phase.USD_SCHEMAS, None, ["UsdGeom"],
           "Volume, OpenVDBAsset, Field3DAsset"),

    # Phase 5: Imaging Foundation (5 modules) - Graphics abstraction layer
    Module("Hgi", Phase.IMAGING_FOUNDATION, "imaging", ["Tf", "Gf"],
           "GPU abstraction interface (HgiCmds, HgiBuffer, HgiTexture, HgiPipeline)"),
    Module("HgiMetal", Phase.IMAGING_FOUNDATION, "imaging", ["Hgi"],
           "Metal implementation of Hgi interfaces",
           platform="apple"),
    Module("HgiVulkan", Phase.IMAGING_FOUNDATION, "imaging", ["Hgi"],
           "Vulkan implementation of Hgi interfaces",
           platform="linux_windows"),
    Module("Hio", Phase.IMAGING_FOUNDATION, "imaging", ["Gf"],
           "Image/texture loading (OpenEXR, PNG, JPEG)"),
    Module("PxOsd", Phase.IMAGING_FOUNDATION, "imaging", ["Gf", "Vt"],
           "OpenSubdiv integration, subdivision surfaces"),

    # Phase 6: Hydra Core (4 modules) - Render framework
    Module("Hd", Phase.HYDRA_CORE, "imaging", ["Hgi", "Gf", "Vt", "Sdf"],
           "HdRenderIndex, HdRprim, HdSprim, HdBprim, HdSceneDelegate, HdRenderDelegate"),
    Module("HdSt", Phase.HYDRA_CORE, "imaging", ["Hd", "Hgi", "Hio", "PxOsd"],
           "Storm renderer (HdStRenderDelegate, HdStMesh, HdStMaterial)"),
    Module("Hdx", Phase.HYDRA_CORE, "imaging", ["HdSt"],
           "HdxTaskController, HdxRenderTask, HdxSelectionTask, HdxPickTask"),
    Module("HdEngine", Phase.HYDRA_CORE, "imaging", ["Hd"],
           "HdEngine for executing render tasks"),

    # Phase 7: USD Imaging (4 modules) - USD to Hydra bridge
    Module("UsdImaging", Phase.USD_IMAGING, "usdImaging", ["Hd", "Usd", "UsdGeom", "UsdShade", "UsdLux"],
           "UsdImagingDelegate, adapters for USD schema types"),
    Module("UsdSkelImaging", Phase.USD_IMAGING, "usdImaging", ["UsdImaging", "UsdSkel"],
           "Skeleton visualization, skinning adapters"),
    Module("UsdVolImaging", Phase.USD_IMAGING, "usdImaging", ["UsdImaging", "UsdVol"],
           "Volume rendering adapters"),
    Module("UsdAppUtils", Phase.USD_IMAGING, "usdImaging", ["UsdImaging"],
           "FrameRecorder, ColorCorrection, camera utilities"),

    # Phase 8: Swift Integration (2 modules) - Platform-native Swift layer
    Module("SwiftRendering", Phase.SWIFT_INTEGRATION, "swift", ["UsdImaging", "Hdx"],
           "HydraRenderer facade, Metal/Vulkan platform views"),
    Module("SwiftMath", Phase.SWIFT_INTEGRATION, "swift", ["Gf"],
           "SIMD <-> Gf extensions, simd_float3 <-> Vec3f conversions",
           swift_type="SWIFT_SELF_CONTAINED"),
]


# ============================================================================
# Embedded Header Templates
# ============================================================================

SWIFT_H_TEMPLATE = '''// Sources/USDCxx/include/Swift.h
// SWIFT_* macro definitions for C++ interop
#pragma once

#if __has_include(<swift/bridging>)
#include <swift/bridging>
#else
// Fallback definitions when not building with Swift
#define SWIFT_SHARED_REFERENCE(retain, release)
#define SWIFT_IMMORTAL_REFERENCE
#define SWIFT_SELF_CONTAINED
#define SWIFT_UNSAFE_REFERENCE
#define SWIFT_RETURNS_INDEPENDENT_VALUE
#define SWIFT_COMPUTED_PROPERTY
#endif

// Error handling
#define SWIFTUSD_NOEXCEPT noexcept

// Platform detection
#if defined(__APPLE__)
    #define SWIFTUSD_PLATFORM_APPLE 1
    #define SWIFTUSD_HGI_METAL 1
#elif defined(__linux__)
    #define SWIFTUSD_PLATFORM_LINUX 1
    #define SWIFTUSD_HGI_VULKAN 1
#elif defined(_WIN32)
    #define SWIFTUSD_PLATFORM_WINDOWS 1
    #define SWIFTUSD_HGI_VULKAN 1
#endif
'''

TF_H_TEMPLATE = '''// Sources/USDCxx/include/base/Tf.h
#pragma once

#include "../Swift.h"
#include <pxr/base/tf/token.h>

namespace swiftusd {

/// TfToken wrapper - immortal (interned strings never freed)
class Token {
public:
    Token() SWIFTUSD_NOEXCEPT : _token() {}
    Token(const char* text) SWIFTUSD_NOEXCEPT : _token(text) {}
    Token(const TfToken& token) SWIFTUSD_NOEXCEPT : _token(token) {}

    const char* GetText() const SWIFTUSD_NOEXCEPT { return _token.GetText(); }
    bool IsEmpty() const SWIFTUSD_NOEXCEPT { return _token.IsEmpty(); }
    size_t Hash() const SWIFTUSD_NOEXCEPT { return _token.Hash(); }

    bool operator==(const Token& other) const SWIFTUSD_NOEXCEPT {
        return _token == other._token;
    }

    const TfToken& GetTfToken() const SWIFTUSD_NOEXCEPT { return _token; }

private:
    TfToken _token;
} SWIFT_SELF_CONTAINED;

} // namespace swiftusd
'''

GF_H_TEMPLATE = '''// Sources/USDCxx/include/base/Gf.h
#pragma once

#include "../Swift.h"
#include <pxr/base/gf/vec2f.h>
#include <pxr/base/gf/vec3f.h>
#include <pxr/base/gf/vec4f.h>
#include <pxr/base/gf/vec3d.h>
#include <pxr/base/gf/matrix4d.h>
#include <pxr/base/gf/quatd.h>
#include <pxr/base/gf/bbox3d.h>

namespace swiftusd {

// Value types - Swift can copy these directly
struct Vec2f {
    float x, y;

    Vec2f() SWIFTUSD_NOEXCEPT : x(0), y(0) {}
    Vec2f(float x, float y) SWIFTUSD_NOEXCEPT : x(x), y(y) {}
    Vec2f(const GfVec2f& v) SWIFTUSD_NOEXCEPT : x(v[0]), y(v[1]) {}

    GfVec2f ToGf() const SWIFTUSD_NOEXCEPT { return GfVec2f(x, y); }
} SWIFT_SELF_CONTAINED;

struct Vec3f {
    float x, y, z;

    Vec3f() SWIFTUSD_NOEXCEPT : x(0), y(0), z(0) {}
    Vec3f(float x, float y, float z) SWIFTUSD_NOEXCEPT : x(x), y(y), z(z) {}
    Vec3f(const GfVec3f& v) SWIFTUSD_NOEXCEPT : x(v[0]), y(v[1]), z(v[2]) {}

    GfVec3f ToGf() const SWIFTUSD_NOEXCEPT { return GfVec3f(x, y, z); }

    static Vec3f Zero() SWIFTUSD_NOEXCEPT { return Vec3f(0, 0, 0); }
    static Vec3f One() SWIFTUSD_NOEXCEPT { return Vec3f(1, 1, 1); }
} SWIFT_SELF_CONTAINED;

struct Vec4f {
    float x, y, z, w;

    Vec4f() SWIFTUSD_NOEXCEPT : x(0), y(0), z(0), w(0) {}
    Vec4f(float x, float y, float z, float w) SWIFTUSD_NOEXCEPT : x(x), y(y), z(z), w(w) {}
    Vec4f(const GfVec4f& v) SWIFTUSD_NOEXCEPT : x(v[0]), y(v[1]), z(v[2]), w(v[3]) {}

    GfVec4f ToGf() const SWIFTUSD_NOEXCEPT { return GfVec4f(x, y, z, w); }
} SWIFT_SELF_CONTAINED;

struct Vec3d {
    double x, y, z;

    Vec3d() SWIFTUSD_NOEXCEPT : x(0), y(0), z(0) {}
    Vec3d(double x, double y, double z) SWIFTUSD_NOEXCEPT : x(x), y(y), z(z) {}
    Vec3d(const GfVec3d& v) SWIFTUSD_NOEXCEPT : x(v[0]), y(v[1]), z(v[2]) {}

    GfVec3d ToGf() const SWIFTUSD_NOEXCEPT { return GfVec3d(x, y, z); }
} SWIFT_SELF_CONTAINED;

struct Matrix4d {
    double m[16];

    Matrix4d() SWIFTUSD_NOEXCEPT {
        for (int i = 0; i < 16; ++i) m[i] = (i % 5 == 0) ? 1.0 : 0.0;
    }

    Matrix4d(const GfMatrix4d& mat) SWIFTUSD_NOEXCEPT {
        const double* data = mat.GetArray();
        for (int i = 0; i < 16; ++i) m[i] = data[i];
    }

    GfMatrix4d ToGf() const SWIFTUSD_NOEXCEPT { return GfMatrix4d(m); }

    static Matrix4d Identity() SWIFTUSD_NOEXCEPT { return Matrix4d(); }
} SWIFT_SELF_CONTAINED;

struct Quatd {
    double real;
    double i, j, k;

    Quatd() SWIFTUSD_NOEXCEPT : real(1), i(0), j(0), k(0) {}
    Quatd(double real, double i, double j, double k) SWIFTUSD_NOEXCEPT
        : real(real), i(i), j(j), k(k) {}
    Quatd(const GfQuatd& q) SWIFTUSD_NOEXCEPT
        : real(q.GetReal()), i(q.GetImaginary()[0]),
          j(q.GetImaginary()[1]), k(q.GetImaginary()[2]) {}

    GfQuatd ToGf() const SWIFTUSD_NOEXCEPT {
        return GfQuatd(real, GfVec3d(i, j, k));
    }

    static Quatd Identity() SWIFTUSD_NOEXCEPT { return Quatd(); }
} SWIFT_SELF_CONTAINED;

struct BBox3d {
    Vec3d min;
    Vec3d max;
    Matrix4d matrix;

    BBox3d() SWIFTUSD_NOEXCEPT : min(), max(), matrix() {}
    BBox3d(const GfBBox3d& bbox) SWIFTUSD_NOEXCEPT
        : min(bbox.GetRange().GetMin()),
          max(bbox.GetRange().GetMax()),
          matrix(bbox.GetMatrix()) {}
} SWIFT_SELF_CONTAINED;

} // namespace swiftusd
'''

VT_H_TEMPLATE = '''// Sources/USDCxx/include/base/Vt.h
#pragma once

#include "../Swift.h"
#include "Gf.h"
#include <pxr/base/vt/array.h>
#include <atomic>
#include <cstring>

namespace swiftusd {

/// Generic array wrapper with zero-copy data access
template<typename T, typename SwiftT>
class ArrayRef {
public:
    ArrayRef() SWIFTUSD_NOEXCEPT : _array(), _refCount(1) {}
    ArrayRef(const VtArray<T>& array) SWIFTUSD_NOEXCEPT : _array(array), _refCount(1) {}
    ArrayRef(VtArray<T>&& array) SWIFTUSD_NOEXCEPT : _array(std::move(array)), _refCount(1) {}

    // Zero-copy data access
    const T* data() const SWIFTUSD_NOEXCEPT { return _array.cdata(); }
    size_t size() const SWIFTUSD_NOEXCEPT { return _array.size(); }
    bool empty() const SWIFTUSD_NOEXCEPT { return _array.empty(); }

    // Element access
    T operator[](size_t index) const SWIFTUSD_NOEXCEPT { return _array[index]; }

    // Mutable access (detaches COW)
    T* mutableData() SWIFTUSD_NOEXCEPT { return _array.data(); }

    // Get underlying VtArray
    const VtArray<T>& GetVtArray() const SWIFTUSD_NOEXCEPT { return _array; }
    VtArray<T>& GetVtArray() SWIFTUSD_NOEXCEPT { return _array; }

    // Reference counting
    void retain() SWIFTUSD_NOEXCEPT {
        _refCount.fetch_add(1, std::memory_order_relaxed);
    }
    void release() SWIFTUSD_NOEXCEPT {
        if (_refCount.fetch_sub(1, std::memory_order_acq_rel) == 1)
            delete this;
    }

private:
    VtArray<T> _array;
    std::atomic<int> _refCount;
};

// Concrete instantiations for common types
class IntArray : public ArrayRef<int, int32_t> {
public:
    using ArrayRef::ArrayRef;

    static IntArray* Create(const int* data, size_t count) {
        VtArray<int> arr(count);
        std::memcpy(arr.data(), data, count * sizeof(int));
        return new IntArray(std::move(arr));
    }
} SWIFT_SHARED_REFERENCE(retain, release);

class FloatArray : public ArrayRef<float, float> {
public:
    using ArrayRef::ArrayRef;

    static FloatArray* Create(const float* data, size_t count) {
        VtArray<float> arr(count);
        std::memcpy(arr.data(), data, count * sizeof(float));
        return new FloatArray(std::move(arr));
    }
} SWIFT_SHARED_REFERENCE(retain, release);

class Vec3fArray : public ArrayRef<GfVec3f, Vec3f> {
public:
    using ArrayRef::ArrayRef;

    static Vec3fArray* Create(const Vec3f* data, size_t count) {
        VtArray<GfVec3f> arr(count);
        for (size_t i = 0; i < count; ++i) {
            arr[i] = data[i].ToGf();
        }
        return new Vec3fArray(std::move(arr));
    }

    Vec3f GetAt(size_t index) const SWIFTUSD_NOEXCEPT {
        return Vec3f((*this)[index]);
    }
} SWIFT_SHARED_REFERENCE(retain, release);

class Matrix4dArray : public ArrayRef<GfMatrix4d, Matrix4d> {
public:
    using ArrayRef::ArrayRef;

    Matrix4d GetAt(size_t index) const SWIFTUSD_NOEXCEPT {
        return Matrix4d((*this)[index]);
    }
} SWIFT_SHARED_REFERENCE(retain, release);

} // namespace swiftusd
'''

SDF_H_TEMPLATE = '''// Sources/USDCxx/include/usd/Sdf.h
#pragma once

#include "../Swift.h"
#include "../base/Tf.h"
#include <pxr/usd/sdf/path.h>
#include <pxr/usd/sdf/layer.h>
#include <atomic>

namespace swiftusd {

/// SdfPath - value type (small, cheap to copy)
struct Path {
    Path() SWIFTUSD_NOEXCEPT : _path() {}
    Path(const char* pathString) SWIFTUSD_NOEXCEPT : _path(pathString) {}
    Path(const SdfPath& path) SWIFTUSD_NOEXCEPT : _path(path) {}

    const char* GetText() const SWIFTUSD_NOEXCEPT { return _path.GetText(); }
    const char* GetName() const SWIFTUSD_NOEXCEPT { return _path.GetName().c_str(); }
    bool IsEmpty() const SWIFTUSD_NOEXCEPT { return _path.IsEmpty(); }
    bool IsAbsolute() const SWIFTUSD_NOEXCEPT { return _path.IsAbsolutePath(); }
    bool IsPrimPath() const SWIFTUSD_NOEXCEPT { return _path.IsPrimPath(); }

    Path GetParentPath() const SWIFTUSD_NOEXCEPT { return Path(_path.GetParentPath()); }
    Path AppendChild(const Token& name) const SWIFTUSD_NOEXCEPT {
        return Path(_path.AppendChild(name.GetTfToken()));
    }
    Path AppendPath(const Path& suffix) const SWIFTUSD_NOEXCEPT {
        return Path(_path.AppendPath(suffix._path));
    }

    bool operator==(const Path& other) const SWIFTUSD_NOEXCEPT {
        return _path == other._path;
    }

    const SdfPath& GetSdfPath() const SWIFTUSD_NOEXCEPT { return _path; }

    static Path AbsoluteRoot() SWIFTUSD_NOEXCEPT { return Path(SdfPath::AbsoluteRootPath()); }
    static Path Empty() SWIFTUSD_NOEXCEPT { return Path(); }

private:
    SdfPath _path;
} SWIFT_SELF_CONTAINED;

/// SdfLayer - reference counted
class LayerRef {
public:
    static LayerRef* CreateNew(const char* identifier) SWIFTUSD_NOEXCEPT;
    static LayerRef* CreateAnonymous(const char* tag = nullptr) SWIFTUSD_NOEXCEPT;
    static LayerRef* FindOrOpen(const char* identifier) SWIFTUSD_NOEXCEPT;

    const char* GetIdentifier() const SWIFTUSD_NOEXCEPT;
    const char* GetDisplayName() const SWIFTUSD_NOEXCEPT;
    bool IsDirty() const SWIFTUSD_NOEXCEPT;
    bool IsAnonymous() const SWIFTUSD_NOEXCEPT;

    bool Save(bool force = false) SWIFTUSD_NOEXCEPT;
    bool Export(const char* path) SWIFTUSD_NOEXCEPT;

    void retain() SWIFTUSD_NOEXCEPT { _refCount.fetch_add(1, std::memory_order_relaxed); }
    void release() SWIFTUSD_NOEXCEPT {
        if (_refCount.fetch_sub(1, std::memory_order_acq_rel) == 1)
            delete this;
    }

    SdfLayerHandle GetSdfLayer() const SWIFTUSD_NOEXCEPT { return _layer; }

private:
    LayerRef(const SdfLayerHandle& layer) SWIFTUSD_NOEXCEPT : _layer(layer), _refCount(1) {}
    SdfLayerHandle _layer;
    std::atomic<int> _refCount;
} SWIFT_SHARED_REFERENCE(retain, release);

} // namespace swiftusd
'''

USD_H_TEMPLATE = '''// Sources/USDCxx/include/usd/Usd.h
#pragma once

#include "../Swift.h"
#include "../base/Tf.h"
#include "../base/Gf.h"
#include "../base/Vt.h"
#include "Sdf.h"
#include <pxr/usd/usd/stage.h>
#include <pxr/usd/usd/prim.h>
#include <pxr/usd/usd/attribute.h>
#include <atomic>

namespace swiftusd {

// Forward declarations
class StageRef;
class PrimRef;
class AttributeRef;

/// UsdTimeCode - value type
struct TimeCode {
    double value;

    TimeCode() SWIFTUSD_NOEXCEPT : value(0) {}
    TimeCode(double v) SWIFTUSD_NOEXCEPT : value(v) {}
    TimeCode(const UsdTimeCode& tc) SWIFTUSD_NOEXCEPT : value(tc.GetValue()) {}

    UsdTimeCode ToUsd() const SWIFTUSD_NOEXCEPT { return UsdTimeCode(value); }

    static TimeCode Default() SWIFTUSD_NOEXCEPT { return TimeCode(UsdTimeCode::Default().GetValue()); }
    static TimeCode EarliestTime() SWIFTUSD_NOEXCEPT { return TimeCode(UsdTimeCode::EarliestTime().GetValue()); }
} SWIFT_SELF_CONTAINED;

/// UsdStage wrapper
class StageRef {
public:
    // Factory methods
    static StageRef* CreateNew(const char* identifier) SWIFTUSD_NOEXCEPT;
    static StageRef* CreateInMemory(const char* identifier = nullptr) SWIFTUSD_NOEXCEPT;
    static StageRef* Open(const char* path) SWIFTUSD_NOEXCEPT;

    // Prim access
    PrimRef* GetPrimAtPath(const Path& path) SWIFTUSD_NOEXCEPT;
    PrimRef* GetPseudoRoot() SWIFTUSD_NOEXCEPT;
    PrimRef* GetDefaultPrim() SWIFTUSD_NOEXCEPT;
    PrimRef* DefinePrim(const Path& path, const Token& typeName = Token()) SWIFTUSD_NOEXCEPT;
    PrimRef* OverridePrim(const Path& path) SWIFTUSD_NOEXCEPT;
    bool RemovePrim(const Path& path) SWIFTUSD_NOEXCEPT;

    // Stage properties
    double GetStartTimeCode() const SWIFTUSD_NOEXCEPT;
    double GetEndTimeCode() const SWIFTUSD_NOEXCEPT;
    double GetTimeCodesPerSecond() const SWIFTUSD_NOEXCEPT;
    double GetFramesPerSecond() const SWIFTUSD_NOEXCEPT;

    void SetStartTimeCode(double time) SWIFTUSD_NOEXCEPT;
    void SetEndTimeCode(double time) SWIFTUSD_NOEXCEPT;

    // Layers
    LayerRef* GetRootLayer() SWIFTUSD_NOEXCEPT;
    LayerRef* GetSessionLayer() SWIFTUSD_NOEXCEPT;

    // I/O
    bool Save() SWIFTUSD_NOEXCEPT;
    bool Export(const char* path, bool addSourceReference = false) SWIFTUSD_NOEXCEPT;

    // Traversal
    void Traverse(void (*callback)(PrimRef*, void*), void* context) SWIFTUSD_NOEXCEPT;

    // Reference counting
    void retain() SWIFTUSD_NOEXCEPT { _refCount.fetch_add(1, std::memory_order_relaxed); }
    void release() SWIFTUSD_NOEXCEPT {
        if (_refCount.fetch_sub(1, std::memory_order_acq_rel) == 1)
            delete this;
    }

    UsdStageRefPtr GetUsdStage() const SWIFTUSD_NOEXCEPT { return _stage; }

private:
    StageRef(const UsdStageRefPtr& stage) SWIFTUSD_NOEXCEPT : _stage(stage), _refCount(1) {}
    UsdStageRefPtr _stage;
    std::atomic<int> _refCount;
} SWIFT_SHARED_REFERENCE(retain, release);

/// UsdPrim wrapper
class PrimRef {
public:
    PrimRef(const UsdPrim& prim, StageRef* stage) SWIFTUSD_NOEXCEPT;

    // Identity
    Path GetPath() const SWIFTUSD_NOEXCEPT;
    Token GetName() const SWIFTUSD_NOEXCEPT;
    Token GetTypeName() const SWIFTUSD_NOEXCEPT;

    // State
    bool IsValid() const SWIFTUSD_NOEXCEPT;
    bool IsActive() const SWIFTUSD_NOEXCEPT;
    bool IsDefined() const SWIFTUSD_NOEXCEPT;

    void SetActive(bool active) SWIFTUSD_NOEXCEPT;

    // Type checking
    bool IsA(const Token& schemaType) const SWIFTUSD_NOEXCEPT;
    bool HasAPI(const Token& apiSchema) const SWIFTUSD_NOEXCEPT;

    // Hierarchy
    PrimRef* GetParent() SWIFTUSD_NOEXCEPT;
    size_t GetChildCount() const SWIFTUSD_NOEXCEPT;
    void GetChildren(PrimRef** outChildren, size_t maxCount) SWIFTUSD_NOEXCEPT;
    PrimRef* GetChild(const Token& name) SWIFTUSD_NOEXCEPT;

    // Attributes
    AttributeRef* GetAttribute(const Token& name) SWIFTUSD_NOEXCEPT;
    AttributeRef* CreateAttribute(const Token& name, const Token& typeName) SWIFTUSD_NOEXCEPT;
    bool HasAttribute(const Token& name) const SWIFTUSD_NOEXCEPT;

    // Stage access
    StageRef* GetStage() SWIFTUSD_NOEXCEPT { return _stage; }

    // Reference counting
    void retain() SWIFTUSD_NOEXCEPT { _refCount.fetch_add(1, std::memory_order_relaxed); }
    void release() SWIFTUSD_NOEXCEPT {
        if (_refCount.fetch_sub(1, std::memory_order_acq_rel) == 1)
            delete this;
    }

    const UsdPrim& GetUsdPrim() const SWIFTUSD_NOEXCEPT { return _prim; }

private:
    UsdPrim _prim;
    StageRef* _stage;  // Non-owning, stage outlives prim
    std::atomic<int> _refCount{1};
} SWIFT_SHARED_REFERENCE(retain, release);

/// UsdAttribute wrapper
class AttributeRef {
public:
    AttributeRef(const UsdAttribute& attr, PrimRef* prim) SWIFTUSD_NOEXCEPT;

    Token GetName() const SWIFTUSD_NOEXCEPT;
    Token GetTypeName() const SWIFTUSD_NOEXCEPT;
    bool IsValid() const SWIFTUSD_NOEXCEPT;
    bool IsDefined() const SWIFTUSD_NOEXCEPT;
    bool IsAuthored() const SWIFTUSD_NOEXCEPT;

    // Value access (typed)
    bool GetBool(TimeCode time, bool* outValue) const SWIFTUSD_NOEXCEPT;
    bool GetInt(TimeCode time, int* outValue) const SWIFTUSD_NOEXCEPT;
    bool GetFloat(TimeCode time, float* outValue) const SWIFTUSD_NOEXCEPT;
    bool GetDouble(TimeCode time, double* outValue) const SWIFTUSD_NOEXCEPT;
    bool GetVec3f(TimeCode time, Vec3f* outValue) const SWIFTUSD_NOEXCEPT;
    bool GetVec3d(TimeCode time, Vec3d* outValue) const SWIFTUSD_NOEXCEPT;
    bool GetMatrix4d(TimeCode time, Matrix4d* outValue) const SWIFTUSD_NOEXCEPT;
    bool GetToken(TimeCode time, Token* outValue) const SWIFTUSD_NOEXCEPT;

    // Array access
    Vec3fArray* GetVec3fArray(TimeCode time) const SWIFTUSD_NOEXCEPT;
    IntArray* GetIntArray(TimeCode time) const SWIFTUSD_NOEXCEPT;
    FloatArray* GetFloatArray(TimeCode time) const SWIFTUSD_NOEXCEPT;

    // Value setting
    bool SetBool(bool value, TimeCode time = TimeCode::Default()) SWIFTUSD_NOEXCEPT;
    bool SetInt(int value, TimeCode time = TimeCode::Default()) SWIFTUSD_NOEXCEPT;
    bool SetFloat(float value, TimeCode time = TimeCode::Default()) SWIFTUSD_NOEXCEPT;
    bool SetDouble(double value, TimeCode time = TimeCode::Default()) SWIFTUSD_NOEXCEPT;
    bool SetVec3f(Vec3f value, TimeCode time = TimeCode::Default()) SWIFTUSD_NOEXCEPT;
    bool SetMatrix4d(Matrix4d value, TimeCode time = TimeCode::Default()) SWIFTUSD_NOEXCEPT;
    bool SetToken(Token value, TimeCode time = TimeCode::Default()) SWIFTUSD_NOEXCEPT;

    bool SetVec3fArray(Vec3fArray* array, TimeCode time = TimeCode::Default()) SWIFTUSD_NOEXCEPT;
    bool SetIntArray(IntArray* array, TimeCode time = TimeCode::Default()) SWIFTUSD_NOEXCEPT;

    // Reference counting
    void retain() SWIFTUSD_NOEXCEPT { _refCount.fetch_add(1, std::memory_order_relaxed); }
    void release() SWIFTUSD_NOEXCEPT {
        if (_refCount.fetch_sub(1, std::memory_order_acq_rel) == 1)
            delete this;
    }

private:
    UsdAttribute _attr;
    PrimRef* _prim;
    std::atomic<int> _refCount{1};
} SWIFT_SHARED_REFERENCE(retain, release);

} // namespace swiftusd
'''

HGI_H_TEMPLATE = '''// Sources/USDCxx/include/imaging/Hgi.h
#pragma once

#include "../Swift.h"
#include <pxr/imaging/hgi/hgi.h>
#include <atomic>
#include <memory>

#if SWIFTUSD_PLATFORM_APPLE
#include <pxr/imaging/hgiMetal/hgi.h>
#endif

#if SWIFTUSD_HGI_VULKAN
#include <pxr/imaging/hgiVulkan/hgi.h>
#endif

namespace swiftusd {

/// Platform-agnostic GPU interface
class HgiRef {
public:
#if SWIFTUSD_PLATFORM_APPLE
    /// Create Metal-backed Hgi (pass MTLDevice*)
    static HgiRef* CreateMetal(void* mtlDevice) SWIFTUSD_NOEXCEPT;
#endif

#if SWIFTUSD_HGI_VULKAN
    /// Create Vulkan-backed Hgi
    static HgiRef* CreateVulkan(void* vkInstance, void* vkPhysicalDevice, void* vkDevice) SWIFTUSD_NOEXCEPT;
#endif

    /// Start a new frame
    void StartFrame() SWIFTUSD_NOEXCEPT;

    /// End current frame
    void EndFrame() SWIFTUSD_NOEXCEPT;

    /// Submit command buffer (platform-specific handle)
    void SubmitCommands(void* commandBuffer) SWIFTUSD_NOEXCEPT;

    Hgi* GetHgi() const SWIFTUSD_NOEXCEPT { return _hgi.get(); }

    void retain() SWIFTUSD_NOEXCEPT { _refCount.fetch_add(1, std::memory_order_relaxed); }
    void release() SWIFTUSD_NOEXCEPT {
        if (_refCount.fetch_sub(1, std::memory_order_acq_rel) == 1)
            delete this;
    }

private:
    HgiRef(std::unique_ptr<Hgi> hgi) SWIFTUSD_NOEXCEPT : _hgi(std::move(hgi)), _refCount(1) {}
    std::unique_ptr<Hgi> _hgi;
    std::atomic<int> _refCount;
} SWIFT_SHARED_REFERENCE(retain, release);

} // namespace swiftusd
'''

HYDRA_RENDERER_H_TEMPLATE = '''// Sources/USDCxx/include/imaging/UsdImaging.h
#pragma once

#include "../Swift.h"
#include "../usd/Usd.h"
#include <atomic>
#include <memory>

namespace swiftusd {

// Forward declarations
class HgiRef;
class RenderIndexRef;

/// Complete Hydra renderer facade
class HydraRendererRef {
public:
    /// Create renderer for Metal (pass MTLDevice*)
    static HydraRendererRef* CreateMetal(void* mtlDevice) SWIFTUSD_NOEXCEPT;

    /// Create renderer for Vulkan
    static HydraRendererRef* CreateVulkan(void* vkInstance, void* vkPhysicalDevice, void* vkDevice) SWIFTUSD_NOEXCEPT;

    /// Set the USD stage to render
    void SetStage(StageRef* stage) SWIFTUSD_NOEXCEPT;

    /// Set current time
    void SetTime(TimeCode time) SWIFTUSD_NOEXCEPT;

    /// Set viewport dimensions
    void SetViewport(int x, int y, int width, int height) SWIFTUSD_NOEXCEPT;

    /// Set camera matrices (row-major 4x4)
    void SetCamera(const Matrix4d& viewMatrix, const Matrix4d& projMatrix) SWIFTUSD_NOEXCEPT;

    /// Prepare frame (sync scene changes)
    void Prepare() SWIFTUSD_NOEXCEPT;

    /// Render frame to command buffer (platform-specific)
    void Render(void* commandBuffer) SWIFTUSD_NOEXCEPT;

    void retain() SWIFTUSD_NOEXCEPT { _refCount.fetch_add(1, std::memory_order_relaxed); }
    void release() SWIFTUSD_NOEXCEPT {
        if (_refCount.fetch_sub(1, std::memory_order_acq_rel) == 1)
            delete this;
    }

private:
    HydraRendererRef() SWIFTUSD_NOEXCEPT : _refCount(1) {}

    // Internal implementation details hidden
    struct Impl;
    std::unique_ptr<Impl> _impl;
    std::atomic<int> _refCount;
} SWIFT_SHARED_REFERENCE(retain, release);

} // namespace swiftusd
'''

MODULE_MODULEMAP_TEMPLATE = '''// Sources/USDCxx/include/module.modulemap

module USDCxx [system] {
    requires cplusplus

    header "Swift.h"

    module Base {
        header "base/Arch.h"
        header "base/Tf.h"
        header "base/Gf.h"
        header "base/Vt.h"
        export *
    }

    module Scene {
        header "usd/Sdf.h"
        header "usd/Usd.h"
        header "usd/Schemas.h"
        export *
    }

    module Imaging {
        header "imaging/Hgi.h"
        header "imaging/Hd.h"
        header "imaging/HdSt.h"
        header "imaging/Hdx.h"
        header "imaging/UsdImaging.h"
        export *
    }

    export *
}
'''

PACKAGE_SWIFT_TEMPLATE = '''// swift-tools-version: 5.9

import PackageDescription
import Foundation

// MARK: - Configuration

/// Set to true to build against real Pixar USD
/// Set to false for standalone mode with minimal dependencies
let usePixarUSD = true

// MARK: - USD Source Directories (when building from source)

let usdSourceDirs: [String] = [
    // Base
    "base/arch",
    "base/tf",
    "base/gf",
    "base/vt",
    "base/work",
    "base/plug",
    "base/trace",

    // USD Core
    "usd/ar",
    "usd/kind",
    "usd/sdf",
    "usd/pcp",
    "usd/usd",

    // USD Schemas
    "usd/usdGeom",
    "usd/usdShade",
    "usd/usdLux",
    "usd/usdSkel",
    "usd/usdVol",
    "usd/usdUtils",

    // Imaging Foundation
    "imaging/garch",
    "imaging/hio",
    "imaging/hgi",
    "imaging/cameraUtil",
    "imaging/geomUtil",
    "imaging/pxOsd",

    // Hydra Core
    "imaging/hf",
    "imaging/hd",
    "imaging/hdSt",
    "imaging/hdx",

    // USD Imaging
    "usdImaging/usdImaging",
    "usdImaging/usdSkelImaging",
    "usdImaging/usdVolImaging",
    "usdImaging/usdAppUtils",
]

// Platform-specific imaging backends
#if os(macOS) || os(iOS) || os(tvOS) || os(visionOS)
let usdPlatformDirs: [String] = ["imaging/hgiMetal"]
#elseif os(Linux) || os(Windows)
let usdPlatformDirs: [String] = ["imaging/hgiVulkan"]
#else
let usdPlatformDirs: [String] = []
#endif

// MARK: - Build Settings

let usdCxxSettings: [CXXSetting] = [
    .headerSearchPath("../.."),
    .headerSearchPath("../../OpenUSD"),
    .headerSearchPath("../../OpenUSD/pxr"),

    // USD configuration
    .define("PXR_USE_NAMESPACES", to: "0"),
    .define("PXR_PYTHON_SUPPORT_ENABLED", to: "0"),
    .define("PXR_PREFER_SAFETY_OVER_SPEED", to: "1"),

    // Platform-specific
    #if os(macOS) || os(iOS) || os(tvOS) || os(visionOS)
    .define("PXR_METAL_SUPPORT_ENABLED", to: "1"),
    .define("PXR_VULKAN_SUPPORT_ENABLED", to: "0"),
    #elseif os(Linux) || os(Windows)
    .define("PXR_METAL_SUPPORT_ENABLED", to: "0"),
    .define("PXR_VULKAN_SUPPORT_ENABLED", to: "1"),
    #endif

    // Suppress warnings
    .unsafeFlags([
        "-Wno-deprecated",
        "-Wno-deprecated-declarations",
        "-Wno-unused-parameter",
    ]),
]

let wrapperCxxSettings: [CXXSetting] = [
    .headerSearchPath("../../OpenUSD"),
    .headerSearchPath("../../OpenUSD/pxr"),
    .define("PXR_USE_NAMESPACES", to: "0"),
]

let swiftSettings: [SwiftSetting] = [
    .interoperabilityMode(.Cxx),
    .enableUpcomingFeature("StrictConcurrency"),
]

// MARK: - Linker Settings

#if os(macOS) || os(iOS) || os(tvOS) || os(visionOS)
let usdLinkerSettings: [LinkerSetting] = [
    .linkedFramework("Foundation"),
    .linkedFramework("Metal"),
    .linkedFramework("MetalKit"),
    .linkedFramework("CoreGraphics"),
    .linkedLibrary("z"),
    .linkedLibrary("c++"),
]
#elseif os(Linux)
let usdLinkerSettings: [LinkerSetting] = [
    .linkedLibrary("pthread"),
    .linkedLibrary("dl"),
    .linkedLibrary("z"),
    .linkedLibrary("stdc++"),
    .linkedLibrary("vulkan"),
]
#elseif os(Windows)
let usdLinkerSettings: [LinkerSetting] = [
    .linkedLibrary("z"),
    .linkedLibrary("vulkan-1"),
]
#else
let usdLinkerSettings: [LinkerSetting] = []
#endif

// MARK: - Package Definition

let package = Package(
    name: "SwiftUSD",
    platforms: [
        .macOS(.v14),
        .iOS(.v17),
        .tvOS(.v17),
        .visionOS(.v1),
    ],
    products: [
        .library(name: "SwiftUSD", targets: ["SwiftUSD"]),
        .library(name: "USDCxx", targets: ["USDCxx"]),
    ],
    targets: usePixarUSD ? [
        // =========================================================================
        // MARK: - PixarUSD (C++ USD built from source)
        // =========================================================================
        .target(
            name: "PixarUSD",
            path: "OpenUSD/pxr",
            sources: usdSourceDirs + usdPlatformDirs,
            publicHeadersPath: ".",
            cxxSettings: usdCxxSettings,
            linkerSettings: usdLinkerSettings
        ),

        // =========================================================================
        // MARK: - USDCxx (C++ wrappers with SWIFT_* annotations)
        // =========================================================================
        .target(
            name: "USDCxx",
            dependencies: ["PixarUSD"],
            path: "Sources/USDCxx",
            sources: ["src"],
            publicHeadersPath: "include",
            cxxSettings: wrapperCxxSettings
        ),

        // =========================================================================
        // MARK: - SwiftUSD (Pure Swift layer)
        // =========================================================================
        .target(
            name: "SwiftUSD",
            dependencies: ["USDCxx"],
            path: "Sources/SwiftUSD",
            swiftSettings: swiftSettings
        ),

        // =========================================================================
        // MARK: - Tests
        // =========================================================================
        .testTarget(
            name: "SwiftUSDTests",
            dependencies: ["SwiftUSD"],
            path: "Tests/SwiftUSDTests",
            swiftSettings: swiftSettings
        ),
    ] : [
        // =========================================================================
        // Standalone mode (no PixarUSD dependency)
        // =========================================================================
        .target(
            name: "USDCxx",
            path: "Sources/USDCxx",
            sources: ["src"],
            publicHeadersPath: "include",
            cxxSettings: [
                .define("SWIFTUSD_STANDALONE", to: "1"),
            ]
        ),

        .target(
            name: "SwiftUSD",
            dependencies: ["USDCxx"],
            path: "Sources/SwiftUSD",
            swiftSettings: swiftSettings
        ),

        .testTarget(
            name: "SwiftUSDTests",
            dependencies: ["SwiftUSD"],
            path: "Tests/SwiftUSDTests",
            swiftSettings: swiftSettings
        ),
    ],
    cxxLanguageStandard: .cxx17
)
'''

# ============================================================================
# Progress File Management
# ============================================================================

PROGRESS_FILE = Path(__file__).parent.parent / "IMPLEMENTATION_PROGRESS_CXX.md"
FAILURE_LOG = Path(__file__).parent.parent / "IMPLEMENTATION_FAILURES_CXX.log"

STATUS_SYMBOLS = {
    "pending": "⬜",
    "in_progress": "🔶",
    "completed": "✅",
    "failed": "⏸️",
    "skipped": "⏭️",
}

PHASE_DESCRIPTIONS = {
    Phase.ARCHITECTURE: "Setup Package.swift with C++ interop",
    Phase.FOUNDATION: "Essential types used everywhere",
    Phase.SCENE_DESCRIPTION: "Layer and path fundamentals",
    Phase.USD_CORE: "The heart of USD",
    Phase.USD_SCHEMAS: "Scene description schemas",
    Phase.IMAGING_FOUNDATION: "Graphics abstraction layer",
    Phase.HYDRA_CORE: "Render framework",
    Phase.USD_IMAGING: "USD to Hydra bridge",
    Phase.SWIFT_INTEGRATION: "Platform-native Swift layer",
}


def load_progress() -> dict:
    """Load progress from markdown file."""
    if not PROGRESS_FILE.exists():
        return {}

    progress = {}
    with open(PROGRESS_FILE) as f:
        for line in f:
            line = line.strip()
            if line.startswith("|") and not line.startswith("|--") and "Status" not in line:
                parts = [p.strip() for p in line.split("|")]
                if len(parts) >= 3:
                    module_name = parts[1]
                    status_symbol = parts[2]
                    if status_symbol == "✅":
                        progress[module_name] = "completed"
                    elif status_symbol == "🔶":
                        progress[module_name] = "in_progress"
                    elif status_symbol == "⏸️":
                        progress[module_name] = "failed"
                    elif status_symbol == "⏭️":
                        progress[module_name] = "skipped"
    return progress


def save_progress(modules: List[Module]):
    """Save progress to markdown file."""
    with open(PROGRESS_FILE, "w") as f:
        f.write("# SwiftUSD C++ Interop Implementation Progress\n\n")
        f.write(f"Last updated: {datetime.now().strftime('%Y-%m-%d')}\n\n")
        f.write("**Architecture**: 2-layer (Swift → C++ with SWIFT_* annotations)\n\n")
        f.write("**Goal**: Complete USD pipeline with Hydra/Storm rendering via Metal/Vulkan\n\n")

        total = len(modules)
        completed = sum(1 for m in modules if m.status == "completed")
        f.write(f"**Progress**: {completed}/{total} modules completed\n\n")

        in_progress = next((m for m in modules if m.status == "in_progress"), None)
        if in_progress:
            f.write(f"**Current Focus**: Phase {in_progress.phase.value} - {in_progress.phase.name} ({in_progress.name})\n")

        f.write("\n---\n")

        current_phase = None
        for m in modules:
            if m.phase != current_phase:
                current_phase = m.phase
                phase_modules = [mod for mod in modules if mod.phase == current_phase]
                phase_completed = sum(1 for mod in phase_modules if mod.status == "completed")

                f.write(f"\n## Phase {m.phase.value}: {m.phase.name} ({phase_completed}/{len(phase_modules)})\n")
                desc = PHASE_DESCRIPTIONS.get(current_phase, "")
                if desc:
                    f.write(f"{desc}\n")

                f.write("\n| Module | Status | Type | Platform | Description |\n")
                f.write("|--------|--------|------|----------|-------------|\n")

            status_symbol = STATUS_SYMBOLS.get(m.status, "⬜")
            platform = {"all": "All", "apple": "Apple", "linux_windows": "Linux/Win"}.get(m.platform, m.platform)
            f.write(f"| {m.name} | {status_symbol} | {m.swift_type.replace('SWIFT_', '')} | {platform} | {m.description} |\n")

        # Type Mapping
        f.write("\n---\n\n## Type Mapping Strategy\n\n")
        f.write("| USD C++ Type | Swift Type | Annotation |\n")
        f.write("|--------------|------------|------------|\n")
        f.write("| TfToken | Token | SWIFT_SELF_CONTAINED |\n")
        f.write("| SdfPath | Path | SWIFT_SELF_CONTAINED |\n")
        f.write("| GfVec3f | Vec3f | SWIFT_SELF_CONTAINED |\n")
        f.write("| GfMatrix4d | Matrix4d | SWIFT_SELF_CONTAINED |\n")
        f.write("| VtArray<T> | TArray | SWIFT_SHARED_REFERENCE |\n")
        f.write("| UsdStageRefPtr | StageRef | SWIFT_SHARED_REFERENCE |\n")
        f.write("| UsdPrim | PrimRef | SWIFT_SHARED_REFERENCE |\n")
        f.write("| SdfLayerHandle | LayerRef | SWIFT_SHARED_REFERENCE |\n")
        f.write("| Hgi | HgiRef | SWIFT_SHARED_REFERENCE |\n")

        # Platform Support
        f.write("\n---\n\n## Platform Support\n\n")
        f.write("| Platform | GPU Backend | Status |\n")
        f.write("|----------|-------------|--------|\n")
        f.write("| macOS | HgiMetal | Primary |\n")
        f.write("| iOS | HgiMetal | Supported |\n")
        f.write("| visionOS | HgiMetal | Supported |\n")
        f.write("| Linux | HgiVulkan | Supported |\n")
        f.write("| Windows | HgiVulkan | Supported |\n")


# ============================================================================
# File Path Generators
# ============================================================================

def get_expected_files(module: Module) -> ModuleFiles:
    """Generate expected file paths for a module implementation."""

    if module.name == "setup_cxx_interop":
        return ModuleFiles(
            header_files=["Sources/USDCxx/include/Swift.h",
                         "Sources/USDCxx/include/module.modulemap"],
            swift_files=[],
            test_files=[]
        )

    # Swift integration modules
    if module.category == "swift":
        swift_name = module.name.replace("Swift", "")
        return ModuleFiles(
            header_files=[],
            swift_files=[f"Sources/SwiftUSD/{swift_name}/{swift_name}.swift"],
            test_files=[f"Tests/SwiftUSDTests/{module.name}Tests.swift"]
        )

    # Map module name to header file name
    header_name = module.name[0].lower() + module.name[1:]

    if module.category == "base":
        return ModuleFiles(
            header_files=[f"Sources/USDCxx/include/base/{module.name}.h"],
            swift_files=[f"Sources/SwiftUSD/{module.name}/{module.name}.swift"],
            test_files=[f"Tests/SwiftUSDTests/{module.name}Tests.swift"]
        )
    elif module.category == "usd":
        return ModuleFiles(
            header_files=[f"Sources/USDCxx/include/usd/{module.name}.h"],
            swift_files=[f"Sources/SwiftUSD/{module.name}/{module.name}.swift"],
            test_files=[f"Tests/SwiftUSDTests/{module.name}Tests.swift"]
        )
    elif module.category == "imaging" or module.category == "usdImaging":
        return ModuleFiles(
            header_files=[f"Sources/USDCxx/include/imaging/{module.name}.h"],
            swift_files=[f"Sources/SwiftUSD/{module.name}/{module.name}.swift"],
            test_files=[f"Tests/SwiftUSDTests/{module.name}Tests.swift"]
        )
    else:
        # Schema modules (UsdGeom, UsdShade, etc.)
        return ModuleFiles(
            header_files=[f"Sources/USDCxx/include/usd/Schemas.h"],  # Combined schemas header
            swift_files=[f"Sources/SwiftUSD/{module.name}/{module.name}.swift"],
            test_files=[f"Tests/SwiftUSDTests/{module.name}Tests.swift"]
        )


# ============================================================================
# Prompt Generation
# ============================================================================

def generate_setup_prompt() -> str:
    """Generate the prompt for Phase 0: Setup C++ interop architecture."""
    return f'''
TASK: Setup SwiftUSD C++ Interop Architecture

You are the SwiftUSD Implementation Agent. This is Phase 0: creating the foundational
C++ interop architecture that replaces the old 3-layer C wrapper approach.

NEW ARCHITECTURE (2-layer):
- Swift → C++ directly via Swift/C++ interop
- Uses SWIFT_SHARED_REFERENCE and SWIFT_SELF_CONTAINED annotations
- Namespace: swiftusd::
- Reference counting: retain()/release() methods

CLEANUP FIRST:
1. Delete the entire Sources/OpenUSDInterop/ directory if it exists
2. This removes the old 3-layer architecture

CREATE NEW STRUCTURE:

1. Create Sources/USDCxx/include/Swift.h:
```cpp
{SWIFT_H_TEMPLATE}
```

2. Create Sources/USDCxx/include/module.modulemap:
```
{MODULE_MODULEMAP_TEMPLATE}
```

3. Create Sources/USDCxx/src/Handles.cpp (minimal implementation glue):
```cpp
// Sources/USDCxx/src/Handles.cpp
// Minimal implementation file to satisfy Swift Package Manager
// Actual implementations are header-only or in module-specific files

#include "../include/Swift.h"

namespace swiftusd {{
// Empty for now - implementations added as modules are completed
}}
```

4. Create directory structure:
   - Sources/USDCxx/include/base/    (for Arch.h, Tf.h, Gf.h, Vt.h)
   - Sources/USDCxx/include/usd/     (for Sdf.h, Usd.h, Schemas.h)
   - Sources/USDCxx/include/imaging/ (for Hgi.h, Hd.h, HdSt.h, Hdx.h, UsdImaging.h)

5. Update Package.swift to use C++ interop:
```swift
{PACKAGE_SWIFT_TEMPLATE}
```

6. Keep Sources/SwiftUSD/ directory but clear old files that depend on OpenUSDInterop

VERIFICATION:
1. Sources/OpenUSDInterop/ should NOT exist
2. Sources/USDCxx/include/Swift.h exists
3. Sources/USDCxx/include/module.modulemap exists
4. Package.swift has .interoperabilityMode(.Cxx)
5. Run: swift build (should compile, even if empty)

When complete, output: IMPLEMENTATION_COMPLETE
If blocked, output: IMPLEMENTATION_BLOCKED: <reason>
'''


def generate_module_prompt(module: Module) -> str:
    """Generate the Claude Code prompt for implementing a specific module."""

    if module.name == "setup_cxx_interop":
        return generate_setup_prompt()

    expected = get_expected_files(module)

    # Get the appropriate header template
    template_map = {
        "Tf": TF_H_TEMPLATE,
        "Gf": GF_H_TEMPLATE,
        "Vt": VT_H_TEMPLATE,
        "Sdf": SDF_H_TEMPLATE,
        "Usd": USD_H_TEMPLATE,
        "Hgi": HGI_H_TEMPLATE,
    }

    template = template_map.get(module.name, "")

    # Determine pxr path
    if module.category == "base":
        pxr_path = f"pxr/base/{module.name.lower()}"
    elif module.category == "usd":
        pxr_path = f"pxr/usd/{module.name.lower()}"
    elif module.category == "imaging":
        pxr_path = f"pxr/imaging/{module.name.lower()}"
    elif module.category == "usdImaging":
        pxr_path = f"pxr/usdImaging/{module.name.lower()}"
    else:
        # Schema modules
        pxr_path = f"pxr/{module.name.lower()}"

    # Platform-specific notes
    platform_notes = ""
    if module.platform == "apple":
        platform_notes = """
PLATFORM: Apple only (macOS, iOS, visionOS)
- Use #if SWIFTUSD_PLATFORM_APPLE guards
- Requires Metal framework
- Skip on Linux/Windows builds
"""
    elif module.platform == "linux_windows":
        platform_notes = """
PLATFORM: Linux and Windows only
- Use #if SWIFTUSD_HGI_VULKAN guards
- Requires Vulkan SDK
- Skip on Apple builds
"""

    template_section = f"""
REFERENCE HEADER TEMPLATE:
```cpp
{template}
```
""" if template else ""

    return f'''
TASK: Implement SwiftUSD C++ Interop for {module.name}

MODULE: {module.name}
DESCRIPTION: {module.description}
DEPENDENCIES: {', '.join(module.dependencies) if module.dependencies else 'None'}
TYPE: {module.swift_type}
{platform_notes}

ARCHITECTURE (2-layer C++ interop):

Layer 1 - C++ Header with SWIFT_* annotations:
  Location: {expected.header_files[0] if expected.header_files else 'N/A'}
  Contains: Classes with SWIFT_SELF_CONTAINED or SWIFT_SHARED_REFERENCE

Layer 2 - Swift wrapper (optional convenience layer):
  Location: {expected.swift_files[0] if expected.swift_files else 'N/A'}
  Contains: Swift extensions, typealiases, convenience methods

USD REFERENCE: OpenUSD/{pxr_path}/
{template_section}

KEY PATTERNS:

1. SWIFT_SELF_CONTAINED (value types):
   - For small, copyable types (Token, Path, Vec3f, Matrix4d)
   - Swift copies the entire struct
   - No reference counting needed
   ```cpp
   struct Vec3f {{
       float x, y, z;
       // ... methods
   }} SWIFT_SELF_CONTAINED;
   ```

2. SWIFT_SHARED_REFERENCE (reference types):
   - For large/heap-allocated types (Stage, Layer, Arrays)
   - Must implement retain()/release()
   - Use std::atomic<int> for thread-safe ref counting
   ```cpp
   class StageRef {{
       void retain() SWIFTUSD_NOEXCEPT {{ _refCount.fetch_add(1, std::memory_order_relaxed); }}
       void release() SWIFTUSD_NOEXCEPT {{
           if (_refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) delete this;
       }}
   private:
       std::atomic<int> _refCount{{1}};
   }} SWIFT_SHARED_REFERENCE(retain, release);
   ```

3. Factory methods return pointers:
   ```cpp
   static StageRef* Create() SWIFTUSD_NOEXCEPT;  // Returns new StageRef*
   ```

4. All methods marked SWIFTUSD_NOEXCEPT (maps to noexcept)

5. Namespace: swiftusd::

IMPLEMENTATION STEPS:

1. Create C++ header: {expected.header_files[0] if expected.header_files else 'N/A'}
   - Include "../Swift.h" and relevant pxr headers
   - Define wrapper classes in namespace swiftusd
   - Use appropriate SWIFT_* annotation

2. Create Swift file: {expected.swift_files[0] if expected.swift_files else 'N/A'}
   - Import USDCxx module
   - Add convenience extensions if needed
   - Create typealiases for cleaner API

3. Create tests: {expected.test_files[0] if expected.test_files else 'N/A'}
   - Test basic construction
   - Test value access
   - Test reference counting (for SWIFT_SHARED_REFERENCE)

4. Update module.modulemap if needed

VERIFICATION:
1. swift build
2. swift test --filter {module.name}Tests

When complete, output: IMPLEMENTATION_COMPLETE
If blocked, output: IMPLEMENTATION_BLOCKED: <reason>
'''


def generate_fix_prompt(module: Module, verification: VerificationResult) -> str:
    """Generate a prompt to fix specific issues."""
    issues = []

    if not verification.files_ok:
        issues.append(f"Missing files:\n  - " + "\n  - ".join(verification.missing_files))

    if not verification.build_ok:
        errors = extract_build_errors(verification.build_output)
        issues.append(f"Build errors:\n{errors}")

    if not verification.tests_ok:
        failures = extract_test_failures(verification.test_output)
        issues.append(f"Test failures:\n{failures}")

    return f'''
FIX REQUIRED for {module.name}

The previous implementation attempt has the following issues:

{chr(10).join(issues)}

Please fix these specific issues. Focus only on the problems listed above.

When fixed, the implementation must:
1. Create all required files
2. Pass `swift build` without errors
3. Pass `swift test --filter {module.name}Tests`

Read the existing implementation files first, then make targeted fixes.
Do not start from scratch unless absolutely necessary.
'''


# ============================================================================
# Verification
# ============================================================================

def verify_module_files(module: Module, base_path: Path) -> tuple:
    """Verify that expected files exist."""
    expected = get_expected_files(module)
    missing = []

    for f in expected.header_files + expected.swift_files + expected.test_files:
        full_path = base_path / f
        if not full_path.exists():
            missing.append(f)
        elif full_path.stat().st_size < 50:
            missing.append(f"{f} (file too small)")

    return (len(missing) == 0, missing)


def verify_build(base_path: Path, timeout: int = 300) -> tuple:
    """Run swift build and verify compilation."""
    try:
        result = subprocess.run(
            ["swift", "build"],
            capture_output=True,
            text=True,
            timeout=timeout,
            cwd=str(base_path)
        )
        return (result.returncode == 0, result.stdout + result.stderr)
    except subprocess.TimeoutExpired:
        return (False, f"Build timed out after {timeout}s")
    except Exception as e:
        return (False, f"Build error: {str(e)}")


def verify_tests(module: Module, base_path: Path, timeout: int = 120) -> tuple:
    """Run tests for a specific module."""
    test_filter = f"{module.name}Tests"

    try:
        result = subprocess.run(
            ["swift", "test", "--filter", test_filter],
            capture_output=True,
            text=True,
            timeout=timeout,
            cwd=str(base_path)
        )
        return (result.returncode == 0, result.stdout + result.stderr)
    except subprocess.TimeoutExpired:
        return (False, f"Tests timed out after {timeout}s")
    except Exception as e:
        return (False, f"Test error: {str(e)}")


def verify_module_implementation(
    module: Module,
    base_path: Path,
    build_timeout: int = 300,
    test_timeout: int = 120,
    skip_build_test: bool = False
) -> VerificationResult:
    """Comprehensive verification of module implementation."""

    # Skip platform-specific modules on wrong platform
    if module.platform == "apple" and not is_apple_platform():
        return VerificationResult(
            files_ok=True, build_ok=True, tests_ok=True,
            missing_files=[], build_output="Skipped (not Apple platform)",
            test_output="Skipped (not Apple platform)"
        )
    if module.platform == "linux_windows" and is_apple_platform():
        return VerificationResult(
            files_ok=True, build_ok=True, tests_ok=True,
            missing_files=[], build_output="Skipped (Apple platform)",
            test_output="Skipped (Apple platform)"
        )

    # Phase 0 just needs to build
    if module.name == "setup_cxx_interop":
        if skip_build_test:
            return VerificationResult(
                files_ok=True, build_ok=True, tests_ok=True,
                missing_files=[], build_output="Skipped", test_output="Skipped"
            )
        build_ok, build_output = verify_build(base_path, timeout=build_timeout)
        return VerificationResult(
            files_ok=True, build_ok=build_ok, tests_ok=True,
            missing_files=[], build_output=build_output, test_output="N/A (Phase 0)"
        )

    # Check files
    files_ok, missing_files = verify_module_files(module, base_path)
    if not files_ok:
        return VerificationResult(
            files_ok=False, build_ok=False, tests_ok=False,
            missing_files=missing_files,
            build_output="Skipped (files missing)",
            test_output="Skipped (files missing)"
        )

    if skip_build_test:
        return VerificationResult(
            files_ok=True, build_ok=True, tests_ok=True,
            missing_files=[], build_output="Skipped", test_output="Skipped"
        )

    # Build
    build_ok, build_output = verify_build(base_path, timeout=build_timeout)
    if not build_ok:
        return VerificationResult(
            files_ok=True, build_ok=False, tests_ok=False,
            missing_files=[], build_output=build_output,
            test_output="Skipped (build failed)"
        )

    # Tests
    tests_ok, test_output = verify_tests(module, base_path, timeout=test_timeout)
    return VerificationResult(
        files_ok=True, build_ok=True, tests_ok=tests_ok,
        missing_files=[], build_output=build_output, test_output=test_output
    )


def extract_build_errors(output: str, max_lines: int = 20) -> str:
    """Extract relevant build error lines."""
    error_lines = []
    for line in output.split('\n'):
        if 'error:' in line.lower() or 'undefined' in line.lower():
            error_lines.append(line)
            if len(error_lines) >= max_lines:
                break
    return '\n'.join(error_lines) if error_lines else output[-1500:]


def extract_test_failures(output: str, max_lines: int = 20) -> str:
    """Extract relevant test failure lines."""
    failure_lines = []
    in_failure = False
    for line in output.split('\n'):
        if 'FAIL' in line or 'failed' in line.lower() or 'error:' in line.lower():
            in_failure = True
        if in_failure:
            failure_lines.append(line)
            if len(failure_lines) >= max_lines:
                break
    return '\n'.join(failure_lines) if failure_lines else output[-1500:]


def log_failure(module: Module, verification: VerificationResult) -> None:
    """Log detailed failure information."""
    timestamp = datetime.now().strftime("%Y-%m-%d %H:%M:%S")

    with open(FAILURE_LOG, "a") as f:
        f.write(f"\n{'='*60}\n")
        f.write(f"Module: {module.name}\n")
        f.write(f"Timestamp: {timestamp}\n")
        f.write(f"{'='*60}\n\n")

        if not verification.files_ok:
            f.write("MISSING FILES:\n")
            for file in verification.missing_files:
                f.write(f"  - {file}\n")
            f.write("\n")

        if not verification.build_ok:
            f.write("BUILD OUTPUT:\n")
            f.write(verification.build_output[-3000:])
            f.write("\n\n")

        if not verification.tests_ok:
            f.write("TEST OUTPUT:\n")
            f.write(verification.test_output[-3000:])
            f.write("\n\n")


# ============================================================================
# Cleanup
# ============================================================================

def cleanup_old_architecture(base_path: Path, dry_run: bool = False) -> None:
    """Remove OpenUSDInterop before starting fresh."""
    interop_path = base_path / "Sources" / "OpenUSDInterop"
    if interop_path.exists():
        if dry_run:
            print(f"[DRY-RUN] Would remove {interop_path}")
        else:
            shutil.rmtree(interop_path)
            print(f"[CLEANUP] Removed {interop_path}")
    else:
        print(f"[CLEANUP] {interop_path} does not exist, nothing to remove")


# ============================================================================
# Claude Runner
# ============================================================================

def run_claude(prompt: str, cwd: Path, timeout: Optional[int] = None, verbose: bool = False) -> str:
    """Run Claude Code with the given prompt."""
    cmd = ["claude", "--print", "--dangerously-skip-permissions", "-p", prompt]

    if verbose:
        print(f"Running: claude --print --dangerously-skip-permissions -p '<prompt>'")
        print(f"Working directory: {cwd}")

    try:
        process = subprocess.Popen(
            cmd,
            stdout=subprocess.PIPE,
            stderr=subprocess.STDOUT,
            text=True,
            bufsize=1,
            cwd=str(cwd)
        )

        output_lines = []
        start_time = time.time()

        print("\n" + "-" * 40)
        print("Claude Code Output:")
        print("-" * 40)

        try:
            for line in iter(process.stdout.readline, ''):
                if not line:
                    break
                print(line, end='', flush=True)
                output_lines.append(line)

                if timeout and time.time() - start_time > timeout:
                    process.kill()
                    return f"TIMEOUT: Claude Code exceeded {timeout}s limit"

            process.wait()
        except Exception as e:
            process.kill()
            return f"ERROR during execution: {str(e)}"

        print("-" * 40 + "\n")
        return ''.join(output_lines)

    except FileNotFoundError:
        return "ERROR: 'claude' command not found. Is Claude Code CLI installed?"
    except Exception as e:
        return f"ERROR: {str(e)}"


# ============================================================================
# Module Navigation
# ============================================================================

def get_next_module(modules: List[Module], force_continue: bool = False) -> Optional[Module]:
    """Get the next module to implement based on dependency order."""
    for m in modules:
        if m.status == "pending":
            # Skip platform-specific modules on wrong platform
            if m.platform == "apple" and not is_apple_platform():
                m.status = "skipped"
                continue
            if m.platform == "linux_windows" and is_apple_platform():
                m.status = "skipped"
                continue

            deps_met = True
            deps_failed = False
            for dep in m.dependencies:
                dep_module = next((mod for mod in modules if mod.name == dep), None)
                if dep_module:
                    if dep_module.status == "failed":
                        deps_failed = True
                    elif dep_module.status not in ("completed", "skipped"):
                        deps_met = False
                        break

            if deps_met:
                if deps_failed and not force_continue:
                    continue
                return m
    return None


def recover_in_progress_modules(
    modules: List[Module],
    base_path: Path,
    build_timeout: int,
    test_timeout: int,
    skip_build_test: bool
) -> None:
    """Recover modules stuck in 'in_progress' state."""
    for module in modules:
        if module.status == "in_progress":
            print(f"\n[RECOVERY] Checking stuck module: {module.name}")

            verification = verify_module_implementation(
                module, base_path, build_timeout, test_timeout, skip_build_test
            )

            if verification.success:
                print(f"[RECOVERY] {module.name} is actually complete!")
                module.status = "completed"
                module.completed_at = datetime.now().strftime("%Y-%m-%d %H:%M")
            else:
                print(f"[RECOVERY] {module.name} needs work, resetting to pending")
                module.status = "pending"


def list_modules(modules: List[Module]):
    """Print all modules grouped by phase."""
    print("\nSwiftUSD C++ Interop Implementation Modules")
    print("=" * 60)

    current_phase = None
    for m in modules:
        if m.phase != current_phase:
            current_phase = m.phase
            print(f"\n## Phase {m.phase.value}: {m.phase.name}")
            print("-" * 40)

        status_icon = {
            "pending": " ",
            "in_progress": "...",
            "completed": "X",
            "failed": "!",
            "skipped": "-"
        }.get(m.status, " ")

        platform = {"all": "", "apple": "[Apple]", "linux_windows": "[Linux/Win]"}.get(m.platform, "")
        deps = f" (deps: {', '.join(m.dependencies)})" if m.dependencies else ""
        print(f"  [{status_icon}] {m.name:<20} {m.swift_type:<25} {platform}{deps}")


# ============================================================================
# Main
# ============================================================================

def main():
    parser = argparse.ArgumentParser(
        description="SwiftUSD C++ Interop Implementation Agent",
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
Examples:
  python Scripts/implement_swiftusd_cxx.py              # Run all modules
  python Scripts/implement_swiftusd_cxx.py --start-phase 1   # Start from Phase 1
  python Scripts/implement_swiftusd_cxx.py --module Tf       # Single module only
  python Scripts/implement_swiftusd_cxx.py --dry-run        # Preview prompts
  python Scripts/implement_swiftusd_cxx.py --list           # List all modules
  python Scripts/implement_swiftusd_cxx.py --clean          # Remove old architecture
        """
    )
    parser.add_argument("--start-phase", type=int, default=0,
                        help="Start from phase N (0-8)")
    parser.add_argument("--module", type=str,
                        help="Implement specific module only")
    parser.add_argument("--dry-run", action="store_true",
                        help="Print prompts without running Claude")
    parser.add_argument("--list", action="store_true",
                        help="List all modules and their status")
    parser.add_argument("--clean", action="store_true",
                        help="Remove old OpenUSDInterop architecture")
    parser.add_argument("--verbose", "-v", action="store_true",
                        help="Verbose output")
    parser.add_argument("--timeout", type=int, default=None,
                        help="Timeout per Claude run in seconds (default: no timeout)")
    parser.add_argument("--max-retries", type=int, default=3,
                        help="Maximum retry attempts per module (default: 3)")
    parser.add_argument("--build-timeout", type=int, default=300,
                        help="Build timeout in seconds (default: 300)")
    parser.add_argument("--test-timeout", type=int, default=120,
                        help="Test timeout in seconds (default: 120)")
    parser.add_argument("--strict-deps", action="store_true",
                        help="Stop if a dependency fails")
    parser.add_argument("--skip-build-test", action="store_true",
                        help="Only verify files exist, skip build/test")
    args = parser.parse_args()

    base_path = Path(__file__).parent.parent.resolve()

    # Load progress
    progress = load_progress()
    for m in MODULES:
        m.status = progress.get(m.name, "pending")

    if args.list:
        list_modules(MODULES)
        return

    if args.clean:
        cleanup_old_architecture(base_path, dry_run=args.dry_run)
        if args.dry_run:
            return

    # Recover stuck modules
    print("Checking for stuck modules...")
    recover_in_progress_modules(MODULES, base_path, args.build_timeout, args.test_timeout, args.skip_build_test)
    save_progress(MODULES)

    # Filter modules
    if args.module:
        target = next((m for m in MODULES if m.name == args.module), None)
        if not target:
            print(f"Unknown module: {args.module}")
            print("Use --list to see all available modules")
            sys.exit(1)
        modules_to_process = [target]
    else:
        modules_to_process = [m for m in MODULES if m.phase.value >= args.start_phase]

    pending = [m for m in modules_to_process if m.status == "pending"]

    print("=" * 60)
    print("SwiftUSD C++ Interop Implementation Agent")
    print("=" * 60)
    print(f"Architecture: 2-layer (Swift -> C++ with SWIFT_* annotations)")
    print(f"Total modules: {len(MODULES)}")
    print(f"Modules to process: {len(pending)}")
    print(f"Platform: {'Apple' if is_apple_platform() else 'Linux/Windows'}")
    if args.dry_run:
        print("MODE: Dry run (prompts only)")
    print()

    iteration = 0
    start_time = time.time()

    while True:
        module = get_next_module(modules_to_process, force_continue=not args.strict_deps)
        if not module:
            elapsed = time.time() - start_time
            print(f"\n{'=' * 60}")

            failed = sum(1 for m in modules_to_process if m.status == "failed")
            completed = sum(1 for m in modules_to_process if m.status == "completed")
            skipped = sum(1 for m in modules_to_process if m.status == "skipped")

            if failed > 0:
                print(f"Finished with failures ({elapsed:.1f}s total)")
            else:
                print(f"All modules completed! ({elapsed:.1f}s total)")
            print(f"Completed: {completed}, Skipped: {skipped}, Failed: {failed}")
            break

        iteration += 1
        print(f"\n{'=' * 60}")
        print(f"[{iteration}] Implementing: {module.name}")
        print(f"    Phase: {module.phase.value} ({module.phase.name})")
        print(f"    Type: {module.swift_type}")
        print(f"    Dependencies: {module.dependencies or 'None'}")
        print("=" * 60)

        module.status = "in_progress"
        save_progress(MODULES)

        if args.dry_run:
            prompt = generate_module_prompt(module)
            print("\n--- PROMPT PREVIEW ---")
            print(prompt[:3000])
            if len(prompt) > 3000:
                print(f"... ({len(prompt) - 3000} more characters)")
            print("--- END PREVIEW ---\n")
            module.status = "completed"
            module.completed_at = datetime.now().strftime("%Y-%m-%d %H:%M")
            save_progress(MODULES)
            continue

        retry_count = 0
        verification = None
        module_start = time.time()

        while retry_count <= args.max_retries:
            if retry_count == 0:
                prompt = generate_module_prompt(module)
            else:
                print(f"\n[RETRY {retry_count}/{args.max_retries}] Attempting fix...")
                prompt = generate_fix_prompt(module, verification)

            print(f"\nRunning Claude Code...")
            output = run_claude(prompt, cwd=base_path, timeout=args.timeout, verbose=args.verbose)

            if output.startswith("ERROR:") or output.startswith("TIMEOUT:"):
                print(f"\n[ERROR] Claude execution failed: {output[:200]}")
                retry_count += 1
                continue

            print("\n[VERIFICATION] Checking implementation...")
            verification = verify_module_implementation(
                module, base_path, args.build_timeout, args.test_timeout, args.skip_build_test
            )

            if verification.success:
                elapsed = time.time() - module_start
                print(f"\n[OK] {module.name} completed in {elapsed:.1f}s")
                module.status = "completed"
                module.completed_at = datetime.now().strftime("%Y-%m-%d %H:%M")
                break
            else:
                print("[VERIFICATION] Issues found:")
                if not verification.files_ok:
                    print(f"    - Missing files: {verification.missing_files}")
                if not verification.build_ok:
                    print("    - Build: FAILED")
                if not verification.tests_ok:
                    print("    - Tests: FAILED")

            retry_count += 1

        if not verification or not verification.success:
            elapsed = time.time() - module_start
            print(f"\n[FAIL] {module.name} failed after {args.max_retries} retries ({elapsed:.1f}s)")
            module.status = "failed"
            if verification:
                log_failure(module, verification)
                print(f"    Failure details logged to: {FAILURE_LOG}")

        save_progress(MODULES)

    # Final summary
    save_progress(MODULES)
    completed = sum(1 for m in MODULES if m.status == "completed")
    failed = sum(1 for m in MODULES if m.status == "failed")
    skipped = sum(1 for m in MODULES if m.status == "skipped")

    print(f"\n{'=' * 60}")
    print("SUMMARY")
    print(f"{'=' * 60}")
    print(f"Completed: {completed}/{len(MODULES)}")
    print(f"Skipped: {skipped}")
    if failed > 0:
        print(f"Failed: {failed}")
        print(f"See {FAILURE_LOG} for details")
    print(f"\nProgress saved to: {PROGRESS_FILE}")


if __name__ == "__main__":
    main()
