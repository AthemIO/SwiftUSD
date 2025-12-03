## ULTRATHINK MODE ENABLED

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

# SwiftUSD Alignment Task

You are performing an autonomous alignment of SwiftUSD to OpenUSD version 25.11.

## Full Access Directories
You have FULL READ/WRITE access to these three directories:

1. **SwiftUSD** (/Users/jonathanpeterson/dev/SwiftUSD)
   - Swift wrapper for OpenUSD
   - Contains: Sources/, Package.swift, Swift modules
   - This is what you're updating

2. **OpenUSD** (/Users/jonathanpeterson/dev/OpenUSD)
   - C++ reference source (version 25.11)
   - Contains: pxr/, CHANGELOG.md, build_scripts/, cmake/
   - Use as reference for what needs to change

3. **MetaverseKit** (/Users/jonathanpeterson/dev/MetaverseKit)
   - Dependencies: TBB, MaterialX, OpenSubdiv, OpenEXR, etc.
   - Contains: Sources/, Package.swift
   - Update these when OpenUSD requires newer versions

## Wabiverse Conventions
Reference: LynrAI/SwiftUSD@77abfccf (style guide for Swift wrapper patterns)

## READ-BEFORE-MODIFY PROTOCOL (MANDATORY)

**CRITICAL RULE**: Before modifying ANY C++ file in SwiftUSD, you MUST first read the corresponding file in OpenUSD at `/Users/jonathanpeterson/dev/OpenUSD`.

### Required Workflow
For EVERY C++ source file modification:

