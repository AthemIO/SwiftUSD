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
import threading
import time
from datetime import datetime
from pathlib import Path

# ANSI color codes for terminal output
class Colors:
    HEADER = '\033[95m'
    BLUE = '\033[94m'
    CYAN = '\033[96m'
    GREEN = '\033[92m'
    YELLOW = '\033[93m'
    RED = '\033[91m'
    BOLD = '\033[1m'
    DIM = '\033[2m'
    RESET = '\033[0m'

def print_header(msg: str):
    """Print a prominent header message."""
    print(f"\n{Colors.BOLD}{Colors.HEADER}{'='*70}{Colors.RESET}")
    print(f"{Colors.BOLD}{Colors.HEADER}{msg}{Colors.RESET}")
    print(f"{Colors.BOLD}{Colors.HEADER}{'='*70}{Colors.RESET}\n")

def print_phase(phase: str, status: str = "starting"):
    """Print phase status with timestamp."""
    timestamp = datetime.now().strftime("%H:%M:%S")
    if status == "starting":
        icon = "🚀"
        color = Colors.CYAN
    elif status == "completed":
        icon = "✅"
        color = Colors.GREEN
    elif status == "failed":
        icon = "❌"
        color = Colors.RED
    else:
        icon = "⏳"
        color = Colors.YELLOW
    print(f"{Colors.DIM}[{timestamp}]{Colors.RESET} {icon} {color}{phase}{Colors.RESET}")

def print_info(msg: str):
    """Print an info message."""
    timestamp = datetime.now().strftime("%H:%M:%S")
    print(f"{Colors.DIM}[{timestamp}]{Colors.RESET} {Colors.BLUE}ℹ {msg}{Colors.RESET}")

def print_progress(msg: str):
    """Print a progress message."""
    timestamp = datetime.now().strftime("%H:%M:%S")
    print(f"{Colors.DIM}[{timestamp}]{Colors.RESET} {Colors.DIM}  → {msg}{Colors.RESET}")

# Configuration - All three directories are fully accessible
DEFAULT_CONFIG = {
    "SWIFTUSD_PATH": os.path.expanduser("~/dev/SwiftUSD"),
    "OPENUSD_PATH": os.path.expanduser("~/dev/OpenUSD"),
    "METAVERSEKIT_PATH": os.path.expanduser("~/dev/MetaverseKit"),
    "WABIVERSE_REF": "LynrAI/SwiftUSD@77abfccf",
}

# Ultrathink mode prefix - prepended to prompt for extended thinking
ULTRATHINK_PREFIX = """## ULTRATHINK MODE ENABLED

You are in extended thinking mode. For each major decision or complex problem:

1. **Analyze thoroughly** - Before acting, examine the problem from multiple angles
2. **Validate assumptions** - Check that your understanding matches the actual codebase
3. **Consider alternatives** - Think through 2-3 approaches before choosing one
4. **Verify changes** - After making changes, verify they compile/work before moving on
5. **Break down blockers** - If stuck on the same issue 3+ times, step back and reconsider:
   - Is the approach fundamentally wrong?
   - Is there missing context from another file?
   - Should this be skipped and revisited later?

Take your time. Quality and correctness matter more than speed.

---

"""

