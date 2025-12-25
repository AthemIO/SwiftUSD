// HdEngine.h - Hydra Engine for SwiftUSD
// Mirrors: pxr/imaging/hd/engine.h
// C++ header with Swift interop annotations
//
// HdEngine is the application-facing entry point for accessing Hydra.
// It orchestrates the execution of render tasks and manages the task
// context for inter-task communication.

#pragma once

#include "../Swift.h"
#include "../base/Tf.h"
#include "../usd/Sdf.h"
#include "Hd.h"

#include <atomic>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#if defined(USE_PIXAR_USD)
#include "pxr/pxr.h"
#include "pxr/imaging/hd/engine.h"
#include "pxr/imaging/hd/task.h"
#endif // USE_PIXAR_USD

namespace swiftusd {

// ============================================================================
// MARK: - HdTaskRef (Render task wrapper)
// ============================================================================

/// A reference to an Hd render task.
/// Tasks are the unit of work in the Hydra execution pipeline.
/// They are created by scene delegates and executed by the engine.
///
/// Note: Creating custom tasks requires subclassing HdTask in C++.
/// This wrapper provides access to task properties and state.
class HdTaskRef {
public:
    /// Get the scene path of this task.
    Path GetId() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (_task) return Path(_task->GetId());
        return Path();
#else
        return _id;
#endif
    }

    /// Check if this task has converged (for progressive rendering).
    /// Returns true if the task has completed rendering to a stable state.
    bool IsConverged() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (_task) return _task->IsConverged();
        return true;
#else
        return _converged;
#endif
    }

    /// Check if this task wrapper is valid.
    bool IsValid() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return _task != nullptr;
#else
        return !_id.IsEmpty();
#endif
    }

    // Reference counting
    void retain() SWIFTUSD_NOEXCEPT {
        _refCount.fetch_add(1, std::memory_order_relaxed);
    }

    void release() SWIFTUSD_NOEXCEPT {
        if (_refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) {
            delete this;
        }
    }

private:
    friend class HdEngine;

#if defined(USE_PIXAR_USD)
    explicit HdTaskRef(std::shared_ptr<PXR_NS::HdTask> task) SWIFTUSD_NOEXCEPT
        : _task(std::move(task)), _refCount(1) {}

    std::shared_ptr<PXR_NS::HdTask> _task;
#else
    explicit HdTaskRef(const Path& id) SWIFTUSD_NOEXCEPT
        : _id(id), _converged(true), _refCount(1) {}

    Path _id;
    bool _converged;
#endif
    std::atomic<int> _refCount;

} SWIFT_UNSAFE_REFERENCE;

// ============================================================================
// MARK: - HdEngine (Application entry point)
// ============================================================================

/// The application-facing entry point for accessing Hydra.
/// Typically an application would only create one of these.
///
/// The engine orchestrates the 4-phase execution of render tasks:
/// 1. Data Discovery - RenderIndex syncs all prims
/// 2. Prepare - Tasks resolve inter-prim bindings
/// 3. Data Commit - RenderDelegate commits GPU resources
/// 4. Execute - Tasks render to frame buffers
///
/// The engine also manages a task context dictionary that allows
/// tasks to communicate with each other during execution.
class HdEngine {
public:
    /// Create a new Hydra engine.
    static HdEngine* Create() SWIFTUSD_NOEXCEPT {
        return new HdEngine();
    }

    /// Set data in the task context with a string value.
    /// The task context is a token-to-value map shared between tasks.
    void SetTaskContextData(const Token& id, const char* data) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        _engine.SetTaskContextData(id.GetTfToken(), PXR_NS::VtValue(std::string(data ? data : "")));
#else
        _taskContext[id.GetText()] = std::string(data ? data : "");
#endif
    }

    /// Check if task context data exists for the given key.
    bool HasTaskContextData(const Token& id) const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        PXR_NS::VtValue val;
        return const_cast<PXR_NS::HdEngine&>(_engine).GetTaskContextData(id.GetTfToken(), &val);
#else
        return _taskContext.find(id.GetText()) != _taskContext.end();
#endif
    }

    /// Get task context data as a string.
    /// Returns empty string if the key doesn't exist or value isn't a string.
    SWIFT_RETURNS_INDEPENDENT_VALUE
    const char* GetTaskContextData(const Token& id) const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        PXR_NS::VtValue val;
        if (const_cast<PXR_NS::HdEngine&>(_engine).GetTaskContextData(id.GetTfToken(), &val)) {
            if (val.IsHolding<std::string>()) {
                _lastContextValue = val.UncheckedGet<std::string>();
                return _lastContextValue.c_str();
            }
        }
        return "";
