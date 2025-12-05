//
// Copyright 2024 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
#ifndef PXR_USD_VALIDATION_USD_VALIDATION_SWIFT_BRIDGE_H
#define PXR_USD_VALIDATION_USD_VALIDATION_SWIFT_BRIDGE_H

/// \file UsdValidation/swiftBridge.h
/// Swift-compatible bridge functions for UsdValidation types.
///
/// This header provides C-style factory functions and helpers to work around
/// Swift C++ interop limitations with std::function, std::variant, std::unique_ptr,
/// and TfSingleton patterns.
///
/// IMPORTANT: Swift cannot see C++ types that contain std::function, std::variant,
/// or std::unique_ptr members. Therefore, UsdValidationContext, UsdValidationValidator,
/// UsdValidationValidatorSuite, and UsdValidationRegistry are exposed as opaque void*
/// to Swift. The implementation casts these back to the correct types.
///
/// Types that DO export cleanly to Swift (no bridge needed):
/// - UsdValidationError
/// - UsdValidationErrorVector
/// - UsdValidationErrorSite
/// - UsdValidationErrorType
/// - UsdValidationValidatorMetadata
/// - UsdValidationValidatorMetadataVector
/// - UsdValidationTimeRange

#include "pxr/pxr.h"

#include "UsdValidation/api.h"
#include "UsdValidation/error.h"
#include "UsdValidation/timeRange.h"

// Include validator.h for UsdValidationValidatorMetadata and UsdValidationErrorVector
// These types are safe (no std::function/variant members), even though the file
// also contains UsdValidationValidator which has unsafe members.
// We need these definitions for the bridge function signatures.
#include "UsdValidation/validator.h"

// Safe includes - types used in the interface that export cleanly to Swift
#include "Usd/stage.h"
#include "Usd/prim.h"
#include "Sdf/layer.h"
#include "Tf/token.h"

PXR_NAMESPACE_OPEN_SCOPE

/// @{
/// \name UsdValidationContext Swift Bridge Functions
///
/// Factory and helper functions for UsdValidationContext that work with Swift.
/// Swift cannot directly construct UsdValidationContext due to std::variant usage
/// in internal members.

/// Creates a new UsdValidationContext from a vector of keyword tokens.
/// Returns an opaque pointer that must be destroyed with UsdValidation_Swift_DestroyContext.
///
/// \param keywords A vector of TfToken keywords to select validators.
/// \param includeAllAncestors If true, include validators from ancestor TfTypes.
USDVALIDATION_API
void* UsdValidation_Swift_CreateContextFromKeywords(
    const TfTokenVector &keywords,
    bool includeAllAncestors);

/// Creates a new UsdValidationContext from a vector of validator metadata.
/// Returns an opaque pointer that must be destroyed with UsdValidation_Swift_DestroyContext.
///
/// \param metadata A vector of validator metadata to select validators.
/// \param includeAllAncestors If true, include validators from ancestor TfTypes.
USDVALIDATION_API
void* UsdValidation_Swift_CreateContextFromMetadata(
    const UsdValidationValidatorMetadataVector &metadata,
    bool includeAllAncestors);

/// Destroys a UsdValidationContext created by UsdValidation_Swift_CreateContext*.
USDVALIDATION_API
void UsdValidation_Swift_DestroyContext(void *context);

/// Run validation on a SdfLayer using the context.
/// Returns a vector of validation errors.
///
/// \param context Opaque pointer to UsdValidationContext.
/// \param layer The layer to validate.
USDVALIDATION_API
UsdValidationErrorVector UsdValidation_Swift_ValidateLayer(
    void *context,
    const SdfLayerHandle &layer);

/// Run validation on a UsdStage using the context with default settings.
/// Uses UsdTraverseInstanceProxies predicate and full time interval.
/// Returns a vector of validation errors.
///
/// \param context Opaque pointer to UsdValidationContext.
/// \param stage The stage to validate (UsdStageRefPtr for Swift compatibility).
USDVALIDATION_API
UsdValidationErrorVector UsdValidation_Swift_ValidateStage(
    void *context,
    const UsdStageRefPtr &stage);

/// Run validation on a UsdStage using the context with explicit time range.
/// Uses UsdTraverseInstanceProxies predicate.
/// Returns a vector of validation errors.
///
/// \param context Opaque pointer to UsdValidationContext.
/// \param stage The stage to validate (UsdStageRefPtr for Swift compatibility).
/// \param timeRange The time range for validation.
USDVALIDATION_API
UsdValidationErrorVector UsdValidation_Swift_ValidateStageWithTimeRange(
    void *context,
    const UsdStageRefPtr &stage,
    const UsdValidationTimeRange &timeRange);

/// Run validation on a vector of UsdPrims using the context.
/// Returns a vector of validation errors.
///
/// \param context Opaque pointer to UsdValidationContext.
/// \param prims The prims to validate.
/// \param timeRange The time range for validation (optional, defaults to full).
USDVALIDATION_API
UsdValidationErrorVector UsdValidation_Swift_ValidatePrims(
    void *context,
    const std::vector<UsdPrim> &prims,
    const UsdValidationTimeRange &timeRange);

