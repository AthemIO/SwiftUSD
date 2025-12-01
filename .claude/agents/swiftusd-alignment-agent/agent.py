#!/usr/bin/env python3
"""
SwiftUSD Alignment Agent

An autonomous agent framework for aligning SwiftUSD to any OpenUSD release version.
Uses Claude Code (via your Max subscription) as the execution engine.

The agent auto-detects the OpenUSD version from ~/dev/OpenUSD - just update that
repository to the desired version and run the agent.

Usage:
    python agent.py              # Auto-detect version from OpenUSD repo
    python agent.py --resume     # Resume from checkpoint
"""

import json
import os
import re
import subprocess
import sys
from datetime import datetime
from pathlib import Path

# Configuration - All three directories are fully accessible
DEFAULT_CONFIG = {
    "SWIFTUSD_PATH": os.path.expanduser("~/dev/SwiftUSD"),
    "OPENUSD_PATH": os.path.expanduser("~/dev/OpenUSD"),
    "METAVERSEKIT_PATH": os.path.expanduser("~/dev/MetaverseKit"),
    "WABIVERSE_REF": "LynrAI/SwiftUSD@77abfccf",
}


def detect_openusd_version(openusd_path: Path) -> str:
    """
    Auto-detect the OpenUSD version from the repository.

    OpenUSD uses version format: 0.MAJOR.MINOR (e.g., 0.25.11 = version 25.11)

    Checks multiple sources:
    1. cmake/defaults/Version.cmake (PXR_MINOR_VERSION.PXR_PATCH_VERSION for releases)
    2. CHANGELOG.md header
    3. Git tags
    """
    # Try Version.cmake first
    version_cmake = openusd_path / "cmake" / "defaults" / "Version.cmake"
    if version_cmake.exists():
        content = version_cmake.read_text()
        # OpenUSD format: PXR_MAJOR=0, PXR_MINOR=25, PXR_PATCH=11 -> version "25.11"
        minor_match = re.search(r'set\(PXR_MINOR_VERSION\s+"?(\d+)"?\)', content)
        patch_match = re.search(r'set\(PXR_PATCH_VERSION\s+"?(\d+)"?\)', content)
        if minor_match and patch_match:
            return f"{minor_match.group(1)}.{patch_match.group(1)}"

    # Try CHANGELOG.md
    changelog = openusd_path / "CHANGELOG.md"
    if changelog.exists():
        content = changelog.read_text()
        # Look for first version header like ## [25.11]
        match = re.search(r'##\s*\[(\d+\.\d+)\]', content)
        if match:
            return match.group(1)

    # Try git tags
    try:
        result = subprocess.run(
            ["git", "describe", "--tags", "--abbrev=0"],
            cwd=openusd_path,
            capture_output=True,
            text=True
        )
        if result.returncode == 0:
            tag = result.stdout.strip()
            # Parse tag like v25.11 or 25.11
            match = re.search(r'v?(\d+\.\d+)', tag)
            if match:
                return match.group(1)
    except:
        pass

    raise ValueError(
        f"Could not detect OpenUSD version from {openusd_path}. "
        "Please ensure the repository is at a valid release version."
    )


def load_progress(progress_file: Path) -> dict:
    """Load progress from file if it exists."""
    if progress_file.exists():
        with open(progress_file) as f:
            return json.load(f)
    return None


