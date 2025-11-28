//
// Copyright 2025 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
#ifndef PXR_BASE_WORK_TBB_IMPL_H
#define PXR_BASE_WORK_TBB_IMPL_H

/// This file pulls in the entire work implementation

#include "pxr/pxrns.h"

#include "Work/detachedTask_impl.h"
#include "Work/dispatcher_impl.h"
#include "Work/isolatingDispatcher_impl.h"
#include "Work/loops_impl.h"
#include "Work/reduce_impl.h"
#include "Work/sort_impl.h"
#include "Work/taskGraph_impl.h"
#include "Work/threadLimits_impl.h"
#include "Work/withScopedParallelism_impl.h"

#if PXR_USE_NAMESPACES
#define WORK_IMPL_NS PXR_NS
#define PXR_WORK_IMPL_NAMESPACE_USING_DIRECTIVE using namespace WORK_IMPL_NS
#else
#define WORK_IMPL_NS
#define PXR_WORK_IMPL_NAMESPACE_USING_DIRECTIVE
#endif

#endif // PXR_BASE_WORK_TBB_IMPL_H
