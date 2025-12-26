#!/usr/bin/env python3
"""
SwiftUSD CxxFacade Implementation Agent

This script runs Claude Code in a loop to systematically implement the complete
CxxFacade + SwiftUSD architecture using the pimpl (Pointer to Implementation)
pattern.

Architecture:
  SwiftUSD (Swift) → CxxFacade (C++ pimpl) → USD binaries (Vendor/USD/)

The pimpl pattern solves Clang modules incompatibility - Swift's ClangImporter
cannot parse Pixar USD headers because they have #include statements inside
namespace blocks. By keeping all pxr includes in .cpp files, Swift never sees them.

Usage:
    python Scripts/implement_swiftusd_cxxfacade.py                    # Run all
    python Scripts/implement_swiftusd_cxxfacade.py --module Tf        # Single module
    python Scripts/implement_swiftusd_cxxfacade.py --start-phase 1    # Start from phase
    python Scripts/implement_swiftusd_cxxfacade.py --list             # List modules
    python Scripts/implement_swiftusd_cxxfacade.py --dry-run          # Preview prompts
    python Scripts/implement_swiftusd_cxxfacade.py --clean            # Remove old code
    python Scripts/implement_swiftusd_cxxfacade.py --skip-build-test  # Skip verification
    python Scripts/implement_swiftusd_cxxfacade.py --retry-failed     # Retry failed modules
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
from typing import Optional, List, Dict
from datetime import datetime


# ============================================================================
# Configuration
# ============================================================================

class Phase(Enum):
    SETUP = 0              # Directory structure, Package.swift
    FOUNDATION = 1         # Tf, Gf, Vt
    SCENE_DESCRIPTION = 2  # Sdf, Kind
    USD_CORE = 3           # Usd (Stage, Prim, Attribute)
    USD_SCHEMAS = 4        # UsdGeom, UsdShade, UsdLux, UsdSkel
    IMAGING = 5            # Hydra


@dataclass
class Module:
    """Definition of a module to implement."""
    name: str
    phase: Phase
    folder: str  # e.g., "base", "usd", "usdGeom", "imaging"
    pxr_path: str  # e.g., "pxr/base/tf"
    description: str
    types: List[str] = field(default_factory=list)  # Types to implement
    dependencies: List[str] = field(default_factory=list)
    platform: str = "all"  # "all", "apple", "linux"
    status: str = "pending"
    completed_at: Optional[str] = None


@dataclass
class ModuleFiles:
    """Expected files for a module implementation."""
    header_file: str
    impl_file: str
    swift_file: str
    test_file: str


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


# ============================================================================
# Module Registry
# ============================================================================

MODULES = [
    # Phase 0: Setup
    Module(
        name="setup",
        phase=Phase.SETUP,
        folder="",
        pxr_path="",
        description="Create CxxFacade directory structure, module.modulemap, and Package.swift",
        types=[],
    ),

    # Phase 1: Foundation
    Module(
        name="Tf",
        phase=Phase.FOUNDATION,
        folder="base",
        pxr_path="pxr/base/tf",
        description="TfToken - interned strings used as identifiers throughout USD",
        types=["Token"],
    ),

    Module(
        name="Gf",
        phase=Phase.FOUNDATION,
        folder="base",
        pxr_path="pxr/base/gf",
        description="Graphics foundation math types",
        types=["Vec2f", "Vec3f", "Vec4f", "Vec2d", "Vec3d", "Vec4d",
               "Matrix4d", "Matrix4f", "Quatd", "Quatf", "Range3d", "BBox3d"],
        dependencies=["Tf"],
    ),

    Module(
        name="Vt",
        phase=Phase.FOUNDATION,
        folder="base",
        pxr_path="pxr/base/vt",
        description="Value type arrays with copy-on-write semantics",
        types=["IntArray", "FloatArray", "DoubleArray", "Vec2fArray", "Vec3fArray",
               "Vec4fArray", "Matrix4dArray", "TokenArray"],
        dependencies=["Gf", "Tf"],
    ),

    # Phase 2: Scene Description
    Module(
        name="Sdf",
        phase=Phase.SCENE_DESCRIPTION,
        folder="usd",
        pxr_path="pxr/usd/sdf",
        description="Scene description foundation - paths and layers",
        types=["Path", "Layer", "AssetPath", "LayerOffset"],
        dependencies=["Tf", "Vt"],
    ),

    Module(
        name="Kind",
        phase=Phase.SCENE_DESCRIPTION,
        folder="usd",
        pxr_path="pxr/usd/kind",
        description="Model hierarchy kinds for scene organization",
        types=["Registry"],
        dependencies=["Tf"],
    ),

    # Phase 3: USD Core
    Module(
        name="Usd",
        phase=Phase.USD_CORE,
        folder="usd",
        pxr_path="pxr/usd/usd",
        description="Core USD API - Stage, Prim, Property, Attribute, Relationship",
        types=["TimeCode", "Stage", "Prim", "Property", "Attribute", "Relationship",
               "SchemaBase", "Typed", "APISchemaBase", "EditContext", "EditTarget"],
        dependencies=["Sdf", "Tf", "Gf", "Vt"],
    ),

    # Phase 4: USD Schemas
    Module(
        name="UsdGeom",
        phase=Phase.USD_SCHEMAS,
        folder="usdGeom",
        pxr_path="pxr/usd/usdGeom",
        description="Geometry schemas - transforms, meshes, curves, cameras",
        types=["Xformable", "Xform", "Scope", "Imageable", "Boundable", "Gprim",
               "Mesh", "Points", "BasisCurves", "NurbsCurves", "Camera",
               "Cube", "Sphere", "Cylinder", "Capsule", "Cone", "Primvar", "PrimvarsAPI"],
        dependencies=["Usd", "Gf", "Vt"],
    ),

    Module(
        name="UsdShade",
        phase=Phase.USD_SCHEMAS,
        folder="usdShade",
        pxr_path="pxr/usd/usdShade",
        description="Shading schemas - materials, shaders, node graphs",
        types=["Shader", "Material", "NodeGraph", "ConnectableAPI",
               "MaterialBindingAPI", "Input", "Output"],
        dependencies=["Usd", "Sdf"],
    ),

    Module(
        name="UsdLux",
        phase=Phase.USD_SCHEMAS,
        folder="usdLux",
        pxr_path="pxr/usd/usdLux",
        description="Lighting schemas",
        types=["LightAPI", "MeshLightAPI", "VolumeLightAPI", "ListAPI",
               "DistantLight", "DomeLight", "RectLight", "SphereLight",
               "CylinderLight", "DiskLight", "LightFilter", "ShadowAPI", "ShapingAPI"],
        dependencies=["UsdGeom", "UsdShade"],
    ),

    Module(
        name="UsdSkel",
        phase=Phase.USD_SCHEMAS,
        folder="usdSkel",
        pxr_path="pxr/usd/usdSkel",
        description="Skeletal animation schemas",
        types=["Skeleton", "Root", "Animation", "BindingAPI", "BlendShape",
               "SkinningQuery", "SkeletonQuery", "AnimQuery", "Cache", "Topology"],
        dependencies=["UsdGeom"],
    ),

    # Phase 5: Imaging
    Module(
        name="Hydra",
        phase=Phase.IMAGING,
        folder="imaging",
        pxr_path="pxr/imaging",
        description="High-level Hydra rendering facade for USD scenes",
        types=["RenderSettings", "CameraSettings", "Engine"],
        dependencies=["Usd", "UsdGeom"],
    ),
]


# ============================================================================
# Embedded Templates for Pimpl Pattern
# ============================================================================

# Main CxxFacade header with Swift bridging macros
CXXFACADE_H_TEMPLATE = '''// CxxFacade.h - Swift/C++ interop prelude
#pragma once

#if __has_include(<swift/bridging>)
#include <swift/bridging>
#else
#define SWIFT_SHARED_REFERENCE(retain, release)
#define SWIFT_IMMORTAL_REFERENCE
#define SWIFT_SELF_CONTAINED
#define SWIFT_UNSAFE_REFERENCE
#define SWIFT_RETURNS_RETAINED
#endif

#include <atomic>
#include <string>
#include <cstdint>
#include <vector>
#include <memory>

// Platform detection
#if defined(__APPLE__)
    #define CXXFACADE_PLATFORM_APPLE 1
#elif defined(__linux__)
    #define CXXFACADE_PLATFORM_LINUX 1
#elif defined(_WIN32)
    #define CXXFACADE_PLATFORM_WINDOWS 1
#endif

namespace cxxfacade {
    // All types will be in this namespace
}
'''

# TfToken header template (clean, no pxr)
TF_HPP_TEMPLATE = '''// Tf.hpp - TfToken wrapper (pimpl pattern)
// CLEAN HEADER - NO pxr includes!
#pragma once

#ifdef __swift__
#include <swift/bridging>
#else
#define SWIFT_SELF_CONTAINED
#endif

#include <string>
#include <cstddef>

namespace cxxfacade {

/// TfToken - Interned string for efficient comparison and hashing
/// Used throughout USD as identifiers for prims, attributes, etc.
class TfToken {
    void* impl_;  // Hides pxr::TfToken
public:
    TfToken();
    TfToken(const char* text);
    TfToken(const std::string& text);
    TfToken(const TfToken& other);
    TfToken(TfToken&& other) noexcept;
    TfToken& operator=(const TfToken& other);
    TfToken& operator=(TfToken&& other) noexcept;
    ~TfToken();

    const char* GetText() const;
    bool IsEmpty() const;
    size_t Hash() const;
    size_t size() const;

    bool operator==(const TfToken& other) const;
    bool operator!=(const TfToken& other) const;
    bool operator<(const TfToken& other) const;

    // Access underlying pxr::TfToken for interop
    void* GetRawPtr() const { return impl_; }
} SWIFT_SELF_CONTAINED;

} // namespace cxxfacade
'''

# TfToken implementation template
TF_CPP_TEMPLATE = '''// Tf.cpp - TfToken implementation with real USD
#include "../../include/base/Tf.hpp"
#include <pxr/base/tf/token.h>

PXR_NAMESPACE_USING_DIRECTIVE

namespace cxxfacade {

TfToken::TfToken() : impl_(new pxr::TfToken()) {}

TfToken::TfToken(const char* text) : impl_(new pxr::TfToken(text)) {}

TfToken::TfToken(const std::string& text) : impl_(new pxr::TfToken(text)) {}

TfToken::TfToken(const TfToken& other)
    : impl_(new pxr::TfToken(*static_cast<pxr::TfToken*>(other.impl_))) {}

TfToken::TfToken(TfToken&& other) noexcept : impl_(other.impl_) {
    other.impl_ = new pxr::TfToken();
}

TfToken& TfToken::operator=(const TfToken& other) {
    if (this != &other) {
        delete static_cast<pxr::TfToken*>(impl_);
        impl_ = new pxr::TfToken(*static_cast<pxr::TfToken*>(other.impl_));
    }
    return *this;
}

TfToken& TfToken::operator=(TfToken&& other) noexcept {
    if (this != &other) {
        delete static_cast<pxr::TfToken*>(impl_);
        impl_ = other.impl_;
        other.impl_ = new pxr::TfToken();
    }
    return *this;
}

TfToken::~TfToken() {
    delete static_cast<pxr::TfToken*>(impl_);
}

const char* TfToken::GetText() const {
    return static_cast<pxr::TfToken*>(impl_)->GetText();
}

bool TfToken::IsEmpty() const {
    return static_cast<pxr::TfToken*>(impl_)->IsEmpty();
}

size_t TfToken::Hash() const {
    return static_cast<pxr::TfToken*>(impl_)->Hash();
}

size_t TfToken::size() const {
    return static_cast<pxr::TfToken*>(impl_)->size();
}

bool TfToken::operator==(const TfToken& other) const {
    return *static_cast<pxr::TfToken*>(impl_) == *static_cast<pxr::TfToken*>(other.impl_);
}

bool TfToken::operator!=(const TfToken& other) const {
    return !(*this == other);
}

bool TfToken::operator<(const TfToken& other) const {
    return *static_cast<pxr::TfToken*>(impl_) < *static_cast<pxr::TfToken*>(other.impl_);
}

} // namespace cxxfacade
'''

# Gf (Graphics Foundation) header template
GF_HPP_TEMPLATE = '''// Gf.hpp - Graphics foundation math types (pimpl pattern)
// CLEAN HEADER - NO pxr includes!
#pragma once

#ifdef __swift__
#include <swift/bridging>
#else
#define SWIFT_SELF_CONTAINED
#endif

#include <cmath>
#include <cstddef>

namespace cxxfacade {

// ============================================================================
// Vector Types (Value Types - SWIFT_SELF_CONTAINED)
// ============================================================================

struct Vec2f {
    float x, y;

    Vec2f() : x(0), y(0) {}
    Vec2f(float x_, float y_) : x(x_), y(y_) {}

    float Length() const { return std::sqrt(x*x + y*y); }
    float LengthSquared() const { return x*x + y*y; }
    Vec2f Normalized() const;
    float Dot(const Vec2f& other) const { return x*other.x + y*other.y; }

    Vec2f operator+(const Vec2f& o) const { return Vec2f(x+o.x, y+o.y); }
    Vec2f operator-(const Vec2f& o) const { return Vec2f(x-o.x, y-o.y); }
    Vec2f operator*(float s) const { return Vec2f(x*s, y*s); }
    Vec2f operator/(float s) const { return Vec2f(x/s, y/s); }
    bool operator==(const Vec2f& o) const { return x==o.x && y==o.y; }

    static Vec2f Zero() { return Vec2f(); }
} SWIFT_SELF_CONTAINED;

struct Vec3f {
    float x, y, z;

    Vec3f() : x(0), y(0), z(0) {}
    Vec3f(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}

    float Length() const { return std::sqrt(x*x + y*y + z*z); }
    float LengthSquared() const { return x*x + y*y + z*z; }
    Vec3f Normalized() const;
    float Dot(const Vec3f& other) const { return x*other.x + y*other.y + z*other.z; }
    Vec3f Cross(const Vec3f& other) const;

    Vec3f operator+(const Vec3f& o) const { return Vec3f(x+o.x, y+o.y, z+o.z); }
    Vec3f operator-(const Vec3f& o) const { return Vec3f(x-o.x, y-o.y, z-o.z); }
    Vec3f operator*(float s) const { return Vec3f(x*s, y*s, z*s); }
    Vec3f operator/(float s) const { return Vec3f(x/s, y/s, z/s); }
    Vec3f operator-() const { return Vec3f(-x, -y, -z); }
    bool operator==(const Vec3f& o) const { return x==o.x && y==o.y && z==o.z; }

    static Vec3f Zero() { return Vec3f(); }
    static Vec3f XAxis() { return Vec3f(1, 0, 0); }
    static Vec3f YAxis() { return Vec3f(0, 1, 0); }
    static Vec3f ZAxis() { return Vec3f(0, 0, 1); }
} SWIFT_SELF_CONTAINED;

struct Vec4f {
    float x, y, z, w;

    Vec4f() : x(0), y(0), z(0), w(0) {}
    Vec4f(float x_, float y_, float z_, float w_) : x(x_), y(y_), z(z_), w(w_) {}

    float Length() const { return std::sqrt(x*x + y*y + z*z + w*w); }
    Vec4f Normalized() const;
    float Dot(const Vec4f& other) const { return x*other.x + y*other.y + z*other.z + w*other.w; }

    bool operator==(const Vec4f& o) const { return x==o.x && y==o.y && z==o.z && w==o.w; }
    static Vec4f Zero() { return Vec4f(); }
} SWIFT_SELF_CONTAINED;

// Double precision vectors
struct Vec2d {
    double x, y;
    Vec2d() : x(0), y(0) {}
    Vec2d(double x_, double y_) : x(x_), y(y_) {}
    double Length() const { return std::sqrt(x*x + y*y); }
    Vec2d Normalized() const;
    static Vec2d Zero() { return Vec2d(); }
} SWIFT_SELF_CONTAINED;

struct Vec3d {
    double x, y, z;
    Vec3d() : x(0), y(0), z(0) {}
    Vec3d(double x_, double y_, double z_) : x(x_), y(y_), z(z_) {}
    double Length() const { return std::sqrt(x*x + y*y + z*z); }
    Vec3d Normalized() const;
    Vec3d Cross(const Vec3d& other) const;
    double Dot(const Vec3d& other) const { return x*other.x + y*other.y + z*other.z; }
    static Vec3d Zero() { return Vec3d(); }
    static Vec3d XAxis() { return Vec3d(1, 0, 0); }
    static Vec3d YAxis() { return Vec3d(0, 1, 0); }
    static Vec3d ZAxis() { return Vec3d(0, 0, 1); }
} SWIFT_SELF_CONTAINED;

struct Vec4d {
    double x, y, z, w;
    Vec4d() : x(0), y(0), z(0), w(0) {}
    Vec4d(double x_, double y_, double z_, double w_) : x(x_), y(y_), z(z_), w(w_) {}
    double Length() const { return std::sqrt(x*x + y*y + z*z + w*w); }
    static Vec4d Zero() { return Vec4d(); }
} SWIFT_SELF_CONTAINED;

// ============================================================================
// Quaternion Types
// ============================================================================

struct Quatf {
    float w, x, y, z;  // Real part first, then imaginary

    Quatf() : w(1), x(0), y(0), z(0) {}  // Identity
    Quatf(float w_, float x_, float y_, float z_) : w(w_), x(x_), y(y_), z(z_) {}

    Quatf Normalized() const;
    Quatf GetInverse() const;
    float Length() const { return std::sqrt(w*w + x*x + y*y + z*z); }

    static Quatf Identity() { return Quatf(); }
    static Quatf FromAxisAngle(const Vec3f& axis, float radians);
} SWIFT_SELF_CONTAINED;

struct Quatd {
    double w, x, y, z;

    Quatd() : w(1), x(0), y(0), z(0) {}
    Quatd(double w_, double x_, double y_, double z_) : w(w_), x(x_), y(y_), z(z_) {}

    Quatd Normalized() const;
    Quatd GetInverse() const;
    double Length() const { return std::sqrt(w*w + x*x + y*y + z*z); }

    static Quatd Identity() { return Quatd(); }
    static Quatd FromAxisAngle(const Vec3d& axis, double radians);
} SWIFT_SELF_CONTAINED;

// ============================================================================
// Matrix Types (Column-major, like OpenGL)
// ============================================================================

struct Matrix4d {
    double data[16];  // Column-major

    Matrix4d();  // Identity
    Matrix4d(double diagonal);

    double& operator()(int row, int col) { return data[col * 4 + row]; }
    double operator()(int row, int col) const { return data[col * 4 + row]; }

    Matrix4d GetTranspose() const;
    Matrix4d GetInverse() const;
    double GetDeterminant() const;

    Vec3d TransformPoint(const Vec3d& p) const;
    Vec3d TransformDirection(const Vec3d& d) const;

    Matrix4d operator*(const Matrix4d& other) const;

    static Matrix4d Identity();
    static Matrix4d Translate(const Vec3d& t);
    static Matrix4d Scale(const Vec3d& s);
    static Matrix4d RotateX(double radians);
    static Matrix4d RotateY(double radians);
    static Matrix4d RotateZ(double radians);
} SWIFT_SELF_CONTAINED;

struct Matrix4f {
    float data[16];

    Matrix4f();
    Matrix4f(float diagonal);

    float& operator()(int row, int col) { return data[col * 4 + row]; }
    float operator()(int row, int col) const { return data[col * 4 + row]; }

    static Matrix4f Identity();
} SWIFT_SELF_CONTAINED;

// ============================================================================
// Range/BBox Types
// ============================================================================

struct Range3d {
    Vec3d min, max;

    Range3d() : min(Vec3d()), max(Vec3d()) {}
    Range3d(const Vec3d& min_, const Vec3d& max_) : min(min_), max(max_) {}

    bool IsEmpty() const;
    Vec3d GetSize() const;
    Vec3d GetCenter() const;
    bool Contains(const Vec3d& point) const;
    void UnionWith(const Range3d& other);
    void UnionWith(const Vec3d& point);
} SWIFT_SELF_CONTAINED;

struct BBox3d {
    Range3d range;
    Matrix4d matrix;

    BBox3d();
    BBox3d(const Range3d& range_);
    BBox3d(const Range3d& range_, const Matrix4d& matrix_);

    Range3d ComputeAlignedRange() const;
    Vec3d ComputeCentroid() const;
} SWIFT_SELF_CONTAINED;

} // namespace cxxfacade
'''

# Vt (Value Types) header template
VT_HPP_TEMPLATE = '''// Vt.hpp - Value type arrays with copy-on-write (pimpl pattern)
// CLEAN HEADER - NO pxr includes!
#pragma once

#ifdef __swift__
#include <swift/bridging>
#else
#define SWIFT_SHARED_REFERENCE(retain, release)
#define SWIFT_RETURNS_RETAINED
#endif

#include <cstddef>
#include <atomic>
#include "Tf.hpp"
#include "Gf.hpp"

namespace cxxfacade {

namespace detail {
    struct VtArrayImplBase;
}

// ============================================================================
// VtArray Template (Reference Type with COW semantics)
// ============================================================================

template<typename T>
class VtArray;

// Forward declare retain/release for each array type
class VtIntArray;
class VtFloatArray;
class VtDoubleArray;
class VtVec2fArray;
class VtVec3fArray;
class VtVec4fArray;
class VtMatrix4dArray;
class VtTokenArray;

void retainVtIntArray(VtIntArray* arr);
void releaseVtIntArray(VtIntArray* arr);
void retainVtFloatArray(VtFloatArray* arr);
void releaseVtFloatArray(VtFloatArray* arr);
void retainVtDoubleArray(VtDoubleArray* arr);
void releaseVtDoubleArray(VtDoubleArray* arr);
void retainVtVec3fArray(VtVec3fArray* arr);
void releaseVtVec3fArray(VtVec3fArray* arr);
void retainVtTokenArray(VtTokenArray* arr);
void releaseVtTokenArray(VtTokenArray* arr);

// ============================================================================
// Concrete Array Types
// ============================================================================

class VtIntArray {
    void* impl_;
    mutable std::atomic<int> refCount_{1};
public:
    VtIntArray();
    explicit VtIntArray(size_t size);
    ~VtIntArray();

    size_t size() const;
    bool empty() const { return size() == 0; }
    void reserve(size_t n);
    void resize(size_t n);
    void push_back(int value);

    int operator[](size_t i) const;
    int* data();
    const int* data() const;

    void* GetRawPtr() const { return impl_; }

    friend void retainVtIntArray(VtIntArray* arr);
    friend void releaseVtIntArray(VtIntArray* arr);
} SWIFT_SHARED_REFERENCE(retainVtIntArray, releaseVtIntArray);

class VtFloatArray {
    void* impl_;
    mutable std::atomic<int> refCount_{1};
public:
    VtFloatArray();
    explicit VtFloatArray(size_t size);
    ~VtFloatArray();

    size_t size() const;
    bool empty() const { return size() == 0; }
    void reserve(size_t n);
    void resize(size_t n);
    void push_back(float value);

    float operator[](size_t i) const;
    float* data();
    const float* data() const;

    void* GetRawPtr() const { return impl_; }

    friend void retainVtFloatArray(VtFloatArray* arr);
    friend void releaseVtFloatArray(VtFloatArray* arr);
} SWIFT_SHARED_REFERENCE(retainVtFloatArray, releaseVtFloatArray);

class VtDoubleArray {
    void* impl_;
    mutable std::atomic<int> refCount_{1};
public:
    VtDoubleArray();
    explicit VtDoubleArray(size_t size);
    ~VtDoubleArray();

    size_t size() const;
    void push_back(double value);
    double operator[](size_t i) const;

    friend void retainVtDoubleArray(VtDoubleArray* arr);
    friend void releaseVtDoubleArray(VtDoubleArray* arr);
} SWIFT_SHARED_REFERENCE(retainVtDoubleArray, releaseVtDoubleArray);

class VtVec3fArray {
    void* impl_;
    mutable std::atomic<int> refCount_{1};
public:
    VtVec3fArray();
    explicit VtVec3fArray(size_t size);
    ~VtVec3fArray();

    size_t size() const;
    void push_back(const Vec3f& value);
    Vec3f operator[](size_t i) const;

    friend void retainVtVec3fArray(VtVec3fArray* arr);
    friend void releaseVtVec3fArray(VtVec3fArray* arr);
} SWIFT_SHARED_REFERENCE(retainVtVec3fArray, releaseVtVec3fArray);

class VtTokenArray {
    void* impl_;
    mutable std::atomic<int> refCount_{1};
public:
    VtTokenArray();
    explicit VtTokenArray(size_t size);
    ~VtTokenArray();

    size_t size() const;
    void push_back(const TfToken& value);
    TfToken operator[](size_t i) const;

    friend void retainVtTokenArray(VtTokenArray* arr);
    friend void releaseVtTokenArray(VtTokenArray* arr);
} SWIFT_SHARED_REFERENCE(retainVtTokenArray, releaseVtTokenArray);

} // namespace cxxfacade
'''

# Sdf header template
SDF_HPP_TEMPLATE = '''// Sdf.hpp - Scene description foundation (pimpl pattern)
// CLEAN HEADER - NO pxr includes!
#pragma once

#ifdef __swift__
#include <swift/bridging>
#else
#define SWIFT_SELF_CONTAINED
#define SWIFT_SHARED_REFERENCE(retain, release)
#define SWIFT_RETURNS_RETAINED
#endif

#include <string>
#include <vector>
#include <atomic>
#include "../base/Tf.hpp"

namespace cxxfacade {

namespace detail {
    struct SdfPathImpl;
    struct SdfLayerImpl;
}

// ============================================================================
// SdfPath - Scene description path (Value Type)
// ============================================================================

class SdfPath {
    void* impl_;
public:
    SdfPath();
    explicit SdfPath(const std::string& path);
    SdfPath(const SdfPath& other);
    SdfPath(SdfPath&& other) noexcept;
    SdfPath& operator=(const SdfPath& other);
    SdfPath& operator=(SdfPath&& other) noexcept;
    ~SdfPath();

    std::string GetAsString() const;
    std::string GetName() const;
    TfToken GetNameToken() const;

    bool IsEmpty() const;
    bool IsAbsolutePath() const;
    bool IsRootPrimPath() const;
    bool IsPrimPath() const;
    bool IsPropertyPath() const;

    SdfPath GetParentPath() const;
    SdfPath AppendChild(const TfToken& childName) const;
    SdfPath AppendProperty(const TfToken& propName) const;

    bool operator==(const SdfPath& other) const;
    bool operator!=(const SdfPath& other) const;
    bool operator<(const SdfPath& other) const;

    size_t GetHash() const;
    void* GetRawPtr() const { return impl_; }

    static SdfPath AbsoluteRootPath();
    static SdfPath EmptyPath();
} SWIFT_SELF_CONTAINED;

// ============================================================================
// SdfAssetPath - Asset reference path (Value Type)
// ============================================================================

class SdfAssetPath {
    std::string path_;
    std::string resolvedPath_;
public:
    SdfAssetPath() = default;
    explicit SdfAssetPath(const std::string& path);
    SdfAssetPath(const std::string& path, const std::string& resolvedPath);

    const std::string& GetAssetPath() const { return path_; }
    const std::string& GetResolvedPath() const { return resolvedPath_; }

    bool operator==(const SdfAssetPath& other) const;
    bool operator<(const SdfAssetPath& other) const;
} SWIFT_SELF_CONTAINED;

// ============================================================================
// SdfLayerOffset - Time offset/scale for layer composition (Value Type)
// ============================================================================

struct SdfLayerOffset {
    double offset;
    double scale;

    SdfLayerOffset() : offset(0.0), scale(1.0) {}
    SdfLayerOffset(double off, double scl = 1.0) : offset(off), scale(scl) {}

    bool IsIdentity() const { return offset == 0.0 && scale == 1.0; }
    SdfLayerOffset GetInverse() const;
    SdfLayerOffset operator*(const SdfLayerOffset& other) const;
} SWIFT_SELF_CONTAINED;

// ============================================================================
// SdfLayer - Scene description layer (Reference Type)
// ============================================================================

class SdfLayer;
void retainSdfLayer(SdfLayer* layer);
void releaseSdfLayer(SdfLayer* layer);

class SdfLayer {
    detail::SdfLayerImpl* impl_;
    mutable std::atomic<int> refCount_{1};
public:
    ~SdfLayer();

    static SdfLayer* CreateNew(const std::string& identifier) SWIFT_RETURNS_RETAINED;
    static SdfLayer* CreateAnonymous(const std::string& tag = "") SWIFT_RETURNS_RETAINED;
    static SdfLayer* FindOrOpen(const std::string& identifier) SWIFT_RETURNS_RETAINED;

    std::string GetIdentifier() const;
    std::string GetDisplayName() const;
    std::string GetRealPath() const;

    bool Save(bool force = false) const;
    bool Export(const std::string& filename) const;

    bool IsAnonymous() const;
    bool IsDirty() const;
    bool IsEmpty() const;

    double GetStartTimeCode() const;
    double GetEndTimeCode() const;
    void SetStartTimeCode(double time);
    void SetEndTimeCode(double time);

    void* GetRawPtr() const;

    friend void retainSdfLayer(SdfLayer* layer);
    friend void releaseSdfLayer(SdfLayer* layer);

private:
    explicit SdfLayer(detail::SdfLayerImpl* impl);
    SdfLayer(const SdfLayer&) = delete;
    SdfLayer& operator=(const SdfLayer&) = delete;
} SWIFT_SHARED_REFERENCE(retainSdfLayer, releaseSdfLayer);

} // namespace cxxfacade
'''

# Usd core header template
USD_HPP_TEMPLATE = '''// Usd.hpp - Core USD API (pimpl pattern)
// CLEAN HEADER - NO pxr includes!
#pragma once

#ifdef __swift__
#include <swift/bridging>
#else
#define SWIFT_SELF_CONTAINED
#define SWIFT_SHARED_REFERENCE(retain, release)
#define SWIFT_RETURNS_RETAINED
#endif

#include <string>
#include <vector>
#include <atomic>
#include "../base/Tf.hpp"
#include "../base/Gf.hpp"
#include "../base/Vt.hpp"
#include "Sdf.hpp"

namespace cxxfacade {

namespace detail {
    struct UsdStageImpl;
    struct UsdPrimImpl;
    struct UsdAttributeImpl;
    struct UsdRelationshipImpl;
}

// Forward declarations
class UsdStage;
class UsdPrim;
class UsdAttribute;
class UsdRelationship;

// ============================================================================
// UsdTimeCode - Time value for animation (Value Type)
// ============================================================================

struct UsdTimeCode {
    double value;
    bool isDefault;

    UsdTimeCode() : value(0.0), isDefault(true) {}
    explicit UsdTimeCode(double t) : value(t), isDefault(false) {}

    double GetValue() const { return value; }
    bool IsDefault() const { return isDefault; }

    bool operator==(const UsdTimeCode& other) const;
    bool operator<(const UsdTimeCode& other) const;

    static UsdTimeCode Default() { return UsdTimeCode(); }
    static UsdTimeCode EarliestTime();
} SWIFT_SELF_CONTAINED;

// ============================================================================
// Retain/Release declarations
// ============================================================================

void retainUsdStage(UsdStage* stage);
void releaseUsdStage(UsdStage* stage);
void retainUsdPrim(UsdPrim* prim);
void releaseUsdPrim(UsdPrim* prim);
void retainUsdAttribute(UsdAttribute* attr);
void releaseUsdAttribute(UsdAttribute* attr);
void retainUsdRelationship(UsdRelationship* rel);
void releaseUsdRelationship(UsdRelationship* rel);

// ============================================================================
// UsdStage - Root container for scene description (Reference Type)
// ============================================================================

class UsdStage {
    detail::UsdStageImpl* impl_;
    mutable std::atomic<int> refCount_{1};
public:
    ~UsdStage();

    // Factory methods
    static UsdStage* CreateNew(const std::string& identifier) SWIFT_RETURNS_RETAINED;
    static UsdStage* CreateInMemory() SWIFT_RETURNS_RETAINED;
    static UsdStage* Open(const std::string& filePath) SWIFT_RETURNS_RETAINED;

    // Prim access
    UsdPrim* GetPseudoRoot() SWIFT_RETURNS_RETAINED;
    UsdPrim* GetDefaultPrim() SWIFT_RETURNS_RETAINED;
    UsdPrim* GetPrimAtPath(const SdfPath& path) SWIFT_RETURNS_RETAINED;
    UsdPrim* DefinePrim(const SdfPath& path, const TfToken& typeName = TfToken()) SWIFT_RETURNS_RETAINED;
    UsdPrim* OverridePrim(const SdfPath& path) SWIFT_RETURNS_RETAINED;
    bool RemovePrim(const SdfPath& path);

    // Time
    double GetStartTimeCode() const;
    double GetEndTimeCode() const;
    void SetStartTimeCode(double time);
    void SetEndTimeCode(double time);
    double GetFramesPerSecond() const;
    void SetFramesPerSecond(double fps);

    // I/O
    bool Save();
    bool Export(const std::string& filename) const;
    SdfLayer* GetRootLayer() SWIFT_RETURNS_RETAINED;

    // Metadata
    std::string GetRootLayerIdentifier() const;

    void* GetRawPtr() const;

    friend void retainUsdStage(UsdStage* stage);
    friend void releaseUsdStage(UsdStage* stage);

private:
    explicit UsdStage(detail::UsdStageImpl* impl);
    UsdStage(const UsdStage&) = delete;
    UsdStage& operator=(const UsdStage&) = delete;
} SWIFT_SHARED_REFERENCE(retainUsdStage, releaseUsdStage);

// ============================================================================
// UsdPrim - Scene element (Reference Type)
// ============================================================================

class UsdPrim {
    detail::UsdPrimImpl* impl_;
    mutable std::atomic<int> refCount_{1};
public:
    ~UsdPrim();

    // Identity
    SdfPath GetPath() const;
    TfToken GetName() const;
    TfToken GetTypeName() const;
    std::string GetDescription() const;

    // Validity
    bool IsValid() const;
    bool IsActive() const;
    bool IsDefined() const;
    bool IsAbstract() const;
    bool IsModel() const;
    bool IsGroup() const;

    // Hierarchy
    UsdPrim* GetParent() SWIFT_RETURNS_RETAINED;
    std::vector<UsdPrim*> GetChildren();
    std::vector<UsdPrim*> GetAllChildren();
    bool HasAuthoredReferences() const;

    // Attributes
    UsdAttribute* GetAttribute(const TfToken& name) SWIFT_RETURNS_RETAINED;
    UsdAttribute* CreateAttribute(const TfToken& name, const TfToken& typeName) SWIFT_RETURNS_RETAINED;
    std::vector<UsdAttribute*> GetAttributes();

    // Relationships
    UsdRelationship* GetRelationship(const TfToken& name) SWIFT_RETURNS_RETAINED;
    UsdRelationship* CreateRelationship(const TfToken& name) SWIFT_RETURNS_RETAINED;

    // Metadata
    bool HasMetadata(const TfToken& key) const;

    void* GetRawPtr() const;

    friend void retainUsdPrim(UsdPrim* prim);
    friend void releaseUsdPrim(UsdPrim* prim);

private:
    explicit UsdPrim(detail::UsdPrimImpl* impl);
    UsdPrim(const UsdPrim&) = delete;
    UsdPrim& operator=(const UsdPrim&) = delete;
} SWIFT_SHARED_REFERENCE(retainUsdPrim, releaseUsdPrim);

// ============================================================================
// UsdAttribute - Typed property (Reference Type)
// ============================================================================

class UsdAttribute {
    detail::UsdAttributeImpl* impl_;
    mutable std::atomic<int> refCount_{1};
public:
    ~UsdAttribute();

    TfToken GetName() const;
    TfToken GetTypeName() const;
    SdfPath GetPath() const;

    bool IsValid() const;
    bool IsDefined() const;
    bool HasValue() const;
    bool HasAuthoredValue() const;

    // Typed getters - return true on success
    bool GetBool(UsdTimeCode time = UsdTimeCode::Default()) const;
    int GetInt(UsdTimeCode time = UsdTimeCode::Default()) const;
    float GetFloat(UsdTimeCode time = UsdTimeCode::Default()) const;
    double GetDouble(UsdTimeCode time = UsdTimeCode::Default()) const;
    std::string GetString(UsdTimeCode time = UsdTimeCode::Default()) const;
    TfToken GetToken(UsdTimeCode time = UsdTimeCode::Default()) const;
    Vec3f GetVec3f(UsdTimeCode time = UsdTimeCode::Default()) const;
    Vec3d GetVec3d(UsdTimeCode time = UsdTimeCode::Default()) const;
    Matrix4d GetMatrix4d(UsdTimeCode time = UsdTimeCode::Default()) const;

    // Typed setters
    bool SetBool(bool value, UsdTimeCode time = UsdTimeCode::Default());
    bool SetInt(int value, UsdTimeCode time = UsdTimeCode::Default());
    bool SetFloat(float value, UsdTimeCode time = UsdTimeCode::Default());
    bool SetDouble(double value, UsdTimeCode time = UsdTimeCode::Default());
    bool SetString(const std::string& value, UsdTimeCode time = UsdTimeCode::Default());
    bool SetToken(const TfToken& value, UsdTimeCode time = UsdTimeCode::Default());
    bool SetVec3f(const Vec3f& value, UsdTimeCode time = UsdTimeCode::Default());
    bool SetVec3d(const Vec3d& value, UsdTimeCode time = UsdTimeCode::Default());
    bool SetMatrix4d(const Matrix4d& value, UsdTimeCode time = UsdTimeCode::Default());

    void* GetRawPtr() const;

    friend void retainUsdAttribute(UsdAttribute* attr);
    friend void releaseUsdAttribute(UsdAttribute* attr);

private:
    explicit UsdAttribute(detail::UsdAttributeImpl* impl);
    UsdAttribute(const UsdAttribute&) = delete;
    UsdAttribute& operator=(const UsdAttribute&) = delete;
} SWIFT_SHARED_REFERENCE(retainUsdAttribute, releaseUsdAttribute);

// ============================================================================
// UsdRelationship - Prim relationship (Reference Type)
// ============================================================================

class UsdRelationship {
    detail::UsdRelationshipImpl* impl_;
    mutable std::atomic<int> refCount_{1};
public:
    ~UsdRelationship();

    TfToken GetName() const;
    SdfPath GetPath() const;
    bool IsValid() const;

    bool AddTarget(const SdfPath& target);
    bool RemoveTarget(const SdfPath& target);
    bool SetTargets(const std::vector<SdfPath>& targets);
    std::vector<SdfPath> GetTargets() const;
    bool ClearTargets();

    void* GetRawPtr() const;

    friend void retainUsdRelationship(UsdRelationship* rel);
    friend void releaseUsdRelationship(UsdRelationship* rel);

private:
    explicit UsdRelationship(detail::UsdRelationshipImpl* impl);
    UsdRelationship(const UsdRelationship&) = delete;
    UsdRelationship& operator=(const UsdRelationship&) = delete;
} SWIFT_SHARED_REFERENCE(retainUsdRelationship, releaseUsdRelationship);

} // namespace cxxfacade
'''

# UsdGeom header template
USDGEOM_HPP_TEMPLATE = '''// UsdGeom.hpp - Geometry schemas (pimpl pattern)
// CLEAN HEADER - NO pxr includes!
#pragma once

#ifdef __swift__
#include <swift/bridging>
#else
#define SWIFT_SHARED_REFERENCE(retain, release)
#define SWIFT_RETURNS_RETAINED
#endif

#include <string>
#include <vector>
#include <atomic>
#include "../base/Gf.hpp"
#include "../base/Vt.hpp"
#include "../usd/Usd.hpp"

namespace cxxfacade {

namespace detail {
    struct UsdGeomXformImpl;
    struct UsdGeomMeshImpl;
    struct UsdGeomCameraImpl;
}

// Forward declarations
class UsdGeomXform;
class UsdGeomMesh;
class UsdGeomCamera;

void retainUsdGeomXform(UsdGeomXform* xform);
void releaseUsdGeomXform(UsdGeomXform* xform);
void retainUsdGeomMesh(UsdGeomMesh* mesh);
void releaseUsdGeomMesh(UsdGeomMesh* mesh);
void retainUsdGeomCamera(UsdGeomCamera* camera);
void releaseUsdGeomCamera(UsdGeomCamera* camera);

// ============================================================================
// UsdGeomXform - Transformable prim (Reference Type)
// ============================================================================

class UsdGeomXform {
    detail::UsdGeomXformImpl* impl_;
    mutable std::atomic<int> refCount_{1};
public:
    ~UsdGeomXform();

    static UsdGeomXform* Define(UsdStage* stage, const SdfPath& path) SWIFT_RETURNS_RETAINED;
    static UsdGeomXform* Get(UsdStage* stage, const SdfPath& path) SWIFT_RETURNS_RETAINED;

    UsdPrim* GetPrim() SWIFT_RETURNS_RETAINED;
    SdfPath GetPath() const;

    // Transform operations
    bool SetTranslate(const Vec3d& translate, UsdTimeCode time = UsdTimeCode::Default());
    bool SetRotateXYZ(const Vec3d& rotation, UsdTimeCode time = UsdTimeCode::Default());
    bool SetScale(const Vec3d& scale, UsdTimeCode time = UsdTimeCode::Default());

    Matrix4d GetLocalTransformation(UsdTimeCode time = UsdTimeCode::Default()) const;
    bool ClearXformOpOrder();

    void* GetRawPtr() const;

    friend void retainUsdGeomXform(UsdGeomXform* xform);
    friend void releaseUsdGeomXform(UsdGeomXform* xform);

private:
    explicit UsdGeomXform(detail::UsdGeomXformImpl* impl);
    UsdGeomXform(const UsdGeomXform&) = delete;
    UsdGeomXform& operator=(const UsdGeomXform&) = delete;
} SWIFT_SHARED_REFERENCE(retainUsdGeomXform, releaseUsdGeomXform);

// ============================================================================
// UsdGeomMesh - Polygon mesh (Reference Type)
// ============================================================================

class UsdGeomMesh {
    detail::UsdGeomMeshImpl* impl_;
    mutable std::atomic<int> refCount_{1};
public:
    ~UsdGeomMesh();

    static UsdGeomMesh* Define(UsdStage* stage, const SdfPath& path) SWIFT_RETURNS_RETAINED;
    static UsdGeomMesh* Get(UsdStage* stage, const SdfPath& path) SWIFT_RETURNS_RETAINED;

    UsdPrim* GetPrim() SWIFT_RETURNS_RETAINED;
    SdfPath GetPath() const;

    // Topology
    bool SetPoints(const VtVec3fArray& points, UsdTimeCode time = UsdTimeCode::Default());
    bool SetFaceVertexCounts(const VtIntArray& counts, UsdTimeCode time = UsdTimeCode::Default());
    bool SetFaceVertexIndices(const VtIntArray& indices, UsdTimeCode time = UsdTimeCode::Default());
    bool SetNormals(const VtVec3fArray& normals, UsdTimeCode time = UsdTimeCode::Default());

    // Subdivision
    TfToken GetSubdivisionScheme() const;
    bool SetSubdivisionScheme(const TfToken& scheme);

    void* GetRawPtr() const;

    friend void retainUsdGeomMesh(UsdGeomMesh* mesh);
    friend void releaseUsdGeomMesh(UsdGeomMesh* mesh);

private:
    explicit UsdGeomMesh(detail::UsdGeomMeshImpl* impl);
    UsdGeomMesh(const UsdGeomMesh&) = delete;
    UsdGeomMesh& operator=(const UsdGeomMesh&) = delete;
} SWIFT_SHARED_REFERENCE(retainUsdGeomMesh, releaseUsdGeomMesh);

// ============================================================================
// UsdGeomCamera - Camera prim (Reference Type)
// ============================================================================

class UsdGeomCamera {
    detail::UsdGeomCameraImpl* impl_;
    mutable std::atomic<int> refCount_{1};
public:
    ~UsdGeomCamera();

    static UsdGeomCamera* Define(UsdStage* stage, const SdfPath& path) SWIFT_RETURNS_RETAINED;
    static UsdGeomCamera* Get(UsdStage* stage, const SdfPath& path) SWIFT_RETURNS_RETAINED;

    UsdPrim* GetPrim() SWIFT_RETURNS_RETAINED;

    // Camera parameters
    float GetFocalLength(UsdTimeCode time = UsdTimeCode::Default()) const;
    bool SetFocalLength(float focalLength, UsdTimeCode time = UsdTimeCode::Default());

    float GetHorizontalAperture(UsdTimeCode time = UsdTimeCode::Default()) const;
    bool SetHorizontalAperture(float aperture, UsdTimeCode time = UsdTimeCode::Default());

    float GetVerticalAperture(UsdTimeCode time = UsdTimeCode::Default()) const;
    bool SetVerticalAperture(float aperture, UsdTimeCode time = UsdTimeCode::Default());

    Vec2f GetClippingRange(UsdTimeCode time = UsdTimeCode::Default()) const;
    bool SetClippingRange(const Vec2f& range, UsdTimeCode time = UsdTimeCode::Default());

    void* GetRawPtr() const;

    friend void retainUsdGeomCamera(UsdGeomCamera* camera);
    friend void releaseUsdGeomCamera(UsdGeomCamera* camera);

private:
    explicit UsdGeomCamera(detail::UsdGeomCameraImpl* impl);
    UsdGeomCamera(const UsdGeomCamera&) = delete;
    UsdGeomCamera& operator=(const UsdGeomCamera&) = delete;
} SWIFT_SHARED_REFERENCE(retainUsdGeomCamera, releaseUsdGeomCamera);

} // namespace cxxfacade
'''

# Template dictionary for prompt generation
TEMPLATES = {
    "Tf": {"header": TF_HPP_TEMPLATE, "impl": TF_CPP_TEMPLATE},
    "Gf": {"header": GF_HPP_TEMPLATE},
    "Vt": {"header": VT_HPP_TEMPLATE},
    "Sdf": {"header": SDF_HPP_TEMPLATE},
    "Usd": {"header": USD_HPP_TEMPLATE},
    "UsdGeom": {"header": USDGEOM_HPP_TEMPLATE},
}


# ============================================================================
# Path Helpers
# ============================================================================

def get_base_path() -> Path:
    """Get the SwiftUSD project root."""
    return Path(__file__).parent.parent.resolve()


def get_expected_files(module: Module) -> ModuleFiles:
    """Get expected file paths for a module."""
    if module.name == "setup":
        return ModuleFiles(
            header_file="Sources/CxxFacade/include/CxxFacade.h",
            impl_file="Sources/CxxFacade/src/CxxFacade.cpp",
            swift_file="",
            test_file="",
        )

    return ModuleFiles(
        header_file=f"Sources/CxxFacade/include/{module.folder}/{module.name}.hpp",
        impl_file=f"Sources/CxxFacade/src/{module.folder}/{module.name}.cpp",
        swift_file=f"Sources/SwiftUSD/{module.name}/{module.name}.swift",
        test_file=f"Tests/SwiftUSDTests/{module.name}Tests.swift",
    )


# ============================================================================
# Progress Tracking
# ============================================================================

PROGRESS_FILE = Path(__file__).parent.parent / "IMPLEMENTATION_PROGRESS_CXXFACADE.md"
FAILURE_LOG = Path(__file__).parent.parent / "IMPLEMENTATION_FAILURES_CXXFACADE.log"

STATUS_SYMBOLS = {
    "pending": "⬜",
    "in_progress": "🔶",
    "completed": "✅",
    "failed": "❌",
    "skipped": "⏭️",
}


def load_progress() -> dict:
    """Load progress from markdown file."""
    if not PROGRESS_FILE.exists():
        return {}

    progress = {}
    with open(PROGRESS_FILE) as f:
        for line in f:
            line = line.strip()
            if line.startswith("|") and not line.startswith("|--") and "Module" not in line:
                parts = [p.strip() for p in line.split("|")]
                if len(parts) >= 3:
                    module_name = parts[1]
                    status_symbol = parts[2]
                    for status, symbol in STATUS_SYMBOLS.items():
                        if symbol in status_symbol:
                            progress[module_name] = status
                            break
    return progress


def save_progress(modules: List[Module]):
    """Save progress to markdown file."""
    with open(PROGRESS_FILE, "w") as f:
        f.write("# CxxFacade + SwiftUSD Implementation Progress\n\n")
        f.write(f"Last updated: {datetime.now().strftime('%Y-%m-%d %H:%M')}\n\n")
        f.write("## Architecture\n\n")
        f.write("```\n")
        f.write("SwiftUSD (Swift API)\n")
        f.write("    ↓\n")
        f.write("CxxFacade (C++ pimpl - clean headers)\n")
        f.write("    ↓\n")
        f.write("Vendor/USD/*.dylib (Pixar OpenUSD)\n")
        f.write("```\n\n")

        total = len([m for m in modules if m.name != "setup"])
        completed = sum(1 for m in modules if m.status == "completed" and m.name != "setup")
        f.write(f"**Progress**: {completed}/{total} modules\n\n")

        current_phase = None
        for m in modules:
            if m.phase != current_phase:
                current_phase = m.phase
                f.write(f"\n## Phase {m.phase.value}: {m.phase.name}\n\n")
                f.write("| Module | Status | Types | Description |\n")
                f.write("|--------|--------|-------|-------------|\n")

            status_symbol = STATUS_SYMBOLS.get(m.status, "⬜")
            types_str = ", ".join(m.types[:3]) + ("..." if len(m.types) > 3 else "")
            f.write(f"| {m.name} | {status_symbol} | {types_str} | {m.description[:50]} |\n")


# ============================================================================
# Prompt Generation
# ============================================================================

def generate_setup_prompt() -> str:
    """Generate prompt for Phase 0: Setup."""
    return '''
ultrathink

TASK: Setup CxxFacade + SwiftUSD Architecture

You are implementing a new architecture for SwiftUSD using the pimpl (Pointer to
Implementation) pattern to solve Clang modules incompatibility with Pixar USD headers.

## The Problem

Swift's C++ interop uses Clang modules. Pixar USD headers have `#include` statements
inside `PXR_NAMESPACE_OPEN_SCOPE` blocks, which breaks Clang modules parsing.

## The Solution

CxxFacade uses pimpl pattern:
- Headers in `include/` have NO pxr includes - Swift parses these
- Implementations in `src/` have pxr includes - Swift never sees these

## Directory Structure to Create

```
Sources/
├── CxxFacade/
│   ├── include/
│   │   ├── CxxFacade.h           # Swift macros, platform detection
│   │   ├── module.modulemap      # Clang module definition
│   │   ├── base/                 # Tf, Gf, Vt
│   │   ├── usd/                  # Sdf, Kind, Usd
│   │   ├── usdGeom/
│   │   ├── usdShade/
│   │   ├── usdLux/
│   │   ├── usdSkel/
│   │   └── imaging/
│   └── src/
│       ├── CxxFacade.cpp         # Placeholder for SPM
│       ├── base/
│       ├── usd/
│       ├── usdGeom/
│       ├── usdShade/
│       ├── usdLux/
│       ├── usdSkel/
│       └── imaging/
└── SwiftUSD/
    ├── Tf/, Gf/, Vt/
    ├── Sdf/, Kind/, Usd/
    ├── UsdGeom/, UsdShade/, UsdLux/, UsdSkel/
    └── Hydra/
```

## Files to Create

### 1. Sources/CxxFacade/include/CxxFacade.h

```cpp
// CxxFacade.h - Swift/C++ interop prelude
#pragma once

#if __has_include(<swift/bridging>)
#include <swift/bridging>
#else
#define SWIFT_SHARED_REFERENCE(retain, release)
#define SWIFT_IMMORTAL_REFERENCE
#define SWIFT_SELF_CONTAINED
#define SWIFT_UNSAFE_REFERENCE
#define SWIFT_RETURNS_RETAINED
#endif

#include <atomic>
#include <string>
#include <cstdint>
#include <vector>
#include <memory>

// Platform detection
#if defined(__APPLE__)
    #define CXXFACADE_PLATFORM_APPLE 1
#elif defined(__linux__)
    #define CXXFACADE_PLATFORM_LINUX 1
#elif defined(_WIN32)
    #define CXXFACADE_PLATFORM_WINDOWS 1
#endif

// Namespace
namespace cxxfacade {
    // All types will be in this namespace
}
```

### 2. Sources/CxxFacade/include/module.modulemap

```
module CxxFacade [system] {
    requires cplusplus

    header "CxxFacade.h"

    module Base {
        header "base/Tf.hpp"
        header "base/Gf.hpp"
        header "base/Vt.hpp"
        export *
    }

    module Usd {
        header "usd/Sdf.hpp"
        header "usd/Kind.hpp"
        header "usd/Usd.hpp"
        export *
    }

    module Schemas {
        header "usdGeom/UsdGeom.hpp"
        header "usdShade/UsdShade.hpp"
        header "usdLux/UsdLux.hpp"
        header "usdSkel/UsdSkel.hpp"
        export *
    }

    module Imaging {
        header "imaging/Hydra.hpp"
        export *
    }

    export *
}
```

### 3. Sources/CxxFacade/src/CxxFacade.cpp

```cpp
// Placeholder to satisfy Swift Package Manager
namespace cxxfacade {}
```

### 4. Update Package.swift

Create a new Package.swift that:
- Defines CxxFacade target with C++ sources
- Defines SwiftUSD target depending on CxxFacade
- Links to USD libraries in Vendor/USD/darwin/lib/
- Uses .interoperabilityMode(.Cxx) for Swift targets

Key settings:
- CxxFacade: publicHeadersPath: "include", sources: ["src"]
- headerSearchPath to Vendor/USD/darwin/include (for .cpp files)
- Link: usd_tf, usd_gf, usd_vt, usd_sdf, usd_usd, usd_usdGeom, etc.

## IMPORTANT

1. Create ALL directories first
2. Create placeholder .hpp files in each include subdirectory (empty with #pragma once)
3. Create placeholder .cpp files in each src subdirectory
4. The module.modulemap must list headers that exist

## Verification

After creating files:
1. `swift build` should compile (may have warnings, but no errors)

When complete, output: IMPLEMENTATION_COMPLETE
If blocked, output: IMPLEMENTATION_BLOCKED: <reason>
'''


def generate_module_prompt(module: Module) -> str:
    """Generate the full implementation prompt for a module using 3-step workflow."""

    if module.name == "setup":
        return generate_setup_prompt()

    expected = get_expected_files(module)
    types_list = ", ".join(module.types)
    deps_list = ", ".join(module.dependencies) if module.dependencies else "None"

    # Get embedded template if available
    template = TEMPLATES.get(module.name, {})
    header_template = template.get("header", "")
    impl_template = template.get("impl", "")

    # Determine which pxr headers to study
    pxr_headers = []
    if module.folder == "base":
        if module.name == "Tf":
            pxr_headers = ["token.h", "type.h"]
        elif module.name == "Gf":
            pxr_headers = ["vec2f.h", "vec3f.h", "vec4f.h", "matrix4d.h", "quatd.h", "range3d.h", "bbox3d.h"]
        elif module.name == "Vt":
            pxr_headers = ["array.h", "value.h"]
    elif module.folder == "usd":
        if module.name == "Sdf":
            pxr_headers = ["path.h", "layer.h", "assetPath.h"]
        elif module.name == "Kind":
            pxr_headers = ["registry.h"]
        elif module.name == "Usd":
            pxr_headers = ["stage.h", "prim.h", "attribute.h", "relationship.h", "timeCode.h"]
    elif module.folder == "usdGeom":
        pxr_headers = ["xform.h", "mesh.h", "camera.h", "points.h", "basisCurves.h", "primvarsAPI.h"]
    elif module.folder == "usdShade":
        pxr_headers = ["material.h", "shader.h", "materialBindingAPI.h"]
    elif module.folder == "usdLux":
        pxr_headers = ["distantLight.h", "domeLight.h", "rectLight.h", "sphereLight.h"]
    elif module.folder == "usdSkel":
        pxr_headers = ["skeleton.h", "root.h", "animation.h", "bindingAPI.h"]
    elif module.folder == "imaging":
        pxr_headers = ["usdImagingGL/engine.h", "hd/engine.h", "hd/renderIndex.h"]

    pxr_headers_str = ", ".join(pxr_headers)

    # Build template section
    template_section = ""
    if header_template:
        template_section += f'''
### Reference Header Template

Use this as a starting point and expand with all types listed:

```cpp
{header_template[:2000]}{"..." if len(header_template) > 2000 else ""}
```
'''
    if impl_template:
        template_section += f'''
### Reference Implementation Template

```cpp
{impl_template[:1500]}{"..." if len(impl_template) > 1500 else ""}
```
'''

    return f'''
ultrathink

TASK: Implement {module.name} for CxxFacade + SwiftUSD

## 3-Step Implementation Workflow

This workflow ensures you understand the Pixar USD source before implementing.

---

## STEP 1: Analyze OpenUSD Source

**IMPORTANT**: First, read and understand the Pixar USD implementation.

**Path to study**: `OpenUSD/{module.pxr_path}/`
**Key headers**: {pxr_headers_str}

Questions to answer:
1. What are the main classes/structs in this module?
2. What methods do they expose?
3. What are the constructor patterns?
4. Are types value types (small, copyable) or reference types (shared ownership)?

Read the OpenUSD headers to understand:
- Public API surface
- Type definitions
- Key methods you need to wrap

---

## STEP 2: Implement CxxFacade (Pimpl Pattern)

Create clean C++ headers that Swift can parse (NO pxr includes!).

### Architecture Reminder

```
SwiftUSD (Swift) imports CxxFacade headers
    ↓
CxxFacade headers (include/) - CLEAN, no pxr
    ↓
CxxFacade implementation (src/) - includes pxr
    ↓
Pixar USD libraries (Vendor/USD/darwin/lib)
```

### Files to Create

**Clean Header**: `{expected.header_file}`
- NO pxr includes!
- Forward declare: `namespace detail {{ struct {module.name}Impl; }}`
- Use SWIFT_SELF_CONTAINED for value types
- Use SWIFT_SHARED_REFERENCE for reference types
- Retain/release at global scope

**Implementation**: `{expected.impl_file}`
- Include real pxr headers here
- Implement detail::{module.name}Impl with actual USD objects
- Wrap all USD calls
{template_section}
---

## STEP 3: Create SwiftUSD API

**Swift File**: `{expected.swift_file}`

```swift
import CxxFacade

// Extensions for Swift idioms:
// - Hashable, Equatable, CustomStringConvertible conformances
// - Convenience initializers
// - Computed properties for cleaner API
// - Operators where appropriate (+, -, *, etc. for math types)
```

---

## STEP 4: Create Tests

**Test File**: `{expected.test_file}`

```swift
import XCTest
import SwiftUSD
@testable import CxxFacade

final class {module.name}Tests: XCTestCase {{
    // Test creation, properties, methods for each type
}}
```

---

## Module Details

- **Name**: {module.name}
- **Description**: {module.description}
- **Types to implement**: {types_list}
- **Dependencies**: {deps_list}
- **Folder**: {module.folder}

---

## Implementation Patterns

### Value Type (SWIFT_SELF_CONTAINED)

For small, copyable types like Token, Vec3f, TimeCode:

```cpp
struct Vec3f {{
    float x, y, z;

    Vec3f() : x(0), y(0), z(0) {{}}
    Vec3f(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {{}}

    float Length() const;
    Vec3f Normalized() const;
    float Dot(const Vec3f& other) const;
    Vec3f Cross(const Vec3f& other) const;

    Vec3f operator+(const Vec3f& o) const {{ return Vec3f(x+o.x, y+o.y, z+o.z); }}
    Vec3f operator-(const Vec3f& o) const {{ return Vec3f(x-o.x, y-o.y, z-o.z); }}
    bool operator==(const Vec3f& o) const {{ return x==o.x && y==o.y && z==o.z; }}

    static Vec3f Zero() {{ return Vec3f(); }}
}} SWIFT_SELF_CONTAINED;
```

### Reference Type (SWIFT_SHARED_REFERENCE)

For types with shared ownership like Stage, Prim, Layer:

```cpp
namespace detail {{ struct StageImpl; }}

class Stage;
void retainStage(Stage* s);
void releaseStage(Stage* s);

class Stage {{
    detail::StageImpl* impl_;
    mutable std::atomic<int> refCount_{{1}};
public:
    static Stage* Open(const std::string& path) SWIFT_RETURNS_RETAINED;
    static Stage* CreateInMemory() SWIFT_RETURNS_RETAINED;
    ~Stage();

    UsdPrim* GetPrimAtPath(const SdfPath& path) SWIFT_RETURNS_RETAINED;
    bool Save();

    void* GetRawPtr() const;  // For advanced interop

    friend void retainStage(Stage* s);
    friend void releaseStage(Stage* s);
private:
    explicit Stage(detail::StageImpl* impl);
    Stage(const Stage&) = delete;
}} SWIFT_SHARED_REFERENCE(retainStage, releaseStage);
```

---

## Verification Checklist

Before marking complete, verify:
1. ✅ All 4 files exist with substantial content (>100 lines for main files)
2. ✅ `swift build` succeeds
3. ✅ `swift test --filter {module.name}Tests` passes
4. ✅ Header has ZERO pxr includes
5. ✅ All pxr includes are in .cpp file only

---

## CRITICAL RULES

1. **Headers must have ZERO pxr includes** - Swift parses these
2. **All pxr includes go in .cpp only** - Swift never sees these
3. **Use `detail::XxxImpl` structs** to hide USD types
4. **Reference types need retain/release** at global scope
5. **Test basic functionality** for each type

When complete, output: IMPLEMENTATION_COMPLETE
If blocked, output: IMPLEMENTATION_BLOCKED: <reason>
'''


def generate_fix_prompt(module: Module, verification: Optional[VerificationResult]) -> str:
    """Generate a prompt to fix specific issues."""
    issues = []

    # Handle case where verification is None (e.g., timeout before verification)
    if verification is None:
        return f'''
ultrathink

RETRY for {module.name}

The previous attempt timed out or failed before verification could complete.

Please continue implementing {module.name} with these types: {", ".join(module.types) if module.types else "N/A"}

Expected files:
- Sources/CxxFacade/include/{module.folder}/{module.name}.hpp
- Sources/CxxFacade/src/{module.folder}/{module.name}.cpp
- Sources/SwiftUSD/{module.name}/{module.name}.swift
- Tests/SwiftUSDTests/{module.name}Tests.swift

Read any existing files first, then complete or fix the implementation.

When complete, output: IMPLEMENTATION_COMPLETE
If blocked, output: IMPLEMENTATION_BLOCKED: <reason>
'''

    if not verification.files_ok:
        issues.append(f"Missing files:\n  - " + "\n  - ".join(verification.missing_files))

    if not verification.build_ok:
        # Extract key errors
        errors = []
        for line in verification.build_output.split('\n'):
            if 'error:' in line.lower():
                errors.append(line)
        errors_str = '\n'.join(errors[:15])
        issues.append(f"Build errors:\n{errors_str}")

    if not verification.tests_ok:
        issues.append(f"Test failures:\n{verification.test_output[-1000:]}")

    return f'''
ultrathink

FIX REQUIRED for {module.name}

The previous implementation has issues:

{chr(10).join(issues)}

Please fix these specific issues. Focus on:
1. Ensuring all expected files exist with substantial content (>100 lines)
2. Fixing any build errors
3. Making tests pass

Read the existing files first, then make targeted fixes.
Do NOT start from scratch unless absolutely necessary.

When fixed, output: IMPLEMENTATION_COMPLETE
If blocked, output: IMPLEMENTATION_BLOCKED: <reason>
'''


# ============================================================================
# Verification
# ============================================================================

def verify_files(module: Module, base_path: Path) -> tuple:
    """Check that expected files exist and have content."""
    expected = get_expected_files(module)
    missing = []

    files_to_check = [expected.header_file, expected.impl_file]
    if expected.swift_file:
        files_to_check.append(expected.swift_file)
    if expected.test_file:
        files_to_check.append(expected.test_file)

    for f in files_to_check:
        if not f:
            continue
        full_path = base_path / f
        if not full_path.exists():
            missing.append(f)
        elif full_path.stat().st_size < 100:
            missing.append(f"{f} (file too small)")

    return (len(missing) == 0, missing)


def verify_build(base_path: Path) -> tuple:
    """Run swift build. No timeout - runs until completion."""
    try:
        result = subprocess.run(
            ["swift", "build"],
            capture_output=True,
            text=True,
            cwd=str(base_path)
        )
        return (result.returncode == 0, result.stdout + result.stderr)
    except Exception as e:
        return (False, f"Build error: {str(e)}")


def verify_tests(module: Module, base_path: Path) -> tuple:
    """Run tests for a module. No timeout - runs until completion."""
    if module.name == "setup":
        return (True, "No tests for setup")

    try:
        result = subprocess.run(
            ["swift", "test", "--filter", f"{module.name}Tests"],
            capture_output=True,
            text=True,
            cwd=str(base_path)
        )
        return (result.returncode == 0, result.stdout + result.stderr)
    except Exception as e:
        return (False, f"Test error: {str(e)}")


def verify_module(module: Module, base_path: Path,
                  skip_build_test: bool = False) -> VerificationResult:
    """Full verification of module implementation."""

    # Check files
    files_ok, missing = verify_files(module, base_path)
    if not files_ok:
        return VerificationResult(
            files_ok=False, build_ok=False, tests_ok=False,
            missing_files=missing,
            build_output="Skipped (files missing)",
            test_output="Skipped (files missing)"
        )

    if skip_build_test:
        return VerificationResult(
            files_ok=True, build_ok=True, tests_ok=True,
            missing_files=[], build_output="Skipped", test_output="Skipped"
        )

    # Build
    build_ok, build_output = verify_build(base_path)
    if not build_ok:
        return VerificationResult(
            files_ok=True, build_ok=False, tests_ok=False,
            missing_files=[], build_output=build_output,
            test_output="Skipped (build failed)"
        )

    # Tests
    tests_ok, test_output = verify_tests(module, base_path)
    return VerificationResult(
        files_ok=True, build_ok=True, tests_ok=tests_ok,
        missing_files=[], build_output=build_output, test_output=test_output
    )


def log_failure(module: Module, verification: VerificationResult):
    """Log failure details."""
    with open(FAILURE_LOG, "a") as f:
        f.write(f"\n{'='*60}\n")
        f.write(f"Module: {module.name}\n")
        f.write(f"Time: {datetime.now()}\n")
        f.write(f"{'='*60}\n")

        if not verification.files_ok:
            f.write(f"Missing files: {verification.missing_files}\n")
        if not verification.build_ok:
            f.write(f"Build output:\n{verification.build_output[-2000:]}\n")
        if not verification.tests_ok:
            f.write(f"Test output:\n{verification.test_output[-2000:]}\n")


# ============================================================================
# Claude Runner
# ============================================================================

def run_claude(prompt: str, cwd: Path) -> str:
    """Run Claude Code with prompt. No timeout - runs until completion."""
    cmd = ["claude", "--print", "--dangerously-skip-permissions", "-p", prompt]

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

        print("\n" + "-" * 50)
        print("Claude Code Output:")
        print("-" * 50)

        for line in iter(process.stdout.readline, ''):
            if not line:
                break
            print(line, end='', flush=True)
            output_lines.append(line)

        process.wait()
        print("-" * 50 + "\n")
        return ''.join(output_lines)

    except FileNotFoundError:
        return "ERROR: 'claude' command not found"
    except Exception as e:
        return f"ERROR: {str(e)}"


# ============================================================================
# Cleanup
# ============================================================================

def cleanup_old_code(base_path: Path, dry_run: bool = False):
    """Remove old SwiftUSD, USDCxx, PixarUSD, OpenUSDInterop."""
    old_dirs = [
        base_path / "Sources" / "SwiftUSD",
        base_path / "Sources" / "USDCxx",
        base_path / "Sources" / "PixarUSD",
        base_path / "Sources" / "OpenUSDInterop",
    ]

    for d in old_dirs:
        if d.exists():
            if dry_run:
                print(f"[DRY-RUN] Would remove: {d}")
            else:
                shutil.rmtree(d)
                print(f"[REMOVED] {d}")


# ============================================================================
# Main
# ============================================================================

def list_modules():
    """Print all modules."""
    print("\nCxxFacade + SwiftUSD Modules")
    print("=" * 70)

    current_phase = None
    for m in MODULES:
        if m.phase != current_phase:
            current_phase = m.phase
            print(f"\n## Phase {m.phase.value}: {m.phase.name}")
            print("-" * 50)

        status = STATUS_SYMBOLS.get(m.status, "⬜")
        deps = f" → deps: {', '.join(m.dependencies)}" if m.dependencies else ""
        types = f" ({len(m.types)} types)" if m.types else ""
        print(f"  {status} {m.name:<12}{types:<15}{deps}")
        print(f"       {m.description}")


def recover_in_progress_modules(modules: List[Module], base_path: Path, skip_build_test: bool = False):
    """Recover modules stuck in 'in_progress' state by verifying their implementation."""
    for module in modules:
        if module.status == "in_progress":
            print(f"[RECOVERY] Checking stuck module: {module.name}")
            verification = verify_module(module, base_path, skip_build_test)
            if verification.success:
                print(f"[RECOVERY] {module.name} is actually complete!")
                module.status = "completed"
                module.completed_at = datetime.now().strftime("%Y-%m-%d %H:%M")
            else:
                print(f"[RECOVERY] {module.name} needs work, resetting to pending")
                if not verification.files_ok:
                    print(f"           Missing files: {verification.missing_files}")
                if not verification.build_ok:
                    print(f"           Build: FAILED")
                if not verification.tests_ok:
                    print(f"           Tests: FAILED")
                module.status = "pending"


def get_next_module(modules: List[Module], force_continue: bool = False) -> Optional[Module]:
    """Get next pending module with satisfied dependencies.

    Args:
        modules: List of modules to check
        force_continue: If True, skip modules with failed dependencies instead of blocking
    """
    for m in modules:
        if m.status != "pending":
            continue

        deps_ok = True
        deps_failed = False
        for dep in m.dependencies:
            # Check global MODULES for dependency status (not just filtered list)
            dep_module = next((mod for mod in MODULES if mod.name == dep), None)
            if dep_module:
                if dep_module.status == "failed":
                    deps_failed = True
                    deps_ok = False
                    break
                elif dep_module.status not in ("completed", "skipped"):
                    deps_ok = False
                    break

        # If dependencies failed and not forcing, skip this module
        if deps_failed and not force_continue:
            continue

        if deps_ok or (deps_failed and force_continue):
            return m
    return None


def main():
    parser = argparse.ArgumentParser(
        description="SwiftUSD CxxFacade Implementation Agent",
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
Examples:
  python Scripts/implement_swiftusd_cxxfacade.py                    # Run all
  python Scripts/implement_swiftusd_cxxfacade.py --module setup     # Setup only
  python Scripts/implement_swiftusd_cxxfacade.py --module Tf        # Single module
  python Scripts/implement_swiftusd_cxxfacade.py --start-phase 1    # Start from phase
  python Scripts/implement_swiftusd_cxxfacade.py --list             # List modules
  python Scripts/implement_swiftusd_cxxfacade.py --dry-run          # Preview prompts
  python Scripts/implement_swiftusd_cxxfacade.py --clean            # Remove old code
  python Scripts/implement_swiftusd_cxxfacade.py --retry-failed     # Retry failed modules
        """
    )
    parser.add_argument("--list", action="store_true", help="List all modules")
    parser.add_argument("--module", type=str, help="Implement specific module")
    parser.add_argument("--start-phase", type=int, default=0, help="Start from phase N")
    parser.add_argument("--dry-run", action="store_true", help="Preview prompts only")
    parser.add_argument("--clean", action="store_true", help="Remove old code")
    parser.add_argument("--max-retries", type=int, default=3, help="Max retries per module")
    parser.add_argument("--skip-build-test", action="store_true", help="Skip build/test verification")
    parser.add_argument("--retry-failed", action="store_true", help="Retry previously failed modules")
    parser.add_argument("--strict-deps", action="store_true",
                        help="Stop if a dependency fails (default: continue past failures)")
    parser.add_argument("--verbose", "-v", action="store_true", help="Verbose output")

    args = parser.parse_args()
    base_path = get_base_path()

    # Load progress
    progress = load_progress()
    for m in MODULES:
        m.status = progress.get(m.name, "pending")

    # Recover in_progress modules (from interrupted runs)
    # Verify if they are actually complete, otherwise reset to pending
    has_in_progress = any(m.status == "in_progress" for m in MODULES)
    if has_in_progress:
        print("\n[RECOVERY] Found modules in 'in_progress' state from previous run...")
        recover_in_progress_modules(MODULES, base_path, args.skip_build_test)
        save_progress(MODULES)

    # Reset failed modules if --retry-failed
    if args.retry_failed:
        for m in MODULES:
            if m.status == "failed":
                m.status = "pending"
                print(f"[RESET] {m.name} marked as pending for retry")

    if args.list:
        list_modules()
        return

    if args.clean:
        cleanup_old_code(base_path, args.dry_run)
        return

    print("=" * 70)
    print("SwiftUSD CxxFacade Implementation Agent")
    print("=" * 70)
    print(f"Architecture: SwiftUSD → CxxFacade (pimpl) → USD binaries")
    print(f"Base path: {base_path}")
    print(f"Platform: {platform.system()}")
    print()

    # Filter modules
    if args.module:
        target = next((m for m in MODULES if m.name == args.module), None)
        if not target:
            print(f"Unknown module: {args.module}")
            print("Use --list to see available modules")
            sys.exit(1)
        modules_to_run = [target]
    else:
        modules_to_run = [m for m in MODULES if m.phase.value >= args.start_phase]

    pending = [m for m in modules_to_run if m.status == "pending"]
    print(f"Modules to process: {len(pending)}")
    print()

    # Main loop
    iteration = 0
    start_time = time.time()

    # force_continue is the opposite of strict_deps
    # Default behavior (no --strict-deps): continue past dependency failures
    force_continue = not args.strict_deps

    while True:
        module = get_next_module(modules_to_run, force_continue=force_continue)
        if not module:
            break

        iteration += 1
        print(f"\n{'='*70}")
        print(f"[{iteration}] Implementing: {module.name}")
        print(f"    Phase: {module.phase.name}")
        print(f"    Types: {', '.join(module.types) if module.types else 'N/A'}")
        print(f"    Dependencies: {', '.join(module.dependencies) if module.dependencies else 'None'}")
        print("=" * 70)

        module.status = "in_progress"
        save_progress(MODULES)

        if args.dry_run:
            prompt = generate_module_prompt(module)
            print("\n--- PROMPT PREVIEW ---")
            print(prompt[:4000])
            if len(prompt) > 4000:
                print(f"\n... ({len(prompt) - 4000} more chars)")
            print("--- END PREVIEW ---\n")
            module.status = "completed"
            save_progress(MODULES)
            continue

        # Implementation loop with retries
        retry = 0
        verification = None
        module_start = time.time()

        while retry <= args.max_retries:
            if retry == 0:
                prompt = generate_module_prompt(module)
            else:
                print(f"\n[RETRY {retry}/{args.max_retries}]")
                prompt = generate_fix_prompt(module, verification)

            output = run_claude(prompt, base_path)

            if output.startswith("ERROR"):
                print(f"[ERROR] {output[:200]}")
                retry += 1
                continue

            # Verify
            print("\n[VERIFY] Checking implementation...")
            verification = verify_module(module, base_path, args.skip_build_test)

            if verification.success:
                elapsed = time.time() - module_start
                print(f"[OK] {module.name} completed in {elapsed:.1f}s")
                module.status = "completed"
                module.completed_at = datetime.now().strftime("%Y-%m-%d %H:%M")
                break
            else:
                print("[ISSUES FOUND]")
                if not verification.files_ok:
                    print(f"  - Missing: {verification.missing_files}")
                if not verification.build_ok:
                    print("  - Build: FAILED")
                if not verification.tests_ok:
                    print("  - Tests: FAILED")
                retry += 1

        if not verification or not verification.success:
            elapsed = time.time() - module_start
            print(f"[FAILED] {module.name} after {args.max_retries} retries ({elapsed:.1f}s)")
            module.status = "failed"
            if verification:
                log_failure(module, verification)
            print("[CONTINUING] Moving to next module...")

        save_progress(MODULES)

    # Summary
    total_time = time.time() - start_time
    completed = sum(1 for m in MODULES if m.status == "completed")
    failed = sum(1 for m in MODULES if m.status == "failed")

    print(f"\n{'='*70}")
    print("SUMMARY")
    print(f"{'='*70}")
    print(f"Total time: {total_time:.1f}s")
    print(f"Completed: {completed}/{len(MODULES)}")
    if failed:
        print(f"Failed: {failed}")
        print(f"See: {FAILURE_LOG}")
    print(f"Progress: {PROGRESS_FILE}")


if __name__ == "__main__":
    main()
