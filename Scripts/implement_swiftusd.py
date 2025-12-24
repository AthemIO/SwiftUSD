#!/usr/bin/env python3
"""
SwiftUSD Autonomous Implementation Agent

This script runs Claude Code in a loop to systematically implement
all SwiftUSD modules following the established 3-layer architecture.

Usage:
    python Scripts/implement_swiftusd.py [--start-phase N] [--module MODULE]
    python Scripts/implement_swiftusd.py --dry-run  # Preview prompts
    python Scripts/implement_swiftusd.py --list     # List all modules
"""

import subprocess
import os
import sys
import argparse
import time
from pathlib import Path
from dataclasses import dataclass, field
from enum import Enum
from typing import Optional, List
from datetime import datetime


class Phase(Enum):
    ARCHITECTURE = 0       # refactor_to_pxr_structure
    FOUNDATION = 1         # Arch, Tf, Gf, Vt
    SCENE_DESCRIPTION = 2  # Sdf, Kind
    USD_CORE = 3           # Usd
    USD_SCHEMAS = 4        # UsdGeom, UsdShade, UsdLux, UsdSkel, UsdVol, UsdProc
    UTILITIES = 5          # UsdUtils, Ar
    IMAGING_FOUNDATION = 6 # Garch, Hio, Hgi, HgiMetal, CameraUtil, GeomUtil, PxOsd
    HYDRA_CORE = 7         # Hf, Hd, HdSt, Hdx, HdMetal
    USD_IMAGING = 8        # UsdImaging, UsdSkelImaging, UsdVolImaging, UsdLuxImaging, UsdAppUtils


@dataclass
class Module:
    name: str
    phase: Phase
    category: Optional[str]  # "base", "usd", or None for top-level schemas (pxr/usdGeom/)
    dependencies: List[str] = field(default_factory=list)
    description: str = ""
    status: str = "pending"  # pending, in_progress, completed, failed
    completed_at: Optional[str] = None


@dataclass
class ModuleFiles:
    """Expected files for a module implementation."""
    swift_files: List[str]      # Required Swift files
    header_files: List[str]     # Required C headers
    cpp_files: List[str]        # Required C++ implementations
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


# Complete module registry in implementation order (33 modules)
# Goal: Complete USD pipeline with Hydra rendering via Metal
MODULES = [
    # Phase 0: Architecture (1 module)
    Module("refactor_to_pxr_structure", Phase.ARCHITECTURE, "meta", [],
           "Refactor existing code to pxr structure"),

    # Phase 1: Foundation (4 modules) - Essential types used everywhere
    Module("Arch", Phase.FOUNDATION, "base", [],
           "Platform, timing, filesystem"),
    Module("Tf", Phase.FOUNDATION, "base", ["Arch"],
           "TfToken (used in every API)"),
    Module("Gf", Phase.FOUNDATION, "base", ["Tf"],
           "Vec3f, Matrix4d, Quaternion, Range, BBox, Frustum"),
    Module("Vt", Phase.FOUNDATION, "base", ["Gf"],
           "VtArray, VtValue, VtDictionary"),

    # Phase 2: Scene Description (2 modules) - Layer and path fundamentals
    Module("Sdf", Phase.SCENE_DESCRIPTION, "usd", ["Tf", "Vt"],
           "SdfPath, SdfLayer, SdfAssetPath"),
    Module("Kind", Phase.SCENE_DESCRIPTION, "usd", ["Tf"],
           "Model hierarchy kinds"),

    # Phase 3: USD Core (1 module) - The heart of USD
    Module("Usd", Phase.USD_CORE, "usd", ["Sdf"],
           "Stage, Prim, Attribute, Property, Relationship, References, Payloads, Variants, EditContext"),

    # Phase 4: USD Schemas (6 modules) - Scene description schemas
    Module("UsdGeom", Phase.USD_SCHEMAS, None, ["Usd", "Gf"],
           "Mesh, Xform, Points, BasisCurves, NurbsCurves, Camera, Sphere, Cube, Cylinder, Cone, Capsule, Primvars, BoundingBox, XformCache"),
    Module("UsdShade", Phase.USD_SCHEMAS, None, ["Usd", "Sdf"],
           "Material, Shader, MaterialBindingAPI, Input, Output, ConnectableAPI, NodeGraph"),
    Module("UsdLux", Phase.USD_SCHEMAS, None, ["UsdGeom", "UsdShade"],
           "DistantLight, DomeLight, RectLight, SphereLight, DiskLight, CylinderLight, LightFilter, ShadowAPI"),
    Module("UsdSkel", Phase.USD_SCHEMAS, None, ["UsdGeom"],
           "Skeleton, SkelRoot, SkelAnimation, BlendShape, SkelBindingAPI"),
    Module("UsdVol", Phase.USD_SCHEMAS, None, ["UsdGeom"],
           "Volume, OpenVDBAsset, Field3DAsset"),
    Module("UsdProc", Phase.USD_SCHEMAS, None, ["Usd"],
           "GenerativeProcedural"),

    # Phase 5: Utilities (2 modules) - Convenience APIs
    Module("UsdUtils", Phase.UTILITIES, None, ["Usd"],
           "StageCache, StitchLayers, FlattenLayerStack, Dependencies"),
    Module("Ar", Phase.UTILITIES, "usd", ["Tf"],
           "Resolver, ResolverContext, ResolverContextBinder, Asset"),

    # Phase 6: Imaging Foundation (7 modules) - Graphics abstraction layer
    Module("Garch", Phase.IMAGING_FOUNDATION, "imaging", ["Arch", "Tf"],
           "GarchGLApi, Platform GL/Metal detection"),
    Module("Hio", Phase.IMAGING_FOUNDATION, "imaging", ["Garch", "Gf"],
           "Image loading, OpenEXR, texture formats"),
    Module("Hgi", Phase.IMAGING_FOUNDATION, "imaging", ["Garch", "Tf"],
           "HgiCmds, HgiBuffer, HgiTexture, HgiPipeline, HgiShader, HgiSampler"),
    Module("HgiMetal", Phase.IMAGING_FOUNDATION, "imaging", ["Hgi"],
           "Metal implementation of Hgi interfaces"),
    Module("CameraUtil", Phase.IMAGING_FOUNDATION, "imaging", ["Gf"],
           "Camera framing, conformance, screen window"),
    Module("GeomUtil", Phase.IMAGING_FOUNDATION, "imaging", ["Gf", "Vt"],
           "Mesh generation utilities (sphere, cube, cone, etc.)"),
    Module("PxOsd", Phase.IMAGING_FOUNDATION, "imaging", ["Gf", "Vt"],
           "OpenSubdiv integration, subdivision surfaces"),

    # Phase 7: Hydra Core (5 modules) - Render framework
    Module("Hf", Phase.HYDRA_CORE, "imaging", ["Tf"],
           "Plugin registry for Hydra delegates"),
    Module("Hd", Phase.HYDRA_CORE, "imaging", ["Hf", "Gf", "Vt", "Sdf"],
           "HdRenderIndex, HdRprim, HdSprim, HdBprim, HdSceneDelegate, HdRenderDelegate, HdMesh, HdBasisCurves, HdPoints, HdVolume"),
    Module("HdSt", Phase.HYDRA_CORE, "imaging", ["Hd", "Hgi", "Hio", "PxOsd"],
           "Storm renderer, HdStRenderDelegate, HdStMesh, HdStMaterial, HdStLight, HdStRenderPass"),
    Module("Hdx", Phase.HYDRA_CORE, "imaging", ["HdSt"],
           "HdxTaskController, HdxRenderTask, HdxColorizeTask, HdxSelectionTask, HdxPickTask, HdxShadowTask"),
    Module("HdMetal", Phase.HYDRA_CORE, "imaging", ["Hd", "Hgi", "HgiMetal"],
           "Metal-native render delegate (alternative to HdSt+HgiMetal)"),

    # Phase 8: USD Imaging (5 modules) - USD to Hydra bridge
    Module("UsdImaging", Phase.USD_IMAGING, "usdImaging", ["Hd", "Usd", "UsdGeom", "UsdShade", "UsdLux"],
           "UsdImagingDelegate, adapters for all USD schema types"),
    Module("UsdSkelImaging", Phase.USD_IMAGING, "usdImaging", ["UsdImaging", "UsdSkel"],
           "Skeleton visualization, skinning"),
    Module("UsdVolImaging", Phase.USD_IMAGING, "usdImaging", ["UsdImaging", "UsdVol"],
           "Volume rendering adapters"),
    Module("UsdLuxImaging", Phase.USD_IMAGING, "usdImaging", ["UsdImaging", "UsdLux"],
           "Light adapters for Hydra"),
    Module("UsdAppUtils", Phase.USD_IMAGING, "usdImaging", ["UsdImaging", "CameraUtil"],
           "FrameRecorder, ColorCorrection, rendererSettings"),
]

