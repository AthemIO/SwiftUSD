//
// Copyright 2025 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//

#include "Exec/swiftBridge.h"
#include "Exec/builtinComputations.h"

PXR_NAMESPACE_OPEN_SCOPE

TfToken
Exec_Swift_GetComputeTimeToken()
{
    return ExecBuiltinComputations->computeTime;
}

TfToken
Exec_Swift_GetComputeValueToken()
{
    return ExecBuiltinComputations->computeValue;
}

std::vector<TfToken>
Exec_Swift_GetAllBuiltinComputationTokens()
{
    return ExecBuiltinComputations->GetComputationTokens();
}

PXR_NAMESPACE_CLOSE_SCOPE