def create_alignment_prompt(
    target_version: str,
    swiftusd_path: Path,
    openusd_path: Path,
    metaversekit_path: Path,
    wabiverse_ref: str,
    progress: dict = None
) -> str:
    """Generate the prompt for Claude Code to execute the alignment."""

    progress_section = ""
    if progress:
        progress_section = f"""
## Previous Progress (Resume Mode)
The following progress was made in a previous session:
```json
{json.dumps(progress, indent=2)}
```

Continue from where you left off. Check which phases are completed and proceed with the next pending phase.
"""

    return f"""# SwiftUSD Alignment Task

You are performing an autonomous alignment of SwiftUSD to OpenUSD version {target_version}.

## Full Access Directories
You have FULL READ/WRITE access to these three directories:

1. **SwiftUSD** ({swiftusd_path})
   - Swift wrapper for OpenUSD
   - Contains: Sources/, Package.swift, Swift modules
   - This is what you're updating

2. **OpenUSD** ({openusd_path})
   - C++ reference source (version {target_version})
   - Contains: pxr/, CHANGELOG.md, build_scripts/, cmake/
   - Use as reference for what needs to change

3. **MetaverseKit** ({metaversekit_path})
   - Dependencies: TBB, MaterialX, OpenSubdiv, OpenEXR, etc.
   - Contains: Sources/, Package.swift
   - Update these when OpenUSD requires newer versions

## Wabiverse Conventions
Reference: {wabiverse_ref} (style guide for Swift wrapper patterns)
{progress_section}
## Your Mission
Execute the alignment workflow autonomously, following these phases IN ORDER:

### Phase 0: MetaverseKit Dependency Updates (FIRST)
1. Read {openusd_path}/CHANGELOG.md for dependency requirements
2. Read {openusd_path}/build_scripts/build_usd.py for exact version URLs
3. Compare versions in {metaversekit_path}/Package.swift and source headers
4. For each outdated dependency:
   - Download new source from official URL
   - Update source files in MetaverseKit/Sources/{{Dependency}}/
   - Update version info in Package.swift if needed
5. Verify MetaverseKit builds: `swift build` in {metaversekit_path}

### Phase 1: Pre-Analysis
1. Detect SOURCE_VERSION from {swiftusd_path}/Sources/PixarUSD/Pixar.swift
2. Parse {openusd_path}/CHANGELOG.md for ALL changes between SOURCE and {target_version}
3. Create change manifest with:
   - api_removals: Classes/functions/modules REMOVED in target version (MUST DELETE from SwiftUSD)
   - api_renames: Symbol name changes (old -> new) - update all references
   - module_removals: Entire modules removed (e.g., Ndr removed in 25.08)
   - module_additions: New modules to add
   - deprecated_removals: APIs that were deprecated in earlier versions and NOW REMOVED
   - signature_changes: Function parameter or return type changes
   - new_features: New APIs to expose
4. Save the manifest to {swiftusd_path}/.claude/change-manifest.json

**CRITICAL**: The manifest must capture EVERYTHING that was removed or deprecated-then-removed.
SwiftUSD must NOT contain any code that doesn't exist in OpenUSD {target_version}.

### Phase 2: Module Processing
1. Build dependency graph from {swiftusd_path}/Package.swift
2. Process modules bottom-up by dependency layer:
   - Layer 1: Arch (foundation)
   - Layer 2: Tf, Gf, Js, Trace, Work, Vt
   - Layer 3: Plug
   - ... up to UsdImaging

3. For each module, apply changes IN THIS ORDER:

   **Step A: REMOVE deprecated/removed code FIRST**
   - Delete entire module directories if module was removed (e.g., Ndr/)
   - Delete C++ files (.h, .cpp) for removed classes
   - Delete Swift wrapper files for removed APIs
   - Remove from Package.swift targets and products
   - Search and remove all imports/references to deleted items

   **Step B: Apply renames**
   - Rename symbols across all files (old -> new)
   - Update Swift typealiases
   - Update import statements

   **Step C: Update signatures**
   - Fix function parameter changes
   - Fix return type changes
   - Update Swift wrapper method signatures

   **Step D: Sync new/modified code from OpenUSD**
   - Copy updated C++ headers from {openusd_path}/pxr/
   - Copy updated C++ source files
   - Create Swift wrappers for new APIs following wabiverse conventions

   **Step E: Validate**
   - Run: `swift build --target {{Module}}`
   - Fix any remaining errors before proceeding

**REMOVAL CHECKLIST** (apply for each removed item):
- [ ] Delete the source files (.h, .cpp, .swift)
- [ ] Remove from CMakeLists.txt or Package.swift
- [ ] Remove imports in other files
- [ ] Remove usages in other files
- [ ] Remove Swift typealiases
- [ ] Remove from module exports

### Phase 3: Build Iteration
1. Run full build: `swift bundler run -c release` in {swiftusd_path}
2. Parse and classify errors
3. Fix errors referencing the change manifest and OpenUSD source
4. Repeat until clean build (max 10 iterations)

### Phase 4: Final Validation
1. Verify clean build succeeds
2. Update version in Pixar.swift to {target_version}.x
3. Save final progress to {swiftusd_path}/.claude/alignment-progress.json

## Progress Tracking
After completing each phase, update the progress file at:
{swiftusd_path}/.claude/alignment-progress.json

Use this JSON structure:
```json
{{
  "targetVersion": "{target_version}",
  "sourceVersion": "<detected>",
  "status": "in_progress|completed|failed",
  "startedAt": "<timestamp>",
  "phases": {{
    "metaversekit_update": "pending|in_progress|completed",
    "changelog_analysis": "pending|in_progress|completed",
    "module_processing": "pending|in_progress|completed",
    "build_iteration": "pending|in_progress|completed",
    "final_validation": "pending|in_progress|completed"
  }},
  "completedModules": [],
  "blockedModules": [],
  "buildIterations": 0,
  "lastError": null
}}
```

## Important Guidelines
- Update progress file after each phase completion
- Save change manifest early - reference it throughout
- Follow wabiverse Swift wrapper patterns
- If stuck on same error 3x, mark module as blocked and continue
- For MetaverseKit updates, download official source releases

## Git Commits & Push
After completing each phase, commit and push your changes:

1. **After Phase 0 (MetaverseKit)**:
   ```bash
   cd {metaversekit_path}
   git add -A
   git commit -m "chore: update dependencies for OpenUSD {target_version}

   - Updated TBB, MaterialX, OpenSubdiv, etc. to required versions
   - Verified build passes

   🤖 Generated with Claude Code (SwiftUSD Alignment Agent)"
   git push
   ```

2. **After Phase 1 (Pre-Analysis)**:
   ```bash
   cd {swiftusd_path}
   git add .claude/
   git commit -m "chore: add change manifest for {target_version} alignment

   - Parsed CHANGELOG for breaking changes
   - Created change manifest with API removals, renames, additions

   🤖 Generated with Claude Code (SwiftUSD Alignment Agent)"
   git push
   ```

3. **After Phase 2 (Module Processing)** - Commit after each module or batch:
   ```bash
   cd {swiftusd_path}
   git add -A
   git commit -m "feat: align {{Module}} to OpenUSD {target_version}

   - Applied API changes from change manifest
   - Synced C++ code from OpenUSD
   - Updated Swift wrappers

   🤖 Generated with Claude Code (SwiftUSD Alignment Agent)"
   git push
   ```

4. **After Phase 3 (Build Fixes)**:
   ```bash
   cd {swiftusd_path}
   git add -A
   git commit -m "fix: resolve build errors for OpenUSD {target_version}

   - Fixed compilation errors from alignment
   - Build iteration {{N}}

   🤖 Generated with Claude Code (SwiftUSD Alignment Agent)"
   git push
   ```

5. **After Phase 4 (Final)**:
   ```bash
   cd {swiftusd_path}
   git add -A
   git commit -m "chore: complete alignment to OpenUSD {target_version}

   - Updated version in Pixar.swift
   - All modules aligned and building

   🤖 Generated with Claude Code (SwiftUSD Alignment Agent)"
   git push
   ```

**Important Git Guidelines:**
- Always use descriptive commit messages
- Push after each significant milestone (phase completion, module batch)
- If push fails due to remote changes, pull --rebase first then push
- Include the robot emoji and "Generated with Claude Code" in all commits

## Full Alignment Requirements

**SwiftUSD MUST be a 1:1 match with OpenUSD {target_version}.**

This means:
1. **NO deprecated APIs** - If it's removed in OpenUSD {target_version}, DELETE it from SwiftUSD
2. **NO legacy code** - Remove all backward-compatibility shims
3. **NO orphaned files** - Delete files that no longer have OpenUSD counterparts
4. **ALL new APIs** - Add everything new in {target_version}

### Known Removals to Handle (check CHANGELOG for complete list):

**Modules potentially removed:**
- Ndr (removed 25.08) → functionality moved to Sdr

**Classes/Files potentially removed:**
- UsdCrateInfo → SdfCrateInfo
- UsdUsdFileFormat → SdfUsdFileFormat
- UsdUsdcFileFormat → SdfUsdcFileFormat
- UsdUsdzFileFormat → SdfUsdzFileFormat
- UsdZipFile → SdfZipFile
- TfTemplateString (removed 25.11)
- TraceEventId (removed 25.11)

**Macros renamed:**
- TF_THROW → PXR_TF_THROW

**VERIFY**: After processing, grep SwiftUSD for ANY reference to removed items.
If found, DELETE them. SwiftUSD should compile without ANY deprecated code.

## BEGIN
Start by reading the current progress file (if it exists) and continue from where you left off.
If no progress file exists, start fresh with Phase 0.

Work autonomously until the alignment is complete or you encounter an unrecoverable error.
"""


