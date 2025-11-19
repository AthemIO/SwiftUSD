//
// Copyright 2025 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//

#include "HdSt/dynamicCubemapTextureObject.h"

#include "HdSt/dynamicCubemapTextureImplementation.h"
#include "HdSt/resourceRegistry.h"
#include "HdSt/textureHandleRegistry.h"
#include "HdSt/subtextureIdentifier.h"

PXR_NAMESPACE_OPEN_SCOPE

HdStDynamicCubemapTextureObject::HdStDynamicCubemapTextureObject(
    const HdStTextureIdentifier &textureId,
    HdSt_TextureObjectRegistry * const textureObjectRegistry)
  : HdStCubemapTextureObject(textureId, textureObjectRegistry)
{
}

HdStDynamicCubemapTextureObject::~HdStDynamicCubemapTextureObject()
{
    _DestroyTexture();
}

HdStDynamicCubemapTextureImplementation *
HdStDynamicCubemapTextureObject::_GetImpl() const
{
    const auto * const subId =
        dynamic_cast<const HdStDynamicCubemapSubtextureIdentifier *>(
            GetTextureIdentifier().GetSubtextureIdentifier());
    if (!TF_VERIFY(subId)) {
        return nullptr;
    }

    return subId->GetTextureImplementation();
}

bool
HdStDynamicCubemapTextureObject::IsValid() const
{
    if (HdStDynamicCubemapTextureImplementation * const impl = _GetImpl()) {
        return impl->IsValid(this);
    }
    return true;
}

void
HdStDynamicCubemapTextureObject::_Load()
{
    if (HdStDynamicCubemapTextureImplementation * const impl = _GetImpl()) {
        impl->Load(this);
    }
}

void
HdStDynamicCubemapTextureObject::_Commit()
{
    if (HdStDynamicCubemapTextureImplementation * const impl = _GetImpl()) {
        impl->Commit(this);
    }
}

PXR_NAMESPACE_CLOSE_SCOPE
