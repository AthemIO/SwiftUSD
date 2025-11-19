//
// Copyright 2025 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
#include "UsdSkelImaging/resolvingSceneIndexPlugin.h"

#include "UsdSkelImaging/pointsResolvingSceneIndex.h"
#include "UsdSkelImaging/bindingSchema.h"
#include "UsdSkelImaging/skeletonResolvingSceneIndex.h"

#include "Hd/flattenedDataSourceProviders.h"
#include "Hd/flattenedOverlayDataSourceProvider.h"

#include "Hd/retainedDataSource.h"

PXR_NAMESPACE_OPEN_SCOPE

TF_REGISTRY_FUNCTION(UsdImagingSceneIndexPlugin)
{
    UsdImagingSceneIndexPlugin::Define<
        UsdSkelImagingResolvingSceneIndexPlugin>();
}

HdSceneIndexBaseRefPtr
UsdSkelImagingResolvingSceneIndexPlugin::AppendSceneIndex(
    HdSceneIndexBaseRefPtr const &inputScene)
{
    HdSceneIndexBaseRefPtr sceneIndex = inputScene;

    sceneIndex =
        UsdSkelImagingSkeletonResolvingSceneIndex::New(sceneIndex);

    sceneIndex =
        UsdSkelImagingPointsResolvingSceneIndex::New(sceneIndex);

    return sceneIndex;
}

HdContainerDataSourceHandle
UsdSkelImagingResolvingSceneIndexPlugin::FlattenedDataSourceProviders()
{
    using namespace HdMakeDataSourceContainingFlattenedDataSourceProvider;

    return HdRetainedContainerDataSource::New(
        UsdSkelImagingBindingSchema::GetSchemaToken(),
        Make<HdFlattenedOverlayDataSourceProvider>());
}

TfTokenVector
UsdSkelImagingResolvingSceneIndexPlugin::InstanceDataSourceNames()
{
    return {
        UsdSkelImagingBindingSchema::GetSchemaToken()
    };
}

TfTokenVector
UsdSkelImagingResolvingSceneIndexPlugin::ProxyPathTranslationDataSourceNames()
{
    return {
        UsdSkelImagingBindingSchema::GetSchemaToken()
    };
}


PXR_NAMESPACE_CLOSE_SCOPE
