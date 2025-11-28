//
// Copyright 2016 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
#include "pxr/pxrns.h"
#include "pxr/external/boost/python/class.hpp"

#include "UsdUtils/stageCache.h"

#include "Tf/pyResultConversions.h"

PXR_NAMESPACE_USING_DIRECTIVE

using namespace pxr_boost::python;

void wrapStageCache()
{
    class_<UsdUtilsStageCache>("StageCache")

        .def("Get", &UsdUtilsStageCache::Get,
             return_value_policy<reference_existing_object>())
        .staticmethod("Get")

        .def("GetSessionLayerForVariantSelections",
             &UsdUtilsStageCache::GetSessionLayerForVariantSelections)
        .staticmethod("GetSessionLayerForVariantSelections")
        ;
}