def run_claude_code(prompt: str, working_dir: Path) -> int:
    """Run Claude Code with the given prompt."""

    # Create a temporary file with the prompt for complex multi-line input
    prompt_file = working_dir / ".claude" / "alignment-prompt.md"
    prompt_file.parent.mkdir(parents=True, exist_ok=True)
    prompt_file.write_text(prompt)

    print(f"\n{'='*70}")
    print("Starting Claude Code with alignment task...")
    print(f"{'='*70}")
    print(f"Working directory: {working_dir}")
    print(f"Prompt saved to: {prompt_file}")
    print(f"{'='*70}\n")

    # Run Claude Code with the prompt
    # Using --print to show output, --dangerously-skip-permissions to allow file operations
    cmd = [
        "claude",
        "--print",
        "--dangerously-skip-permissions",
        prompt
    ]

    try:
        # Run interactively so user can see progress
        result = subprocess.run(
            cmd,
            cwd=working_dir,
            text=True
        )
        return result.returncode
    except FileNotFoundError:
        print("Error: 'claude' command not found.")
        print("Please ensure Claude Code is installed and in your PATH.")
        print("Install with: npm install -g @anthropic-ai/claude-code")
        return 1
    except KeyboardInterrupt:
        print("\n\nAlignment interrupted by user.")
        print("Run with --resume to continue from checkpoint.")
        return 130