# Read-before-modify protocol - ensures OpenUSD verification before any SwiftUSD changes
READ_BEFORE_MODIFY_PROTOCOL = """## READ-BEFORE-MODIFY PROTOCOL (MANDATORY)

**CRITICAL RULE**: Before modifying ANY C++ file in SwiftUSD, you MUST first read the corresponding file in OpenUSD at `{openusd_path}`.

### Required Workflow
For EVERY C++ source file modification:

1. **READ OpenUSD FIRST**
   ```
   Before editing: Sources/Tf/token.cpp
   First read:     {openusd_path}/pxr/base/tf/token.cpp
   ```

2. **COMPARE before changing**
   - What does OpenUSD have?
   - What does SwiftUSD currently have?
   - What specific lines need to change?

3. **APPLY only verified changes**
   - Only add code that exists in OpenUSD
   - Only remove code that's removed in OpenUSD
   - Match function signatures exactly

### Module Path Mapping
| SwiftUSD Sources/ | OpenUSD pxr/ |
|-------------------|--------------|
| Arch/ | base/arch/ |
| Tf/ | base/tf/ |
| Gf/ | base/gf/ |
| Vt/ | base/vt/ |
| Work/ | base/work/ |
| Plug/ | base/plug/ |
| Trace/ | base/trace/ |
| Js/ | base/js/ |
| Ts/ | base/ts/ |
| Ar/ | usd/ar/ |
| Sdf/ | usd/sdf/ |
| Pcp/ | usd/pcp/ |
| Usd/ | usd/usd/ |
| Ndr/ | usd/ndr/ |
| Sdr/ | usd/sdr/ |
| UsdGeom/ | usd/usdGeom/ |
| UsdShade/ | usd/usdShade/ |
| UsdLux/ | usd/usdLux/ |
| UsdSkel/ | usd/usdSkel/ |
| UsdVol/ | usd/usdVol/ |
| UsdMedia/ | usd/usdMedia/ |
| UsdRender/ | usd/usdRender/ |
| UsdPhysics/ | usd/usdPhysics/ |
| UsdProc/ | usd/usdProc/ |
| UsdUI/ | usd/usdUI/ |
| UsdUtils/ | usd/usdUtils/ |
| UsdMtlx/ | usd/usdMtlx/ |
| Hd/ | imaging/hd/ |
| HdSt/ | imaging/hdSt/ |
| Hgi/ | imaging/hgi/ |
| HgiMetal/ | imaging/hgiMetal/ |
| HgiGL/ | imaging/hgiGL/ |
| Hdx/ | imaging/hdx/ |
| Glf/ | imaging/glf/ |
| Hio/ | imaging/hio/ |
| CameraUtil/ | imaging/cameraUtil/ |
| GeomUtil/ | imaging/geomUtil/ |
| PxOsd/ | imaging/pxOsd/ |
| UsdImaging/ | usdImaging/usdImaging/ |
| UsdImagingGL/ | usdImaging/usdImagingGL/ |

### Verification Checklist (apply to EACH C++ file edit)
- [ ] Read the OpenUSD version of this file FIRST
- [ ] Confirmed the change aligns with OpenUSD

### EXCEPTIONS (files that DON'T need OpenUSD verification)
- Swift wrapper files in `Sources/PixarUSD/` (Swift-only)
- `Package.swift` (Swift package manager)
- Files in `.claude/` (agent configuration)
- Files explicitly marked as SwiftUSD-specific in CLAUDE.md

### VIOLATION = STOP
If you're about to modify a C++ file and haven't read the OpenUSD counterpart:
**STOP. Read the OpenUSD file first. Then proceed.**

---

"""


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

    # Format the read-before-modify protocol with OpenUSD path
    read_before_modify = READ_BEFORE_MODIFY_PROTOCOL.format(
        openusd_path=openusd_path
    )

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

{read_before_modify}
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

   **Step 0: READ OPENUSD FIRST (REQUIRED)**
   - For EACH C++ file you plan to modify, FIRST read the OpenUSD counterpart
   - Example: Before editing `Sources/Tf/token.cpp`, first read `{openusd_path}/pxr/base/tf/token.cpp`
   - This is NOT optional - skip this step and you risk introducing drift

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


