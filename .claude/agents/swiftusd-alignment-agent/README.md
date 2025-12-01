# SwiftUSD Alignment Agent

An autonomous agent framework for aligning SwiftUSD to any OpenUSD release version.

## Overview

This agent uses **Claude Code** (via your Max subscription) with **full access** to three repositories:
- `~/dev/SwiftUSD` - The Swift wrapper codebase (target of updates)
- `~/dev/OpenUSD` - The C++ reference source (determines target version)
- `~/dev/MetaverseKit` - Dependencies (TBB, MaterialX, OpenSubdiv, etc.)

The agent **auto-detects** the OpenUSD version from `~/dev/OpenUSD`. Simply update that repository to your desired version and run the agent.

**No API key needed** - uses your Claude Code Max subscription!

## Prerequisites

1. **Claude Code CLI** - Already installed with your Max subscription
2. **Python 3.10+**
3. **Required Repositories**:
   ```
   ~/dev/SwiftUSD      # Swift wrapper
   ~/dev/OpenUSD       # C++ reference (at target version)
   ~/dev/MetaverseKit  # Dependencies
   ```

## Usage

### Basic Usage

1. Update `~/dev/OpenUSD` to your target version:
   ```bash
   cd ~/dev/OpenUSD
   git fetch --tags
   git checkout release  # or specific tag like v25.11
   ```

2. Run the agent:
   ```bash
   cd ~/dev/SwiftUSD/.claude/agents/swiftusd-alignment
   python agent.py
   ```

The agent will auto-detect the version and handle the complete upgrade using Claude Code.

### Resume from Checkpoint

If interrupted, resume from the last checkpoint:

```bash
python agent.py --resume
```

### Custom Paths

```bash
python agent.py \
  --swiftusd-path ~/projects/SwiftUSD \
  --openusd-path ~/projects/OpenUSD \
  --metaversekit-path ~/projects/MetaverseKit
```

## What the Agent Does

### Phase 0: MetaverseKit Dependency Updates
- Reads OpenUSD CHANGELOG and build_usd.py for dependency versions
- Compares against MetaverseKit source files
- Downloads and updates outdated dependencies (TBB, MaterialX, OpenSubdiv, etc.)
- Verifies MetaverseKit builds

### Phase 1: Pre-Analysis
- Detects current SwiftUSD version (SOURCE_VERSION)
- Parses OpenUSD CHANGELOG for all changes between versions
- Creates structured change manifest:
  - API removals (classes/functions/modules removed)
  - API renames (symbol name changes)
  - Module additions (new modules to add)
  - New features (APIs to expose)

### Phase 2: Module Processing
- Builds dependency graph from Package.swift
- Processes modules bottom-up by dependency layer (Arch → Tf → ... → UsdImaging)
- For each module, in order:
  1. **REMOVE** deprecated/removed code first (delete files, remove references)
  2. **RENAME** symbols that changed names
  3. **UPDATE** function signatures that changed
  4. **SYNC** new/modified C++ code from OpenUSD
  5. **VALIDATE** with `swift build --target {Module}`

### Phase 3: Build Iteration
- Runs full build: `swift bundler run -c release`
- Parses and classifies errors
- Fixes errors based on CHANGELOG changes
- Repeats until clean build (max 10 iterations)

### Phase 4: Final Validation
- Verifies clean build succeeds
- Updates version in Pixar.swift
- Marks alignment complete

## Git Integration

The agent automatically commits and pushes changes as it progresses:

| Phase | Repository | Commit Type |
|-------|------------|-------------|
| Phase 0 | MetaverseKit | `chore: update dependencies for OpenUSD X.XX` |
| Phase 1 | SwiftUSD | `chore: add change manifest for X.XX alignment` |
| Phase 2 | SwiftUSD | `feat: align {Module} to OpenUSD X.XX` (per module/batch) |
| Phase 3 | SwiftUSD | `fix: resolve build errors for OpenUSD X.XX` |
| Phase 4 | SwiftUSD | `chore: complete alignment to OpenUSD X.XX` |

All commits include the signature:
```
🤖 Generated with Claude Code (SwiftUSD Alignment Agent)
```

## Full Alignment Philosophy

The agent ensures SwiftUSD is a **1:1 match** with the target OpenUSD version:

- **NO deprecated APIs** - Anything removed in OpenUSD is deleted from SwiftUSD
- **NO legacy code** - Backward-compatibility shims are removed
- **NO orphaned files** - Files without OpenUSD counterparts are deleted
- **ALL new APIs** - Everything new in the target version is added

This means after alignment, SwiftUSD will only contain code that exists in the target OpenUSD version.

## Progress Tracking

Progress is saved to `~/dev/SwiftUSD/.claude/alignment-progress.json`:

```json
{
  "targetVersion": "25.11",
  "sourceVersion": "24.08",
  "status": "in_progress",
  "phases": {
    "metaversekit_update": "completed",
    "changelog_analysis": "completed",
    "module_processing": "in_progress",
    ...
  },
  "completedModules": ["Arch", "Tf", "Gf", ...],
  "blockedModules": []
}
```

## Architecture

```
┌──────────────────────────────────────────────────────────┐
│              SwiftUSD Alignment Agent                     │
├──────────────────────────────────────────────────────────┤
│  Python Launcher (agent.py)                               │
│  ├─ Auto-detect OpenUSD version                          │
│  ├─ Generate alignment prompt                            │
│  └─ Launch Claude Code CLI                               │
├──────────────────────────────────────────────────────────┤
│  Claude Code (Max Subscription)                           │
│  ├─ Full file system access                              │
│  ├─ Shell command execution                              │
│  └─ Autonomous task completion                           │
├──────────────────────────────────────────────────────────┤
│  Full Access to Three Repositories                        │
│  ├─ ~/dev/SwiftUSD (read/write)                          │
│  ├─ ~/dev/OpenUSD (read/write)                           │
│  └─ ~/dev/MetaverseKit (read/write)                      │
└──────────────────────────────────────────────────────────┘
```

## Files Generated

| File | Description |
|------|-------------|
| `.claude/alignment-progress.json` | Progress tracking for resume |
| `.claude/alignment-prompt.md` | The prompt sent to Claude Code |
| `.claude/change-manifest.json` | Parsed CHANGELOG changes |

## Troubleshooting

### Agent stops without completing
- Check `alignment-progress.json` for the last state
- Use `--resume` to continue from checkpoint
- Check for blocked modules in the progress file

### Build errors persist
- The agent will mark modules as "blocked" after 3 failed iterations
- Review blocked modules manually
- Check the `buildErrors` in progress file

### Version detection fails
- Ensure `~/dev/OpenUSD` is at a valid release tag
- The agent checks: `cmake/defaults/Version.cmake`, `CHANGELOG.md`, git tags

### Claude Code not found
- Ensure Claude Code CLI is installed and in PATH
- Should be available with your Max subscription

## License

This agent is part of the SwiftUSD project.
