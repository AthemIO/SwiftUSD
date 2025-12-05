//
// Copyright 2024 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//

#include "UsdValidation/swiftBridge.h"

// Include the full headers for the implementation.
// These types don't export cleanly to Swift, but we need them here.
#include "UsdValidation/context.h"
#include "UsdValidation/registry.h"
#include "UsdValidation/validator.h"

#include "Usd/primRange.h"

PXR_NAMESPACE_OPEN_SCOPE

// ---------------------------------------------------------------------------
// UsdValidationContext Bridge Functions
// ---------------------------------------------------------------------------

void*
UsdValidation_Swift_CreateContextFromKeywords(
    const TfTokenVector &keywords,
    bool includeAllAncestors)
{
    return static_cast<void*>(
        new UsdValidationContext(keywords, includeAllAncestors));
}

void*
UsdValidation_Swift_CreateContextFromMetadata(
    const UsdValidationValidatorMetadataVector &metadata,
    bool includeAllAncestors)
{
    return static_cast<void*>(
        new UsdValidationContext(metadata, includeAllAncestors));
}

void
UsdValidation_Swift_DestroyContext(void *context)
{
    delete static_cast<UsdValidationContext*>(context);
}

UsdValidationErrorVector
UsdValidation_Swift_ValidateLayer(
    void *context,
    const SdfLayerHandle &layer)
{
    if (!context) {
        return UsdValidationErrorVector();
    }
    return static_cast<UsdValidationContext*>(context)->Validate(layer);
}

UsdValidationErrorVector
UsdValidation_Swift_ValidateStage(
    void *context,
    const UsdStageRefPtr &stage)
{
    if (!context) {
        return UsdValidationErrorVector();
    }
    // UsdStageRefPtr implicitly converts to UsdStagePtr for the C++ API
    return static_cast<UsdValidationContext*>(context)->Validate(stage);
}

UsdValidationErrorVector
UsdValidation_Swift_ValidateStageWithTimeRange(
    void *context,
    const UsdStageRefPtr &stage,
    const UsdValidationTimeRange &timeRange)
{
    if (!context) {
        return UsdValidationErrorVector();
    }
    // UsdStageRefPtr implicitly converts to UsdStagePtr for the C++ API
    return static_cast<UsdValidationContext*>(context)->Validate(
        stage, timeRange);
}

UsdValidationErrorVector
UsdValidation_Swift_ValidatePrims(
    void *context,
    const std::vector<UsdPrim> &prims,
    const UsdValidationTimeRange &timeRange)
{
    if (!context) {
        return UsdValidationErrorVector();
    }
    return static_cast<UsdValidationContext*>(context)->Validate(
        prims, timeRange);
}

// ---------------------------------------------------------------------------
// UsdValidationRegistry Bridge Functions
// ---------------------------------------------------------------------------

bool
UsdValidation_Swift_HasValidator(const TfToken &validatorName)
{
    return UsdValidationRegistry::GetInstance().HasValidator(validatorName);
}

bool
UsdValidation_Swift_HasValidatorSuite(const TfToken &suiteName)
{
    return UsdValidationRegistry::GetInstance().HasValidatorSuite(suiteName);
}

bool
UsdValidation_Swift_GetValidatorMetadata(
    const TfToken &validatorName,
    UsdValidationValidatorMetadata *outMetadata)
{
    if (!outMetadata) {
        return false;
    }
    return UsdValidationRegistry::GetInstance().GetValidatorMetadata(
        validatorName, outMetadata);
}

UsdValidationValidatorMetadataVector
UsdValidation_Swift_GetAllValidatorMetadata()
{
    return UsdValidationRegistry::GetInstance().GetAllValidatorMetadata();
}

UsdValidationValidatorMetadataVector
UsdValidation_Swift_GetValidatorMetadataForKeyword(const TfToken &keyword)
{
    return UsdValidationRegistry::GetInstance().GetValidatorMetadataForKeyword(
        keyword);
}

