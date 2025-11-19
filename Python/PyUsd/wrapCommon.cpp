//
// Copyright 2016 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
#include "pxr/pxrns.h"
#include "Usd/common.h"
#include "Usd/object.h"
#include "Usd/stage.h"
#include "Usd/stageCache.h"
#include "Tf/pyEnum.h"

#include "pxr/external/boost/python/def.hpp"
#include "pxr/external/boost/python/enum.hpp"


PXR_NAMESPACE_USING_DIRECTIVE

using namespace pxr_boost::python;

void wrapUsdCommon()
{
    def("Describe", (std::string (*)(const UsdObject &)) UsdDescribe);
    def("Describe", (std::string (*)(const UsdStageWeakPtr &)) UsdDescribe);
    def("Describe", (std::string (*)(const UsdStageCache &)) UsdDescribe);

    TfPyWrapEnum<UsdListPosition>();
    TfPyWrapEnum<UsdLoadPolicy>();
    enum_<UsdSchemaKind>("SchemaKind")
        .value("Invalid", UsdSchemaKind::Invalid)
        .value("AbstractBase", UsdSchemaKind::AbstractBase)
        .value("AbstractTyped", UsdSchemaKind::AbstractTyped)
        .value("ConcreteTyped", UsdSchemaKind::ConcreteTyped)
        .value("NonAppliedAPI", UsdSchemaKind::NonAppliedAPI)
        .value("SingleApplyAPI", UsdSchemaKind::SingleApplyAPI)
        .value("MultipleApplyAPI", UsdSchemaKind::MultipleApplyAPI)
    ;
}