1. **READ OpenUSD FIRST**
   ```
   Before editing: Sources/Tf/token.cpp
   First read:     /Users/jonathanpeterson/dev/OpenUSD/pxr/base/tf/token.cpp
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



## Your Mission
Execute the alignment workflow autonomously, following these phases IN ORDER:

### Phase 0: MetaverseKit Dependency Updates (FIRST)
1. Read /Users/jonathanpeterson/dev/OpenUSD/CHANGELOG.md for dependency requirements
2. Read /Users/jonathanpeterson/dev/OpenUSD/build_scripts/build_usd.py for exact version URLs
3. Compare versions in /Users/jonathanpeterson/dev/MetaverseKit/Package.swift and source headers
4. For each outdated dependency:
   - Download new source from official URL
   - Update source files in MetaverseKit/Sources/{Dependency}/
   - Update version info in Package.swift if needed
5. Verify MetaverseKit builds: `swift build` in /Users/jonathanpeterson/dev/MetaverseKit

### Phase 1: Pre-Analysis
1. Detect SOURCE_VERSION from /Users/jonathanpeterson/dev/SwiftUSD/Sources/PixarUSD/Pixar.swift
2. Parse /Users/jonathanpeterson/dev/OpenUSD/CHANGELOG.md for ALL changes between SOURCE and 25.11
3. Create change manifest with:
   - api_removals: Classes/functions/modules REMOVED in target version (MUST DELETE from SwiftUSD)
   - api_renames: Symbol name changes (old -> new) - update all references
   - module_removals: Entire modules removed (e.g., Ndr removed in 25.08)
   - module_additions: New modules to add
   - deprecated_removals: APIs that were deprecated in earlier versions and NOW REMOVED
   - signature_changes: Function parameter or return type changes
   - new_features: New APIs to expose
4. Save the manifest to /Users/jonathanpeterson/dev/SwiftUSD/.claude/change-manifest.json

**CRITICAL**: The manifest must capture EVERYTHING that was removed or deprecated-then-removed.
SwiftUSD must NOT contain any code that doesn't exist in OpenUSD 25.11.

### Phase 2: Module Processing
1. Build dependency graph from /Users/jonathanpeterson/dev/SwiftUSD/Package.swift
2. Process modules bottom-up by dependency layer:
   - Layer 1: Arch (foundation)
   - Layer 2: Tf, Gf, Js, Trace, Work, Vt
   - Layer 3: Plug
   - ... up to UsdImaging

3. For each module, apply changes IN THIS ORDER:

   **Step 0: READ OPENUSD FIRST (REQUIRED)**
   - For EACH C++ file you plan to modify, FIRST read the OpenUSD counterpart
   - Example: Before editing `Sources/Tf/token.cpp`, first read `/Users/jonathanpeterson/dev/OpenUSD/pxr/base/tf/token.cpp`
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
   - Copy updated C++ headers from /Users/jonathanpeterson/dev/OpenUSD/pxr/
   - Copy updated C++ source files
   - Create Swift wrappers for new APIs following wabiverse conventions

   **Step E: Validate**
   - Run: `swift build --target {Module}`
   - Fix any remaining errors before proceeding

**REMOVAL CHECKLIST** (apply for each removed item):
- [ ] Delete the source files (.h, .cpp, .swift)
- [ ] Remove from CMakeLists.txt or Package.swift
- [ ] Remove imports in other files
- [ ] Remove usages in other files
- [ ] Remove Swift typealiases
- [ ] Remove from module exports

### Phase 3: Build Iteration
1. Run full build: `swift bundler run -c release` in /Users/jonathanpeterson/dev/SwiftUSD
2. Parse and classify errors
3. Fix errors referencing the change manifest and OpenUSD source
4. Repeat until clean build (max 10 iterations)

### Phase 4: Final Validation
1. Verify clean build succeeds
2. Update version in Pixar.swift to 25.11.x
3. Save final progress to /Users/jonathanpeterson/dev/SwiftUSD/.claude/alignment-progress.json

## Progress Tracking
After completing each phase, update the progress file at:
/Users/jonathanpeterson/dev/SwiftUSD/.claude/alignment-progress.json

Use this JSON structure:
```json
{
  "targetVersion": "25.11",
  "sourceVersion": "<detected>",
  "status": "in_progress|completed|failed",
  "startedAt": "<timestamp>",
  "phases": {
    "metaversekit_update": "pending|in_progress|completed",
    "changelog_analysis": "pending|in_progress|completed",
    "module_processing": "pending|in_progress|completed",
    "build_iteration": "pending|in_progress|completed",
    "final_validation": "pending|in_progress|completed"
  },
  "completedModules": [],
  "blockedModules": [],
  "buildIterations": 0,
  "lastError": null
}
```

## Important Guidelines
- Update progress file after each phase completion
- Save change manifest early - reference it throughout
- Follow wabiverse Swift wrapper patterns
- If stuck on same error 3x, mark module as blocked and continue
- For MetaverseKit updates, download official source releases

## Development Environment (DO NOT CHANGE)
The following toolchain is correctly configured and working:
- **Xcode 16.4** - This is the correct version. DO NOT suggest changing Xcode versions.
- **Swift 6.1** - Included with Xcode 16.4
- **macOS Sequoia** - Current development platform

If you encounter build errors, the issue is NOT the Xcode/Swift version. Focus on fixing the code.

## Git Commits & Push
After completing each phase, commit and push your changes:

1. **After Phase 0 (MetaverseKit)**:
   ```bash
   cd /Users/jonathanpeterson/dev/MetaverseKit
   git add -A
   git commit -m "chore: update dependencies for OpenUSD 25.11

   - Updated TBB, MaterialX, OpenSubdiv, etc. to required versions
   - Verified build passes

   🤖 Generated with Claude Code (SwiftUSD Alignment Agent)"
   git push
   ```

2. **After Phase 1 (Pre-Analysis)**:
   ```bash
   cd /Users/jonathanpeterson/dev/SwiftUSD
   git add .claude/
   git commit -m "chore: add change manifest for 25.11 alignment

   - Parsed CHANGELOG for breaking changes
   - Created change manifest with API removals, renames, additions

   🤖 Generated with Claude Code (SwiftUSD Alignment Agent)"
   git push
   ```

3. **After Phase 2 (Module Processing)** - Commit after each module or batch:
   ```bash
   cd /Users/jonathanpeterson/dev/SwiftUSD
   git add -A
   git commit -m "feat: align {Module} to OpenUSD 25.11

   - Applied API changes from change manifest
   - Synced C++ code from OpenUSD
   - Updated Swift wrappers

   🤖 Generated with Claude Code (SwiftUSD Alignment Agent)"
   git push
   ```

4. **After Phase 3 (Build Fixes)**:
   ```bash
   cd /Users/jonathanpeterson/dev/SwiftUSD
   git add -A
   git commit -m "fix: resolve build errors for OpenUSD 25.11

   - Fixed compilation errors from alignment
   - Build iteration {N}

   🤖 Generated with Claude Code (SwiftUSD Alignment Agent)"
   git push
   ```

5. **After Phase 4 (Final)**:
   ```bash
   cd /Users/jonathanpeterson/dev/SwiftUSD
   git add -A
   git commit -m "chore: complete alignment to OpenUSD 25.11

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

**SwiftUSD MUST be a 1:1 match with OpenUSD 25.11.**

This means:
1. **NO deprecated APIs** - If it's removed in OpenUSD 25.11, DELETE it from SwiftUSD
2. **NO legacy code** - Remove all backward-compatibility shims
3. **NO orphaned files** - Delete files that no longer have OpenUSD counterparts
4. **ALL new APIs** - Add everything new in 25.11

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
