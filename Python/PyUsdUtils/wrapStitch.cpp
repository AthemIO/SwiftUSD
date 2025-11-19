//
// Copyright 2016 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
/// \file wrapStitch.cpp

#include "pxr/pxrns.h"
#include "pxr/external/boost/python/def.hpp"

#include "UsdUtils/stitch.h"

PXR_NAMESPACE_USING_DIRECTIVE

using namespace pxr_boost::python;

void 
wrapStitch()
{
    def("StitchLayers", 
        (void(*)(const SdfLayerHandle&, const SdfLayerHandle&))
            &UsdUtilsStitchLayers,
        (arg("strongLayer"), arg("weakLayer")));
    def("StitchInfo", 
        (void(*)(const SdfSpecHandle&, const SdfSpecHandle&))
            &UsdUtilsStitchInfo, 
        (arg("strongObj"), arg("weakObj")));
}