/// @}

/// @{
/// \name UsdValidationRegistry Swift Bridge Functions
///
/// Functions to access the UsdValidationRegistry singleton and query validators.
/// The registry itself uses TfSingleton and contains std::unique_ptr, so we
/// provide bridge functions rather than exposing the type directly.

/// Check if a validator with the given name exists in the registry.
USDVALIDATION_API
bool UsdValidation_Swift_HasValidator(const TfToken &validatorName);

/// Check if a validator suite with the given name exists in the registry.
USDVALIDATION_API
bool UsdValidation_Swift_HasValidatorSuite(const TfToken &suiteName);

/// Get metadata for a validator by name.
/// Returns true if found, false otherwise.
/// The metadata parameter is filled if found.
USDVALIDATION_API
bool UsdValidation_Swift_GetValidatorMetadata(
    const TfToken &validatorName,
    UsdValidationValidatorMetadata *outMetadata);

/// Get all validator metadata registered in the registry.
USDVALIDATION_API
UsdValidationValidatorMetadataVector UsdValidation_Swift_GetAllValidatorMetadata();

/// Get validator metadata for validators matching the given keyword.
USDVALIDATION_API
UsdValidationValidatorMetadataVector UsdValidation_Swift_GetValidatorMetadataForKeyword(
    const TfToken &keyword);

/// Get validator metadata for validators matching any of the given keywords.
USDVALIDATION_API
UsdValidationValidatorMetadataVector UsdValidation_Swift_GetValidatorMetadataForKeywords(
    const TfTokenVector &keywords);

/// Get validator metadata for validators belonging to the given plugin.
USDVALIDATION_API
UsdValidationValidatorMetadataVector UsdValidation_Swift_GetValidatorMetadataForPlugin(
    const TfToken &pluginName);

/// Get validator metadata for validators matching the given schema type.
USDVALIDATION_API
UsdValidationValidatorMetadataVector UsdValidation_Swift_GetValidatorMetadataForSchemaType(
    const TfToken &schemaType);

/// @}

/// @{
/// \name UsdValidationValidator Swift Bridge Functions
///
/// Functions to work with validators. Since UsdValidationValidator contains
/// std::variant and std::function, we use opaque pointers.

/// Get or load a validator by name from the registry.
/// Returns an opaque pointer to the validator (owned by registry, do NOT destroy).
/// Returns nullptr if not found.
USDVALIDATION_API
const void* UsdValidation_Swift_GetOrLoadValidatorByName(const TfToken &validatorName);

/// Get metadata from a validator pointer.
/// The validator parameter must be a valid pointer from UsdValidation_Swift_GetOrLoadValidatorByName.
USDVALIDATION_API
UsdValidationValidatorMetadata UsdValidation_Swift_GetValidatorMetadataFromValidator(
    const void *validator);

/// Run a validator directly on a layer.
/// Returns a vector of validation errors.
USDVALIDATION_API
UsdValidationErrorVector UsdValidation_Swift_ValidatorValidateLayer(
    const void *validator,
    const SdfLayerHandle &layer);

/// Run a validator directly on a stage.
/// Returns a vector of validation errors.
USDVALIDATION_API
UsdValidationErrorVector UsdValidation_Swift_ValidatorValidateStage(
    const void *validator,
    const UsdStageRefPtr &stage,
    const UsdValidationTimeRange &timeRange);

/// Run a validator directly on a prim.
/// Returns a vector of validation errors.
USDVALIDATION_API
UsdValidationErrorVector UsdValidation_Swift_ValidatorValidatePrim(
    const void *validator,
    const UsdPrim &prim,
    const UsdValidationTimeRange &timeRange);

/// @}

/// @{
/// \name UsdValidationValidatorSuite Swift Bridge Functions
///
/// Functions to work with validator suites.

/// Get or load a validator suite by name from the registry.
/// Returns an opaque pointer to the suite (owned by registry, do NOT destroy).
/// Returns nullptr if not found.
USDVALIDATION_API
const void* UsdValidation_Swift_GetOrLoadValidatorSuiteByName(const TfToken &suiteName);

/// Get metadata from a validator suite pointer.
USDVALIDATION_API
UsdValidationValidatorMetadata UsdValidation_Swift_GetValidatorSuiteMetadata(
    const void *suite);

/// Get the number of contained validators in a suite.
USDVALIDATION_API
size_t UsdValidation_Swift_GetValidatorSuiteValidatorCount(const void *suite);

/// @}

/// @{
/// \name Helper Functions
///
/// Utility functions for working with validation from Swift.

/// Create a TfTokenVector from an array of C strings.
/// This helper makes it easier to call from Swift without std::vector issues.
USDVALIDATION_API
TfTokenVector UsdValidation_Swift_CreateTokenVector(
    const char* const* tokens,
    size_t count);

/// @}

PXR_NAMESPACE_CLOSE_SCOPE

#endif // PXR_USD_VALIDATION_USD_VALIDATION_SWIFT_BRIDGE_H