PROGRESS_FILE = Path(__file__).parent.parent / "IMPLEMENTATION_PROGRESS.md"


def load_progress() -> dict:
    """Load progress from markdown file (parses table format)."""
    if not Path(PROGRESS_FILE).exists():
        return {}

    progress = {}
    with open(PROGRESS_FILE) as f:
        for line in f:
            line = line.strip()
            # Parse table rows: | Module | Status | Description | Depends On |
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
            # Also parse Phase 0 checkbox format: - [ ] refactor_to_pxr_structure
            elif line.startswith("- [x]"):
                parts = line[6:].strip().split(" - ")
                if parts:
                    progress[parts[0].strip()] = "completed"
            elif line.startswith("- [ ]") and "IN_PROGRESS" in line.upper():
                parts = line[6:].strip().split(" - ")
                if parts:
                    progress[parts[0].strip()] = "in_progress"
    return progress


# Status symbols
STATUS_SYMBOLS = {
    "pending": "⬜",
    "in_progress": "🔶",
    "completed": "✅",
    "failed": "⏸️",
}

# Phase descriptions for the markdown
PHASE_DESCRIPTIONS = {
    Phase.ARCHITECTURE: "",
    Phase.FOUNDATION: "Essential types used everywhere",
    Phase.SCENE_DESCRIPTION: "Layer and path fundamentals",
    Phase.USD_CORE: "The heart of USD",
    Phase.USD_SCHEMAS: "Scene description schemas",
    Phase.UTILITIES: "Convenience APIs",
    Phase.IMAGING_FOUNDATION: "Graphics abstraction layer",
    Phase.HYDRA_CORE: "Render framework",
    Phase.USD_IMAGING: "USD to Hydra bridge",
}


