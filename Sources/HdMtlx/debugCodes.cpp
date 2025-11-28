//
// Copyright 2024 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
#include "HdMtlx/debugCodes.h"

#include "Tf/debug.h"
#include "Tf/registryManager.h"

PXR_NAMESPACE_OPEN_SCOPE

TF_REGISTRY_FUNCTION(TfDebug)
{
    TF_DEBUG_ENVIRONMENT_SYMBOL(HDMTLX_VERSION_UPGRADE,
        "Write the MaterialX documents to disk, before and after the version upgrade");

    TF_DEBUG_ENVIRONMENT_SYMBOL(HDMTLX_WRITE_DOCUMENT,
        "Write the MaterialX document to disk after reconstruction in Hydra");
}

PXR_NAMESPACE_CLOSE_SCOPE
