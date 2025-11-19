//
// Copyright 2025 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
#ifndef PXR_USD_IMAGING_USD_RI_PXR_IMAGING_PXR_CAMERA_PROJECTION_ADAPTER_H
#define PXR_USD_IMAGING_USD_RI_PXR_IMAGING_PXR_CAMERA_PROJECTION_ADAPTER_H

#include "Hd/dataSource.h"
#include "Hd/dataSourceLocator.h"

#include "UsdImaging/primAdapter.h"
#include "UsdImaging/sceneIndexPrimAdapter.h"
#include "UsdImaging/types.h"
#include "UsdRiPxrImaging/api.h"

#include "Usd/prim.h"

#include "Tf/token.h"

#include "pxr/pxrns.h"

PXR_NAMESPACE_OPEN_SCOPE

class UsdRiPxrImagingCameraProjectionAdapter
  : public UsdImagingSceneIndexPrimAdapter
{
public:
    using BaseAdapter = UsdImagingSceneIndexPrimAdapter;

    UsdRiPxrImagingCameraProjectionAdapter();

    USDRIPXRIMAGING_API
    ~UsdRiPxrImagingCameraProjectionAdapter() override;

    // ---------------------------------------------------------------------- //
    /// \name Hydra 2.0
    // ---------------------------------------------------------------------- //

    USDRIPXRIMAGING_API
    TfTokenVector
    GetImagingSubprims(
        const UsdPrim& prim) override;

    USDRIPXRIMAGING_API
    TfToken
    GetImagingSubprimType(
        const UsdPrim& prim,
        const TfToken& subprim) override;

    USDRIPXRIMAGING_API
    HdContainerDataSourceHandle
    GetImagingSubprimData(
        const UsdPrim& prim,
        const TfToken& subprim,
        const UsdImagingDataSourceStageGlobals& stageGlobals) override;

    USDRIPXRIMAGING_API
    HdDataSourceLocatorSet
    InvalidateImagingSubprim(
        const UsdPrim& prim,
        const TfToken& subprim,
        const TfTokenVector& properties,
        UsdImagingPropertyInvalidationType invalidationType) override;
};

PXR_NAMESPACE_CLOSE_SCOPE

#endif // PXR_USD_IMAGING_USD_RI_PXR_IMAGING_PXR_CAMERA_PROJECTION_ADAPTER_H