def save_progress(modules: List[Module]):
    """Save progress to markdown file in table format."""
    with open(PROGRESS_FILE, "w") as f:
        f.write("# SwiftUSD Implementation Progress\n\n")
        f.write(f"Last updated: {datetime.now().strftime('%Y-%m-%d')}\n\n")
        f.write("**Goal**: Complete USD pipeline with Hydra rendering via Metal\n\n")

        # Summary stats
        total = len(modules)
        completed = sum(1 for m in modules if m.status == "completed")

        f.write(f"**Progress**: {completed}/{total} modules completed\n\n")

        # Find current focus
        in_progress = next((m for m in modules if m.status == "in_progress"), None)
        if in_progress:
            f.write(f"**Current Focus**: Phase {in_progress.phase.value} - {in_progress.phase.name} ({in_progress.name})\n")
        else:
            next_module = get_next_module(modules)
            if next_module:
                f.write(f"**Current Focus**: Phase {next_module.phase.value} - {next_module.phase.name}\n")

        f.write("\n---\n")

        # Write each phase
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

                # Phase 0 uses checkbox format
                if current_phase == Phase.ARCHITECTURE:
                    checkbox = "[x]" if m.status == "completed" else "[ ]"
                    status_suffix = ""
                    if m.status == "in_progress":
                        status_suffix = " - IN_PROGRESS"
                    f.write(f"- {checkbox} {m.name}{status_suffix}\n")
                    continue

                # Other phases use table format
                f.write("\n| Module | Status | Description | Depends On |\n")
                f.write("|--------|--------|-------------|------------|\n")

            # Skip Phase 0 modules in table (already written above)
            if current_phase == Phase.ARCHITECTURE:
                continue

            status_symbol = STATUS_SYMBOLS.get(m.status, "⬜")
            deps = ", ".join(m.dependencies) if m.dependencies else "-"
            f.write(f"| {m.name} | {status_symbol} | {m.description} | {deps} |\n")

        # Status Legend
        f.write("\n---\n\n## Status Legend\n\n")
        f.write("| Symbol | Meaning |\n")
        f.write("|--------|--------|\n")
        f.write("| ⬜ | Not started |\n")
        f.write("| 🔶 | In progress |\n")
        f.write("| ✅ | Complete |\n")
        f.write("| ⏸️ | Blocked |\n")

        # Architecture Overview section
        f.write("\n---\n\n## Architecture Overview\n\n")
        f.write("```\n")
        f.write("┌─────────────────────────────────────────────────────────────────┐\n")
        f.write("│                         Swift Apps                              │\n")
        f.write("├─────────────────────────────────────────────────────────────────┤\n")
        f.write("│  SwiftUSD (Pure Swift API)                                      │\n")
        f.write("│  ├── Tf, Gf, Vt (Foundation)                                    │\n")
        f.write("│  ├── Sdf, Usd (Scene Description)                               │\n")
        f.write("│  ├── UsdGeom, UsdShade, UsdLux... (Schemas)                     │\n")
        f.write("│  └── UsdImaging, Hydra (Rendering)                              │\n")
        f.write("├─────────────────────────────────────────────────────────────────┤\n")
        f.write("│  OpenUSDInterop (C/C++ Bridge)                                  │\n")
        f.write("├─────────────────────────────────────────────────────────────────┤\n")
        f.write("│  Pixar OpenUSD (libusd_ms)                                      │\n")
        f.write("└─────────────────────────────────────────────────────────────────┘\n")
        f.write("```\n")

        # Implementation Depth section
        f.write("\n---\n\n## Implementation Depth\n\n")
        f.write("| Phase | Depth | Rationale |\n")
        f.write("|-------|-------|----------|\n")
        f.write("| 0. Architecture | **Full** | One-time refactor |\n")
        f.write("| 1. Foundation | **Full** | Small, used everywhere |\n")
        f.write("| 2. Scene Description | **Core** | SdfPath full, SdfLayer basics |\n")
        f.write("| 3. USD Core | **Full** | This IS the product |\n")
        f.write("| 4. USD Schemas | **Full** | 90% of use cases |\n")
        f.write("| 5. Utilities | **Core** | StageCache, Ar resolver |\n")
        f.write("| 6. Imaging Foundation | **Core** | Hgi abstraction, Metal backend |\n")
        f.write("| 7. Hydra Core | **Full** | Render pipeline |\n")
        f.write("| 8. USD Imaging | **Full** | USD to Hydra bridge |\n")


def get_next_module(modules: List[Module], force_continue: bool = False) -> Optional[Module]:
    """Get the next module to implement based on dependency order.

    Args:
        modules: List of modules to check
        force_continue: If True, skip modules with failed dependencies instead of blocking
    """
    for m in modules:
        if m.status == "pending":
            # Check all dependencies are completed (or failed if force_continue)
            deps_met = True
            deps_failed = False
            for dep in m.dependencies:
                dep_module = next((mod for mod in modules if mod.name == dep), None)
                if dep_module:
                    if dep_module.status == "failed":
                        deps_failed = True
                    elif dep_module.status != "completed":
                        deps_met = False
                        break

            if deps_met:
                if deps_failed and not force_continue:
                    # Skip this module - dependency failed and we're not forcing
                    continue
                return m
    return None


def get_pxr_path(module: Module) -> str:
    """Get the correct pxr/ path for a module.

    Base modules:   pxr/base/tf/
    USD modules:    pxr/usd/sdf/
    Schema modules: pxr/usdGeom/  (top-level, no parent)
    """
    pxr_name = module.name[0].lower() + module.name[1:]

    if module.category is None:
        # Schema modules are at pxr root: pxr/usdGeom/
        return f"pxr/{pxr_name}"
    else:
        # Base/USD modules have parent: pxr/base/tf/, pxr/usd/sdf/
        return f"pxr/{module.category}/{pxr_name}"


def get_interop_paths(module: Module) -> tuple:
    """Get C header and source paths for OpenUSDInterop.

    Returns (include_dir, src_dir) for the module.
    """
    pxr_name = module.name[0].lower() + module.name[1:]

    if module.category is None:
        # Schemas: Sources/OpenUSDInterop/include/usdGeom/
        return (
            f"Sources/OpenUSDInterop/include/{pxr_name}",
            f"Sources/OpenUSDInterop/src/{pxr_name}"
        )
    else:
        # Base/USD: Sources/OpenUSDInterop/include/base/
        return (
            f"Sources/OpenUSDInterop/include/{module.category}",
            f"Sources/OpenUSDInterop/src/{module.category}"
        )