class ProgressMonitor:
    """Monitor progress file and print updates."""

    def __init__(self, progress_file: Path):
        self.progress_file = progress_file
        self.running = False
        self.thread = None
        self.last_state = None

    def start(self):
        """Start monitoring in background thread."""
        self.running = True
        self.thread = threading.Thread(target=self._monitor_loop, daemon=True)
        self.thread.start()

    def stop(self):
        """Stop monitoring."""
        self.running = False
        if self.thread:
            self.thread.join(timeout=1)

    def _monitor_loop(self):
        """Monitor loop that checks progress file periodically."""
        while self.running:
            try:
                if self.progress_file.exists():
                    with open(self.progress_file) as f:
                        current = json.load(f)

                    # Check for phase changes
                    if self.last_state is None:
                        self.last_state = current
                        self._print_initial_state(current)
                    else:
                        self._print_changes(self.last_state, current)
                        self.last_state = current
            except (json.JSONDecodeError, FileNotFoundError):
                pass
            time.sleep(2)  # Check every 2 seconds

    def _print_initial_state(self, state: dict):
        """Print initial state when first detected."""
        print_info(f"Target version: {state.get('targetVersion', 'unknown')}")
        print_info(f"Source version: {state.get('sourceVersion', 'detecting...')}")
        phases = state.get('phases', {})
        for phase, status in phases.items():
            if status == 'in_progress':
                print_phase(f"Phase: {phase.replace('_', ' ').title()}", "in_progress")
            elif status == 'completed':
                print_phase(f"Phase: {phase.replace('_', ' ').title()}", "completed")

    def _print_changes(self, old: dict, new: dict):
        """Print any changes between old and new state."""
        old_phases = old.get('phases', {})
        new_phases = new.get('phases', {})

        for phase, status in new_phases.items():
            old_status = old_phases.get(phase, 'pending')
            if status != old_status:
                phase_name = phase.replace('_', ' ').title()
                if status == 'in_progress':
                    print_phase(f"Phase: {phase_name}", "starting")
                elif status == 'completed':
                    print_phase(f"Phase: {phase_name}", "completed")
                elif status == 'failed':
                    print_phase(f"Phase: {phase_name}", "failed")

        # Check for completed modules
        old_modules = set(old.get('completedModules', []))
        new_modules = set(new.get('completedModules', []))
        for module in new_modules - old_modules:
            print_progress(f"Completed module: {module}")

        # Check for blocked modules
        old_blocked = set(old.get('blockedModules', []))
        new_blocked = set(new.get('blockedModules', []))
        for module in new_blocked - old_blocked:
            print(f"{Colors.YELLOW}⚠️  Module blocked: {module}{Colors.RESET}")

        # Check for build iterations
        old_iter = old.get('buildIterations', 0)
        new_iter = new.get('buildIterations', 0)
        if new_iter > old_iter:
            print_progress(f"Build iteration {new_iter}")

        # Check for errors
        if new.get('lastError') and new.get('lastError') != old.get('lastError'):
            print(f"{Colors.RED}⚠️  Error: {new['lastError'][:100]}...{Colors.RESET}")


