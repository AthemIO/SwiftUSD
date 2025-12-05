#ifndef __PXR_USDVALIDATION_USDVALIDATION_H__
#define __PXR_USDVALIDATION_USDVALIDATION_H__

// UsdValidation
#include <UsdValidation/api.h>
#include <UsdValidation/error.h>
#include <UsdValidation/timeRange.h>
#include <UsdValidation/validatorTokens.h>

// validator.h is included for:
// - UsdValidationValidatorMetadata (plain struct, exports cleanly)
// - UsdValidationValidatorMetadataVector (std::vector, exports cleanly)
// - UsdValidationErrorVector (std::vector, exports cleanly)
// Note: UsdValidationValidator itself may not export due to std::variant<std::function...>
#include <UsdValidation/validator.h>

// Swift Bridge - provides access to types that don't export cleanly to Swift
// (Context, Registry, Validator, ValidatorSuite, Fixer use std::function/variant)
#include <UsdValidation/swiftBridge.h>

// Note: The following headers are NOT included in the umbrella because they
// contain types that crash Swift's C++ interop (std::function, std::variant,
// std::unique_ptr, TfSingleton). Use the swiftBridge.h functions instead.
//
// Excluded headers:
// - context.h (UsdValidationContext uses std::variant internally)
// - registry.h (UsdValidationRegistry uses TfSingleton + std::unique_ptr)
// - fixer.h (UsdValidationFixer uses std::function)

#endif // __PXR_USDVALIDATION_USDVALIDATION_H__