UsdValidationValidatorMetadataVector
UsdValidation_Swift_GetValidatorMetadataForKeywords(const TfTokenVector &keywords)
{
    return UsdValidationRegistry::GetInstance().GetValidatorMetadataForKeywords(
        keywords);
}

UsdValidationValidatorMetadataVector
UsdValidation_Swift_GetValidatorMetadataForPlugin(const TfToken &pluginName)
{
    return UsdValidationRegistry::GetInstance().GetValidatorMetadataForPlugin(
        pluginName);
}

UsdValidationValidatorMetadataVector
UsdValidation_Swift_GetValidatorMetadataForSchemaType(const TfToken &schemaType)
{
    return UsdValidationRegistry::GetInstance().GetValidatorMetadataForSchemaType(
        schemaType);
}

// ---------------------------------------------------------------------------
// UsdValidationValidator Bridge Functions
// ---------------------------------------------------------------------------

const void*
UsdValidation_Swift_GetOrLoadValidatorByName(const TfToken &validatorName)
{
    return static_cast<const void*>(
        UsdValidationRegistry::GetInstance().GetOrLoadValidatorByName(validatorName));
}

UsdValidationValidatorMetadata
UsdValidation_Swift_GetValidatorMetadataFromValidator(const void *validator)
{
    if (!validator) {
        return UsdValidationValidatorMetadata();
    }
    return static_cast<const UsdValidationValidator*>(validator)->GetMetadata();
}

UsdValidationErrorVector
UsdValidation_Swift_ValidatorValidateLayer(
    const void *validator,
    const SdfLayerHandle &layer)
{
    if (!validator) {
        return UsdValidationErrorVector();
    }
    return static_cast<const UsdValidationValidator*>(validator)->Validate(layer);
}

UsdValidationErrorVector
UsdValidation_Swift_ValidatorValidateStage(
    const void *validator,
    const UsdStageRefPtr &stage,
    const UsdValidationTimeRange &timeRange)
{
    if (!validator) {
        return UsdValidationErrorVector();
    }
    // UsdStageRefPtr implicitly converts to UsdStagePtr for the C++ API
    return static_cast<const UsdValidationValidator*>(validator)->Validate(
        stage, timeRange);
}

UsdValidationErrorVector
UsdValidation_Swift_ValidatorValidatePrim(
    const void *validator,
    const UsdPrim &prim,
    const UsdValidationTimeRange &timeRange)
{
    if (!validator) {
        return UsdValidationErrorVector();
    }
    return static_cast<const UsdValidationValidator*>(validator)->Validate(
        prim, timeRange);
}

// ---------------------------------------------------------------------------
// UsdValidationValidatorSuite Bridge Functions
// ---------------------------------------------------------------------------

const void*
UsdValidation_Swift_GetOrLoadValidatorSuiteByName(const TfToken &suiteName)
{
    return static_cast<const void*>(
        UsdValidationRegistry::GetInstance().GetOrLoadValidatorSuiteByName(suiteName));
}

UsdValidationValidatorMetadata
UsdValidation_Swift_GetValidatorSuiteMetadata(const void *suite)
{
    if (!suite) {
        return UsdValidationValidatorMetadata();
    }
    return static_cast<const UsdValidationValidatorSuite*>(suite)->GetMetadata();
}

size_t
UsdValidation_Swift_GetValidatorSuiteValidatorCount(const void *suite)
{
    if (!suite) {
        return 0;
    }
    return static_cast<const UsdValidationValidatorSuite*>(suite)
        ->GetContainedValidators().size();
}

// ---------------------------------------------------------------------------
// Helper Functions
// ---------------------------------------------------------------------------

TfTokenVector
UsdValidation_Swift_CreateTokenVector(
    const char* const* tokens,
    size_t count)
{
    TfTokenVector result;
    result.reserve(count);
    for (size_t i = 0; i < count; ++i) {
        if (tokens[i]) {
            result.push_back(TfToken(tokens[i]));
        }
    }
    return result;
}

PXR_NAMESPACE_CLOSE_SCOPE