def run_claude_code(prompt: str, working_dir: Path, verbose: bool = False) -> int:
    """Run Claude Code with the given prompt and real-time output."""

    # Create a temporary file with the prompt for complex multi-line input
    prompt_file = working_dir / ".claude" / "alignment-prompt.md"
    prompt_file.parent.mkdir(parents=True, exist_ok=True)
    prompt_file.write_text(prompt)

    print_header("SwiftUSD Alignment Agent")
    print_info(f"Working directory: {working_dir}")
    print_info(f"Prompt saved to: {prompt_file}")

    # Start progress monitor
    progress_file = working_dir / ".claude" / "alignment-progress.json"
    monitor = ProgressMonitor(progress_file)
    monitor.start()

    print_phase("Launching Claude Code", "starting")
    print()

    # Run Claude Code interactively (no --print flag) so output streams in real-time
    # --dangerously-skip-permissions allows file operations without prompts
    cmd = [
        "claude",
        "--dangerously-skip-permissions",
    ]

    if verbose:
        cmd.append("--verbose")

    # Pass prompt as final argument
    cmd.append(prompt)

    try:
        # Run with inherited stdio for real-time output visibility
        result = subprocess.run(
            cmd,
            cwd=working_dir,
            text=True,
            # Let Claude's output stream directly to terminal
        )
        monitor.stop()
        return result.returncode
    except FileNotFoundError:
        monitor.stop()
        print(f"{Colors.RED}Error: 'claude' command not found.{Colors.RESET}")
        print("Please ensure Claude Code is installed and in your PATH.")
        print("Install with: npm install -g @anthropic-ai/claude-code")
        return 1
    except KeyboardInterrupt:
        monitor.stop()
        print(f"\n\n{Colors.YELLOW}Alignment interrupted by user.{Colors.RESET}")
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
    parser.add_argument(
        "--no-ultrathink",
        dest="ultrathink",
        action="store_false",
        default=True,
        help="Disable extended thinking mode (enabled by default)"
    )
    parser.add_argument(
        "--verbose", "-v",
        action="store_true",
        help="Enable verbose output from Claude Code"
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
            print(f"{Colors.RED}Error: {name} path does not exist: {path}{Colors.RESET}")
            sys.exit(1)

    # Auto-detect OpenUSD version
    print_info(f"Detecting OpenUSD version from {openusd_path}...")
    try:
        target_version = detect_openusd_version(openusd_path)
    except ValueError as e:
        print(f"{Colors.RED}Error: {e}{Colors.RESET}")
        sys.exit(1)

    print_phase(f"Detected OpenUSD version: {target_version}", "completed")

    # Load previous progress if resuming
    progress_file = swiftusd_path / ".claude" / "alignment-progress.json"
    progress = None
    if args.resume:
        progress = load_progress(progress_file)
        if progress:
            if progress.get("targetVersion") == target_version:
                print_info(f"Resuming previous alignment to {target_version}")
            else:
                print_info(f"Previous alignment was for {progress.get('targetVersion')}, starting fresh for {target_version}")
                progress = None
        else:
            print_info("No previous progress found, starting fresh")

    # Generate the prompt
    prompt = create_alignment_prompt(
        target_version=target_version,
        swiftusd_path=swiftusd_path,
        openusd_path=openusd_path,
        metaversekit_path=metaversekit_path,
        wabiverse_ref=DEFAULT_CONFIG["WABIVERSE_REF"],
        progress=progress
    )

    # Apply ultrathink prefix (enabled by default)
    if args.ultrathink:
        print_info(f"{Colors.BOLD}ULTRATHINK mode: ON{Colors.RESET} (use --no-ultrathink to disable)")
        prompt = ULTRATHINK_PREFIX + prompt
    else:
        print_info("ULTRATHINK mode: OFF")

    if args.verbose:
        print_info("Verbose mode: ON")

    # Run Claude Code
    exit_code = run_claude_code(prompt, swiftusd_path, verbose=args.verbose)

    # Check final status
    final_progress = load_progress(progress_file)
    if final_progress and final_progress.get("status") == "completed":
        print_header("ALIGNMENT COMPLETED SUCCESSFULLY!")
        print(f"{Colors.GREEN}SwiftUSD is now aligned to OpenUSD {target_version}{Colors.RESET}")
    else:
        print_header("ALIGNMENT INCOMPLETE")
        if final_progress:
            status = final_progress.get('status', 'unknown')
            status_color = Colors.YELLOW if status == 'in_progress' else Colors.RED
            print(f"Status: {status_color}{status}{Colors.RESET}")
            completed = [k for k, v in final_progress.get('phases', {}).items() if v == 'completed']
            if completed:
                print(f"Completed phases: {Colors.GREEN}{', '.join(completed)}{Colors.RESET}")
            blocked = final_progress.get('blockedModules', [])
            if blocked:
                print(f"Blocked modules: {Colors.YELLOW}{', '.join(blocked)}{Colors.RESET}")
        print(f"\n{Colors.CYAN}Run with --resume to continue{Colors.RESET}")

    sys.exit(exit_code)


if __name__ == "__main__":
    main()