def get_expected_files(module: Module) -> ModuleFiles:
    """Generate expected file paths for a module implementation."""
    pxr_name = module.name[0].lower() + module.name[1:]

    if module.name == "refactor_to_pxr_structure":
        # Phase 0 creates stubs - no specific file requirements
        return ModuleFiles(
            swift_files=[],
            header_files=[],
            cpp_files=[],
            test_files=[]
        )

    if module.category == "base":
        # Base modules: Arch, Tf, Gf, Vt
        return ModuleFiles(
            swift_files=[f"Sources/SwiftUSD/{module.name}/{module.name}.swift"],
            header_files=[f"Sources/OpenUSDInterop/include/base/{pxr_name}.h"],
            cpp_files=[f"Sources/OpenUSDInterop/src/base/{pxr_name}.cpp"],
            test_files=[f"Tests/SwiftUSDTests/{module.name}Tests.swift"]
        )
    elif module.category == "usd":
        # USD modules: Sdf, Kind, Usd, Ar
        return ModuleFiles(
            swift_files=[f"Sources/SwiftUSD/{module.name}/{module.name}.swift"],
            header_files=[f"Sources/OpenUSDInterop/include/usd/{pxr_name}.h"],
            cpp_files=[f"Sources/OpenUSDInterop/src/usd/{pxr_name}.cpp"],
            test_files=[f"Tests/SwiftUSDTests/{module.name}Tests.swift"]
        )
    else:
        # Schema modules: UsdGeom, UsdShade, UsdLux, etc.
        return ModuleFiles(
            swift_files=[f"Sources/SwiftUSD/{module.name}/{module.name}.swift"],
            header_files=[f"Sources/OpenUSDInterop/include/{pxr_name}/{pxr_name}.h"],
            cpp_files=[f"Sources/OpenUSDInterop/src/{pxr_name}/{pxr_name}.cpp"],
            test_files=[f"Tests/SwiftUSDTests/{module.name}Tests.swift"]
        )


def verify_module_files(module: Module, base_path: Path) -> tuple:
    """
    Verify that expected files exist for a module.

    Returns:
        Tuple of (all_exist: bool, missing_files: List[str])
    """
    expected = get_expected_files(module)
    missing = []

    # Check all file types
    for file_list in [expected.swift_files, expected.header_files,
                      expected.cpp_files, expected.test_files]:
        for f in file_list:
            full_path = base_path / f
            if not full_path.exists():
                missing.append(f)
            elif full_path.stat().st_size < 50:
                missing.append(f"{f} (file too small)")

    return (len(missing) == 0, missing)


def verify_build(base_path: Path, timeout: int = 300) -> tuple:
    """
    Run swift build and verify compilation succeeds.

    Returns:
        Tuple of (success: bool, output: str)
    """
    try:
        result = subprocess.run(
            ["swift", "build"],
            capture_output=True,
            text=True,
            timeout=timeout,
            cwd=str(base_path)
        )

        success = result.returncode == 0
        output = result.stdout + result.stderr

        return (success, output)
    except subprocess.TimeoutExpired:
        return (False, f"Build timed out after {timeout}s")
    except Exception as e:
        return (False, f"Build error: {str(e)}")