def main():
    """Main entry point for the alignment agent."""
    import argparse

    parser = argparse.ArgumentParser(
        description="SwiftUSD Alignment Agent - Automatically align SwiftUSD to OpenUSD",
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
Examples:
  python agent.py                    # Auto-detect OpenUSD version and align
  python agent.py --resume           # Resume interrupted alignment

The agent auto-detects the OpenUSD version from ~/dev/OpenUSD.
Just update that repository to your target version and run the agent.

Uses your Claude Code Max subscription - no API key needed!
        """
    )
    parser.add_argument(
        "--swiftusd-path",
        default=DEFAULT_CONFIG["SWIFTUSD_PATH"],
        help="Path to SwiftUSD (default: ~/dev/SwiftUSD)"
    )
    parser.add_argument(
        "--openusd-path",
        default=DEFAULT_CONFIG["OPENUSD_PATH"],
        help="Path to OpenUSD reference (default: ~/dev/OpenUSD)"
    )
    parser.add_argument(
        "--metaversekit-path",
        default=DEFAULT_CONFIG["METAVERSEKIT_PATH"],
        help="Path to MetaverseKit (default: ~/dev/MetaverseKit)"
    )
    parser.add_argument(
        "--resume",
        action="store_true",
        help="Resume from previous checkpoint"
    )

    args = parser.parse_args()

    # Expand and verify paths
    swiftusd_path = Path(args.swiftusd_path).expanduser()
    openusd_path = Path(args.openusd_path).expanduser()
    metaversekit_path = Path(args.metaversekit_path).expanduser()

    paths_to_check = [
        ("SwiftUSD", swiftusd_path),
        ("OpenUSD", openusd_path),
        ("MetaverseKit", metaversekit_path),
    ]

    for name, path in paths_to_check:
        if not path.exists():
            print(f"Error: {name} path does not exist: {path}")
            sys.exit(1)

    # Auto-detect OpenUSD version
    print(f"Detecting OpenUSD version from {openusd_path}...")
    try:
        target_version = detect_openusd_version(openusd_path)
    except ValueError as e:
        print(f"Error: {e}")
        sys.exit(1)

    print(f"Detected OpenUSD version: {target_version}")

    # Load previous progress if resuming
    progress_file = swiftusd_path / ".claude" / "alignment-progress.json"
    progress = None
    if args.resume:
        progress = load_progress(progress_file)
        if progress:
            if progress.get("targetVersion") == target_version:
                print(f"Resuming previous alignment to {target_version}")
            else:
                print(f"Previous alignment was for {progress.get('targetVersion')}, starting fresh for {target_version}")
                progress = None
        else:
            print("No previous progress found, starting fresh")

    # Generate the prompt
    prompt = create_alignment_prompt(
        target_version=target_version,
        swiftusd_path=swiftusd_path,
        openusd_path=openusd_path,
        metaversekit_path=metaversekit_path,
        wabiverse_ref=DEFAULT_CONFIG["WABIVERSE_REF"],
        progress=progress
    )

    # Run Claude Code
    exit_code = run_claude_code(prompt, swiftusd_path)

    # Check final status
    final_progress = load_progress(progress_file)
    if final_progress and final_progress.get("status") == "completed":
        print(f"\n{'='*70}")
        print("ALIGNMENT COMPLETED SUCCESSFULLY!")
        print(f"{'='*70}")
        print(f"SwiftUSD is now aligned to OpenUSD {target_version}")
    else:
        print(f"\n{'='*70}")
        print("ALIGNMENT INCOMPLETE")
        print(f"{'='*70}")
        if final_progress:
            print(f"Status: {final_progress.get('status', 'unknown')}")
            print(f"Completed phases: {[k for k, v in final_progress.get('phases', {}).items() if v == 'completed']}")
        print("\nRun with --resume to continue")

    sys.exit(exit_code)


if __name__ == "__main__":
    main()