#else
        auto it = _taskContext.find(id.GetText());
        if (it != _taskContext.end()) {
            return it->second.c_str();
        }
        return "";
#endif
    }

    /// Remove data from the task context.
    void RemoveTaskContextData(const Token& id) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        _engine.RemoveTaskContextData(id.GetTfToken());
#else
        _taskContext.erase(id.GetText());
#endif
    }

    /// Clear all task context data.
    void ClearTaskContextData() SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        _engine.ClearTaskContextData();
#else
        _taskContext.clear();
#endif
    }

    /// Get the number of items in the task context.
    size_t GetTaskContextSize() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        // HdEngine doesn't expose task context size, return 0
        return 0;
#else
        return _taskContext.size();
#endif
    }

    /// Execute rendering with the given render index.
    /// This runs the 4-phase execution pipeline with an empty task list.
    /// For actual rendering, use ExecuteWithTasks or ExecuteTaskPaths.
    void Execute(HdRenderIndexRef* index) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (index && index->GetPxrRenderIndex()) {
            PXR_NS::HdTaskSharedPtrVector emptyTasks;
            _engine.Execute(index->GetPxrRenderIndex(), &emptyTasks);
        }
#else
        (void)index;
        _lastExecutedTaskCount = 0;
#endif
    }

    /// Execute rendering with task paths looked up from the render index.
    /// The tasks are retrieved from the render index's scene delegate.
    ///
    /// @param index The render index containing scene data
    /// @param taskPaths Array of paths to tasks to execute
    /// @param taskCount Number of paths in the array
    void ExecuteTaskPaths(HdRenderIndexRef* index, const Path* taskPaths, size_t taskCount) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (index && index->GetPxrRenderIndex() && taskPaths && taskCount > 0) {
            PXR_NS::SdfPathVector paths;
            paths.reserve(taskCount);
            for (size_t i = 0; i < taskCount; ++i) {
                paths.push_back(taskPaths[i].GetSdfPath());
            }
            _engine.Execute(index->GetPxrRenderIndex(), paths);
        }
#else
        (void)index;
        (void)taskPaths;
        _lastExecutedTaskCount = taskCount;
#endif
    }

    /// Check if all tasks at the given paths have converged.
    /// Used for progressive rendering to determine if rendering is complete.
    ///
    /// @param index The render index containing scene data
    /// @param taskPaths Array of paths to tasks to check
    /// @param taskCount Number of paths in the array
    /// @return true if all tasks have converged, false if any are still rendering
    bool AreTasksConverged(HdRenderIndexRef* index, const Path* taskPaths, size_t taskCount) const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (index && index->GetPxrRenderIndex() && taskPaths && taskCount > 0) {
            PXR_NS::SdfPathVector paths;
            paths.reserve(taskCount);
            for (size_t i = 0; i < taskCount; ++i) {
                paths.push_back(taskPaths[i].GetSdfPath());
            }
            // Note: AreTasksConverged is not const in HdEngine, cast needed
            return const_cast<PXR_NS::HdEngine&>(_engine).AreTasksConverged(
                index->GetPxrRenderIndex(), paths);
        }
        return true;
#else
        (void)index;
        (void)taskPaths;
        (void)taskCount;
        return true;
#endif
    }

    /// Get the number of tasks executed in the last Execute call.
    /// For debugging and testing purposes.
    size_t GetLastExecutedTaskCount() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return 0; // Not tracked in real USD
#else
        return _lastExecutedTaskCount;
#endif
    }

    // Reference counting
    void retain() SWIFTUSD_NOEXCEPT {
        _refCount.fetch_add(1, std::memory_order_relaxed);
    }

    void release() SWIFTUSD_NOEXCEPT {
        if (_refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) {
            delete this;
        }
    }

private:
    HdEngine() SWIFTUSD_NOEXCEPT : _refCount(1) {
#if !defined(USE_PIXAR_USD)
        _lastExecutedTaskCount = 0;
#endif
    }

#if defined(USE_PIXAR_USD)
    PXR_NS::HdEngine _engine;
    mutable std::string _lastContextValue;
#else
    std::unordered_map<std::string, std::string> _taskContext;
    size_t _lastExecutedTaskCount;
#endif
    std::atomic<int> _refCount;

} SWIFT_UNSAFE_REFERENCE;

} // namespace swiftusd