def verify_tests(module: Module, base_path: Path, timeout: int = 120) -> tuple:
    """
    Run tests for a specific module.

    Returns:
        Tuple of (success: bool, output: str)
    """
    test_filter = f"{module.name}Tests"

    try:
        result = subprocess.run(
            ["swift", "test", "--filter", test_filter],
            capture_output=True,
            text=True,
            timeout=timeout,
            cwd=str(base_path)
        )

        success = result.returncode == 0
        output = result.stdout + result.stderr

        return (success, output)
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
    """
    Comprehensive verification of module implementation.

    Args:
        module: Module to verify
        base_path: Project root directory
        build_timeout: Build timeout in seconds
        test_timeout: Test timeout in seconds
        skip_build_test: If True, only verify files exist (skip build/test)
    """
    # Phase 0 just needs to build (or files only if skipping)
    if module.name == "refactor_to_pxr_structure":
        if skip_build_test:
            return VerificationResult(
                files_ok=True,
                build_ok=True,
                tests_ok=True,
                missing_files=[],
                build_output="Skipped (--skip-build-test)",
                test_output="Skipped (Phase 0)"
            )
        build_ok, build_output = verify_build(base_path, timeout=build_timeout)
        return VerificationResult(
            files_ok=True,
            build_ok=build_ok,
            tests_ok=True,  # No tests for Phase 0
            missing_files=[],
            build_output=build_output,
            test_output="Skipped (Phase 0)"
        )

    # Step 1: Check files exist
    files_ok, missing_files = verify_module_files(module, base_path)

    if not files_ok:
        return VerificationResult(
            files_ok=False,
            build_ok=False,
            tests_ok=False,
            missing_files=missing_files,
            build_output="Skipped (files missing)",
            test_output="Skipped (files missing)"
        )

    # If skip_build_test, we're done - just files verification
    if skip_build_test:
        return VerificationResult(
            files_ok=True,
            build_ok=True,  # Assume OK since we're skipping
            tests_ok=True,  # Assume OK since we're skipping
            missing_files=[],
            build_output="Skipped (--skip-build-test)",
            test_output="Skipped (--skip-build-test)"
        )

    # Step 2: Verify build
    build_ok, build_output = verify_build(base_path, timeout=build_timeout)

    if not build_ok:
        return VerificationResult(
            files_ok=True,
            build_ok=False,
            tests_ok=False,
            missing_files=[],
            build_output=build_output,
            test_output="Skipped (build failed)"
        )

    # Step 3: Run tests
    tests_ok, test_output = verify_tests(module, base_path, timeout=test_timeout)

    return VerificationResult(
        files_ok=True,
        build_ok=True,
        tests_ok=tests_ok,
        missing_files=[],
        build_output=build_output,
        test_output=test_output
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

    return f"""
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
"""


FAILURE_LOG = Path(__file__).parent.parent / "IMPLEMENTATION_FAILURES.log"


def log_failure(module: Module, verification: VerificationResult) -> None:
    """Log detailed failure information for debugging."""
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


def recover_in_progress_modules(
    modules: List[Module],
    base_path: Path,
    build_timeout: int = 300,
    test_timeout: int = 120,
    skip_build_test: bool = False
) -> None:
    """
    Recover modules stuck in 'in_progress' state.

    Checks if files already exist and verifies build/tests.
    """
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
                print(f"[RECOVERY] {module.name} needs work:")
                if not verification.files_ok:
                    print(f"  - Missing: {verification.missing_files}")
                if not verification.build_ok:
                    print(f"  - Build fails")
                if not verification.tests_ok:
                    print(f"  - Tests fail")
                # Reset to pending so it will be retried
                module.status = "pending"


def generate_refactor_prompt() -> str:
    """Generate the prompt for Phase 0: Create all module stubs."""
    return """
TASK: Create SwiftUSD Module Stubs for All 16 Modules

You are the SwiftUSD Implementation Agent. Phase 0 creates STUB implementations
for ALL modules so the codebase compiles. Later phases replace stubs with real USD code.

WHY STUBS FIRST:
Modules have cross-dependencies (Stage references Sdf.Path, etc.). Without stubs,
modules can't compile until ALL dependencies exist. Creating stubs first lets us
build incrementally.

BEFORE YOU START:
Read IMPLEMENTATION_GUIDE.md for patterns and CLAUDE.md for project structure.

CREATE THESE SWIFT STUBS (Sources/SwiftUSD/{Module}/):

1. Tf/Token.swift - TfToken stub:
   ```swift
   public struct TfToken: Hashable, Sendable, ExpressibleByStringLiteral {
       internal var _text: String
       public init(_ string: String = "") { _text = string }
       public init(stringLiteral value: String) { _text = value }
       public var text: String { _text }
       public var isEmpty: Bool { _text.isEmpty }
       public static let empty = TfToken("")
   }
   ```

2. Gf/Vec3.swift - GfVec3f stub:
   ```swift
   public struct GfVec3f: Hashable, Sendable {
       public var x, y, z: Float
       public init(_ x: Float = 0, _ y: Float = 0, _ z: Float = 0) {
           self.x = x; self.y = y; self.z = z
       }
       public static let zero = GfVec3f(0, 0, 0)
   }
   ```

3. Gf/Matrix4.swift - GfMatrix4d stub:
   ```swift
   public struct GfMatrix4d: Hashable, Sendable {
       internal var _elements: [Double]
       public init() { _elements = [Double](repeating: 0, count: 16) }
       public static let identity = GfMatrix4d()
   }
   ```

4. Gf/Quaternion.swift - GfQuaternion stub:
   ```swift
   public struct GfQuatd: Hashable, Sendable {
       public var real: Double = 1
       public var imaginary: GfVec3d = .zero
       public static let identity = GfQuatd()
   }
   public struct GfVec3d: Hashable, Sendable {
       public var x, y, z: Double
       public init(_ x: Double = 0, _ y: Double = 0, _ z: Double = 0) {
           self.x = x; self.y = y; self.z = z
       }
       public static let zero = GfVec3d(0, 0, 0)
   }
   ```

5. Vt/Array.swift - VtArray stub:
   ```swift
   public struct VtArray<Element>: Sendable where Element: Sendable {
       internal var _elements: [Element]
       public init() { _elements = [] }
       public init(_ elements: [Element]) { _elements = elements }
       public var count: Int { _elements.count }
       public var isEmpty: Bool { _elements.isEmpty }
   }
   public typealias VtIntArray = VtArray<Int32>
   public typealias VtFloatArray = VtArray<Float>
   public typealias VtVec3fArray = VtArray<GfVec3f>
   ```

6. Sdf/Path.swift - SdfPath stub:
   ```swift
   public struct SdfPath: Hashable, Sendable, ExpressibleByStringLiteral {
       internal var _path: String
       public init(_ path: String = "") { _path = path }
       public init(stringLiteral value: String) { _path = value }
       public var string: String { _path }
       public var isEmpty: Bool { _path.isEmpty }
       public static let empty = SdfPath("")
       public static let absoluteRoot = SdfPath("/")
   }
   ```

7. Sdf/Layer.swift - SdfLayer stub:
   ```swift
   public final class SdfLayer: @unchecked Sendable {
       internal var _identifier: String
       internal init(identifier: String) { _identifier = identifier }
       public var identifier: String { _identifier }
       public static func find(identifier: String) -> SdfLayer? { nil }
       public static func createNew(identifier: String) -> SdfLayer? { nil }
   }
   ```

8. Kind/Kind.swift - Kind stub:
   ```swift
   public enum Kind: String, Sendable {
       case model, group, assembly, component, subcomponent
   }
   ```

9. Usd/Stage.swift - UsdStage stub:
   ```swift
   public final class UsdStage: @unchecked Sendable {
       internal init() {}
       public static func createNew() -> UsdStage? { nil }
       public static func open(_ path: String) -> UsdStage? { nil }
       public func prim(at path: SdfPath) -> UsdPrim? { nil }
       public func definePrim(at path: SdfPath, type: TfToken = .empty) -> UsdPrim? { nil }
       public var pseudoRoot: UsdPrim? { nil }
   }
   ```

10. Usd/Prim.swift - UsdPrim stub:
    ```swift
    public struct UsdPrim: Sendable {
        internal var _path: SdfPath
        internal init(path: SdfPath = .empty) { _path = path }
        public var path: SdfPath { _path }
        public var isValid: Bool { !_path.isEmpty }
        public var name: TfToken { TfToken(_path.string) }
        public var typeName: TfToken { .empty }
        public func attribute(_ name: TfToken) -> UsdAttribute? { nil }
    }
    ```

11. Usd/Attribute.swift - UsdAttribute stub:
    ```swift
    public struct UsdAttribute: Sendable {
        internal var _name: TfToken
        internal init(name: TfToken = .empty) { _name = name }
        public var name: TfToken { _name }
        public var isValid: Bool { !_name.isEmpty }
    }
    public struct UsdTimeCode: Hashable, Sendable {
        public var value: Double
        public init(_ value: Double = 0) { self.value = value }
        public static let `default` = UsdTimeCode(0)
    }
    ```

12. UsdGeom/Mesh.swift, Xform.swift, Camera.swift - Schema stubs:
    ```swift
    public struct UsdGeomMesh: Sendable {
        public var prim: UsdPrim
        public init(_ prim: UsdPrim) { self.prim = prim }
        public var isValid: Bool { prim.isValid }
    }
    public struct UsdGeomXform: Sendable {
        public var prim: UsdPrim
        public init(_ prim: UsdPrim) { self.prim = prim }
    }
    public struct UsdGeomCamera: Sendable {
        public var prim: UsdPrim
        public init(_ prim: UsdPrim) { self.prim = prim }
    }
    ```

13. UsdShade/Material.swift, Shader.swift - Shading stubs:
    ```swift
    public struct UsdShadeMaterial: Sendable {
        public var prim: UsdPrim
        public init(_ prim: UsdPrim) { self.prim = prim }
    }
    public struct UsdShadeShader: Sendable {
        public var prim: UsdPrim
        public init(_ prim: UsdPrim) { self.prim = prim }
    }
    ```

14. UsdLux/Light.swift - Lighting stubs:
    ```swift
    public struct UsdLuxDistantLight: Sendable {
        public var prim: UsdPrim
        public init(_ prim: UsdPrim) { self.prim = prim }
    }
    public struct UsdLuxDomeLight: Sendable {
        public var prim: UsdPrim
        public init(_ prim: UsdPrim) { self.prim = prim }
    }
    ```

15. UsdSkel/Skeleton.swift - Skeleton stubs:
    ```swift
    public struct UsdSkelSkeleton: Sendable {
        public var prim: UsdPrim
        public init(_ prim: UsdPrim) { self.prim = prim }
    }
    public struct UsdSkelRoot: Sendable {
        public var prim: UsdPrim
        public init(_ prim: UsdPrim) { self.prim = prim }
    }
    ```

16. UsdVol/Volume.swift - Volume stub:
    ```swift
    public struct UsdVolVolume: Sendable {
        public var prim: UsdPrim
        public init(_ prim: UsdPrim) { self.prim = prim }
    }
    ```

17. UsdProc/Procedural.swift - Procedural stub:
    ```swift
    public struct UsdProcGenerativeProcedural: Sendable {
        public var prim: UsdPrim
        public init(_ prim: UsdPrim) { self.prim = prim }
    }
    ```

18. UsdUtils/StageCache.swift - Utility stub:
    ```swift
    public final class UsdUtilsStageCache: @unchecked Sendable {
        public static let shared = UsdUtilsStageCache()
        private init() {}
        public func find(id: Int) -> UsdStage? { nil }
    }
    ```

19. Ar/Resolver.swift - Asset resolver stub:
    ```swift
    public struct ArResolverContext: Sendable {
        public init() {}
    }
    ```

ALSO CREATE C LAYER STUBS (Sources/OpenUSDInterop/):
- Ensure module.modulemap includes all headers
- Create minimal .h/.cpp files that compile but don't call real USD yet

VERIFICATION:
1. Run: swift build
2. All modules must compile without errors
3. Tests can be empty but must exist

When complete, output: IMPLEMENTATION_COMPLETE
If blocked, output: IMPLEMENTATION_BLOCKED: <reason>
"""


def generate_module_prompt(module: Module) -> str:
    """Generate the Claude Code prompt for implementing a specific module."""

    if module.name == "refactor_to_pxr_structure":
        return generate_refactor_prompt()

    pxr_path = get_pxr_path(module)
    include_dir, src_dir = get_interop_paths(module)
    pxr_name = module.name[0].lower() + module.name[1:]

    return f"""
TASK: Implement SwiftUSD binding for {pxr_path}

You are the SwiftUSD Implementation Agent. Your task is to implement the
{module.name} module with FULL USD integration (actual pxr:: C++ calls).

BEFORE YOU START:
Read the implementation guide: IMPLEMENTATION_GUIDE.md
This contains critical patterns for memory management, error handling, and batch APIs.

MODULE: {module.name}
DESCRIPTION: {module.description}
DEPENDENCIES: {', '.join(module.dependencies) if module.dependencies else 'None'}

ARCHITECTURE (3-layer pattern):

Layer 1 - C Header:
  Location: {include_dir}/{pxr_name}.h
  Contains: extern "C" declarations, opaque handles, C-compatible types

Layer 2 - C++ Implementation:
  Location: {src_dir}/{pxr_name}.cpp
  Contains: #include <{pxr_path}/...>, actual pxr:: calls, try/catch

Layer 3 - Swift Wrapper:
  Location: Sources/SwiftUSD/{module.name}/
  Contains: Idiomatic Swift API, OpaquePointer wrapping, Sendable conformance

USD REFERENCE: OpenUSD/{pxr_path}/

IMPLEMENTATION STEPS:

1. EXPLORE:
   - Read OpenUSD/{pxr_path}/*.h to understand the C++ API
   - Read IMPLEMENTATION_GUIDE.md for patterns
   - Review existing SwiftUSD code for consistency

2. IMPLEMENT C LAYER:
   - Create {include_dir}/{pxr_name}.h with:
     * Opaque handle: typedef struct {module.name}Opaque* {module.name}Ref;
     * Lifecycle: {module.name}_Create, {module.name}_Retain, {module.name}_Release
     * Operations with USD_INTEROP_API macro

   - Create {src_dir}/{pxr_name}.cpp with:
     * #include <{pxr_path}/...>
     * Wrapper struct with std::atomic<int> refCount
     * All pxr:: calls wrapped in try/catch
     * Return UsdResult for error handling

3. IMPLEMENT SWIFT LAYER:
   - Create Sources/SwiftUSD/{module.name}/{module.name}.swift with:
     * Class wrapping OpaquePointer
     * deinit calls _Release
     * Throws-based error handling
     * Sendable conformance where safe

4. UPDATE MODULE MAP:
   - Add header to Sources/OpenUSDInterop/include/module.modulemap

5. ADD TESTS:
   - Create Tests/SwiftUSDTests/{module.name}Tests.swift

6. VERIFY:
   - swift build
   - swift test

CRITICAL PATTERNS (from IMPLEMENTATION_GUIDE.md):
- Batch over iteration: return arrays, not per-element accessors
- Count then fill: size_t _GetCount() then size_t _Get(buffer, maxCount)
- Catch all exceptions: never let C++ exceptions cross to Swift
- Free what you allocate: pair _Create with _Release, strings with _FreeString

When complete, output: IMPLEMENTATION_COMPLETE
If blocked, output: IMPLEMENTATION_BLOCKED: <reason>
"""


def run_claude(prompt: str, cwd: Path, timeout: Optional[int] = None, verbose: bool = False, stream: bool = True) -> str:
    """Run Claude Code with the given prompt.

    Args:
        prompt: The prompt to send to Claude
        cwd: Working directory for Claude (should be project root)
        timeout: Timeout in seconds (None = no timeout, run until finished)
        verbose: Print command being run
        stream: If True, stream output in real-time (default: True)

    Returns:
        Output string from Claude (verification is done separately)
    """
    cmd = [
        "claude",
        "--print",
        "--dangerously-skip-permissions",
        "-p", prompt
    ]

    if verbose:
        print(f"Running: {' '.join(cmd[:3])} -p '<prompt>'")
        print(f"Working directory: {cwd}")

    try:
        if stream:
            # Stream output in real-time using Popen
            process = subprocess.Popen(
                cmd,
                stdout=subprocess.PIPE,
                stderr=subprocess.STDOUT,
                text=True,
                bufsize=1,  # Line buffered
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

                    # Print line in real-time
                    print(line, end='', flush=True)
                    output_lines.append(line)

                    # Check timeout only if specified
                    if timeout and time.time() - start_time > timeout:
                        process.kill()
                        return f"TIMEOUT: Claude Code exceeded {timeout}s limit"

                process.wait()  # Wait for process to finish (no timeout)

            except Exception as e:
                process.kill()
                return f"ERROR during execution: {str(e)}"

            print("-" * 40 + "\n")

            return ''.join(output_lines)

        else:
            # Non-streaming mode (original behavior)
            result = subprocess.run(
                cmd,
                capture_output=True,
                text=True,
                timeout=timeout,  # None = no timeout
                cwd=str(cwd)
            )

            return result.stdout + result.stderr

    except subprocess.TimeoutExpired:
        return f"TIMEOUT: Claude Code exceeded {timeout}s limit"
    except FileNotFoundError:
        return "ERROR: 'claude' command not found. Is Claude Code CLI installed?"
    except Exception as e:
        return f"ERROR: {str(e)}"


def list_modules(modules: List[Module]):
    """Print all modules grouped by phase."""
    print("\nSwiftUSD Implementation Modules")
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
            "failed": "!"
        }.get(m.status, " ")

        deps = f" (deps: {', '.join(m.dependencies)})" if m.dependencies else ""
        category = m.category or "(top-level)"
        print(f"  [{status_icon}] {m.name:<20} {category:<15}{deps}")


def main():
    parser = argparse.ArgumentParser(
        description="SwiftUSD Autonomous Implementation Agent",
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
Examples:
  python Scripts/implement_swiftusd.py              # Run all modules
  python Scripts/implement_swiftusd.py --start-phase 2   # Start from Phase 2
  python Scripts/implement_swiftusd.py --module Arch     # Single module only
  python Scripts/implement_swiftusd.py --dry-run        # Preview prompts
  python Scripts/implement_swiftusd.py --list           # List all modules
        """
    )
    parser.add_argument("--start-phase", type=int, default=0,
                        help="Start from phase N (0-7)")
    parser.add_argument("--module", type=str,
                        help="Implement specific module only")
    parser.add_argument("--dry-run", action="store_true",
                        help="Print prompts without running Claude")
    parser.add_argument("--list", action="store_true",
                        help="List all modules and their status")
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
                        help="Stop if a dependency fails (default: continue anyway)")
    parser.add_argument("--skip-build-test", action="store_true",
                        help="Only verify files exist, skip build/test (useful until all stubs exist)")
    args = parser.parse_args()

    # Always use project root (where this script lives), not cwd
    # This ensures paths work regardless of where user runs the script from
    base_path = Path(__file__).parent.parent.resolve()

    # Load existing progress
    progress = load_progress()
    for m in MODULES:
        m.status = progress.get(m.name, "pending")

    if args.list:
        list_modules(MODULES)
        return

    # Recover any stuck "in_progress" modules before starting
    print("Checking for stuck modules...")
    recover_in_progress_modules(MODULES, base_path, args.build_timeout, args.test_timeout, args.skip_build_test)
    save_progress(MODULES)

    # Filter to specific module if requested
    if args.module:
        target = next((m for m in MODULES if m.name == args.module), None)
        if not target:
            print(f"Unknown module: {args.module}")
            print("Use --list to see all available modules")
            sys.exit(1)
        modules_to_process = [target]
    else:
        modules_to_process = [m for m in MODULES if m.phase.value >= args.start_phase]

    # Calculate stats
    pending = [m for m in modules_to_process if m.status == "pending"]

    print("=" * 60)
    print("SwiftUSD Autonomous Implementation Agent")
    print("=" * 60)
    print(f"Total modules: {len(MODULES)}")
    print(f"Modules to process: {len(pending)}")
    print(f"Max retries per module: {args.max_retries}")
    if args.dry_run:
        print("MODE: Dry run (prompts only, no execution)")
    print()

    iteration = 0
    start_time = time.time()

    while True:
        # By default, continue even if dependencies failed (use --strict-deps to block)
        module = get_next_module(modules_to_process, force_continue=not args.strict_deps)
        if not module:
            elapsed = time.time() - start_time
            print(f"\n{'=' * 60}")

            # Check why we stopped
            failed = sum(1 for m in modules_to_process if m.status == "failed")
            pending = sum(1 for m in modules_to_process if m.status == "pending")
            completed = sum(1 for m in modules_to_process if m.status == "completed")

            if pending > 0 and failed > 0 and args.strict_deps:
                print(f"Stopped: {pending} modules blocked by failed dependencies")
                print("Remove --strict-deps to continue past failures")
            elif failed > 0:
                print(f"Finished with failures ({elapsed:.1f}s total)")
            else:
                print(f"All modules completed! ({elapsed:.1f}s total)")
            break

        iteration += 1
        print(f"\n{'=' * 60}")
        print(f"[{iteration}] Implementing: {module.name}")
        print(f"    Phase: {module.phase.value} ({module.phase.name})")
        print(f"    Category: {module.category}")
        print(f"    Dependencies: {module.dependencies or 'None'}")
        print("=" * 60)

        module.status = "in_progress"
        save_progress(MODULES)

        if args.dry_run:
            prompt = generate_module_prompt(module)
            print("\n--- PROMPT PREVIEW ---")
            print(prompt[:2000])
            if len(prompt) > 2000:
                print(f"... ({len(prompt) - 2000} more characters)")
            print("--- END PREVIEW ---\n")
            module.status = "completed"
            module.completed_at = datetime.now().strftime("%Y-%m-%d %H:%M")
            save_progress(MODULES)
            continue

        # Retry loop with verification
        retry_count = 0
        verification = None
        module_start = time.time()

        while retry_count <= args.max_retries:
            if retry_count == 0:
                prompt = generate_module_prompt(module)
            else:
                print(f"\n[RETRY {retry_count}/{args.max_retries}] Attempting fix...")
                prompt = generate_fix_prompt(module, verification)

            timeout_msg = f"timeout: {args.timeout}s" if args.timeout else "no timeout"
            print(f"\nRunning Claude Code ({timeout_msg})...")

            output = run_claude(prompt, cwd=base_path, timeout=args.timeout, verbose=args.verbose)

            # Check for errors in Claude execution
            if output.startswith("ERROR:") or output.startswith("TIMEOUT:"):
                print(f"\n[ERROR] Claude execution failed: {output[:200]}")
                retry_count += 1
                continue

            # Verify the implementation
            print("\n[VERIFICATION] Checking implementation...")
            verification = verify_module_implementation(
                module, base_path, args.build_timeout, args.test_timeout, args.skip_build_test
            )

            if verification.success:
                elapsed = time.time() - module_start
                print(f"\n[OK] {module.name} completed in {elapsed:.1f}s")
                print("    - Files: OK")
                if args.skip_build_test:
                    print("    - Build: SKIPPED")
                    print("    - Tests: SKIPPED")
                else:
                    print("    - Build: OK")
                    print("    - Tests: OK")
                module.status = "completed"
                module.completed_at = datetime.now().strftime("%Y-%m-%d %H:%M")
                break
            else:
                print("[VERIFICATION] Issues found:")
                if not verification.files_ok:
                    print(f"    - Missing files: {verification.missing_files}")
                if not verification.build_ok and not args.skip_build_test:
                    print("    - Build: FAILED")
                if not verification.tests_ok and not args.skip_build_test:
                    print("    - Tests: FAILED")

            retry_count += 1

        # Check if we exhausted retries
        if not verification or not verification.success:
            elapsed = time.time() - module_start
            print(f"\n[FAIL] {module.name} failed after {args.max_retries} retries ({elapsed:.1f}s)")
            module.status = "failed"

            # Log detailed failure info
            if verification:
                log_failure(module, verification)
                print(f"    Failure details logged to: {FAILURE_LOG}")

        save_progress(MODULES)

    # Final summary
    save_progress(MODULES)
    completed = sum(1 for m in MODULES if m.status == "completed")
    failed = sum(1 for m in MODULES if m.status == "failed")

    print(f"\n{'=' * 60}")
    print("SUMMARY")
    print(f"{'=' * 60}")
    print(f"Completed: {completed}/{len(MODULES)}")
    if failed > 0:
        print(f"Failed: {failed}")
        print(f"See {FAILURE_LOG} for details")
    print(f"\nProgress saved to: {PROGRESS_FILE}")


if __name__ == "__main__":
    main()
