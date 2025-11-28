//
// Copyright 2025 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
#ifndef PXR_USD_IMAGING_USD_RI_PXR_IMAGING_PXR_CAMERA_PROJECTION_API_ADAPTER_H
#define PXR_USD_IMAGING_USD_RI_PXR_IMAGING_PXR_CAMERA_PROJECTION_API_ADAPTER_H

#include "Tf/token.h"
#include "Hd/dataSource.h"

#include "Hd/dataSourceLocator.h"
#include "Usd/prim.h"
#include "UsdImaging/dataSourceStageGlobals.h"
#include "UsdImaging/types.h"
#include "UsdRiPxrImaging/api.h"
#include "UsdImaging/apiSchemaAdapter.h"

#include "pxr/pxrns.h"

PXR_NAMESPACE_OPEN_SCOPE

class UsdRiPxrImagingCameraProjectionAPIAdapter
  : public UsdImagingAPISchemaAdapter
{
public:
    using BaseAdapter = UsdImagingAPISchemaAdapter;

    USDRIPXRIMAGING_API
    HdContainerDataSourceHandle
    GetImagingSubprimData(
        const UsdPrim& prim,
        const TfToken& subprim,
        const TfToken& appliedInstanceName,
        const UsdImagingDataSourceStageGlobals& stageGlobals) override;

    USDRIPXRIMAGING_API
    HdDataSourceLocatorSet
    InvalidateImagingSubprim(
        const UsdPrim& prim,
        const TfToken& subprim,
        const TfToken& appliedInstanceName,
        const TfTokenVector& properties,
        UsdImagingPropertyInvalidationType invalidationType) override;
};

PXR_NAMESPACE_CLOSE_SCOPE

#endif
