//
// Copyright 2025 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//

#include "HgiMetal/swiftBridge.h"
#include "HgiMetal/texture.h"

PXR_NAMESPACE_OPEN_SCOPE

id<MTLTexture> HgiMetal_GetMTLTexture(const HgiTextureHandle& handle)
{
    // Get the raw HgiTexture pointer from the handle
    HgiTexture* hgiTexture = handle.Get();
    if (!hgiTexture) {
        return nil;
    }

    // Cast to HgiMetalTexture and get the MTLTexture
    // This is safe because we know the texture was created by HgiMetal
    HgiMetalTexture* metalTexture = static_cast<HgiMetalTexture*>(hgiTexture);
    return metalTexture->GetTextureId();
}

PXR_NAMESPACE_CLOSE_SCOPE
