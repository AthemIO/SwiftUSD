//
// Copyright 2016 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
#include "pxr/pxrns.h"

#include "pxr/external/boost/python/class.hpp"
#include "pxr/external/boost/python/def.hpp"
#include "pxr/external/boost/python/scope.hpp"
#include "pxr/external/boost/python/tuple.hpp"

#include "Tf/pyResultConversions.h"

#include "UsdShade/shaderDefUtils.h"
#include "UsdShade/shader.h"

PXR_NAMESPACE_USING_DIRECTIVE

using namespace pxr_boost::python;

void wrapUsdShadeShaderDefUtils()
{
    scope thisScope = class_<UsdShadeShaderDefUtils>("ShaderDefUtils", no_init)
        .def("GetDiscoveryResults", 
             &UsdShadeShaderDefUtils::GetDiscoveryResults,
             (arg("shaderDef"), arg("sourceUri")),
             return_value_policy<TfPySequenceToList>())
        .staticmethod("GetDiscoveryResults")
        .def("GetProperties", 
             &UsdShadeShaderDefUtils::GetProperties,
             arg("shaderDef"),
             return_value_policy<TfPySequenceToList>())
        .staticmethod("GetProperties")
        .def("GetPrimvarNamesMetadataString", 
             &UsdShadeShaderDefUtils::GetPrimvarNamesMetadataString,
             (arg("metadata"), arg("shaderDef")))
        .staticmethod("GetPrimvarNamesMetadataString")
    ;
}
