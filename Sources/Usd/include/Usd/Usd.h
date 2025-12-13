#ifndef __PXR_USD_USD_H__
#define __PXR_USD_USD_H__

// usd.
#include <Usd/api.h>
#include <Usd/apiSchemaBase.h>
#include <Usd/attribute.h>
#include <Usd/attributeQuery.h>
#include <Usd/clip.h>
#include <Usd/clipCache.h>
#include <Usd/clipSet.h>
#include <Usd/clipSetDefinition.h>
#include <Usd/clipsAPI.h>
#include <Usd/collectionAPI.h>
#include <Usd/collectionMembershipQuery.h>
#include <Usd/common.h>
#include <Usd/crateData.h>
#include <Usd/crateFile.h>
#include <Usd/crateValueInliners.h>
#include <Usd/debugCodes.h>
#include <Usd/editContext.h>
#include <Usd/editTarget.h>
#include <Usd/errors.h>
#include <Usd/flattenUtils.h>
#include <Usd/inherits.h>
#include <Usd/instanceCache.h>
#include <Usd/instanceKey.h>
#include <Usd/integerCoding.h>
#include <Usd/interpolation.h>
#include <Usd/interpolators.h>
#include <Usd/listEditImpl.h>
#include <Usd/modelAPI.h>
#include <Usd/notice.h>
#include <Usd/object.h>
#include <Usd/payloads.h>
#include <Usd/prim.h>
#include <Usd/primCompositionQuery.h>
#include <Usd/primData.h>
#include <Usd/primDataHandle.h>
#include <Usd/primDefinition.h>
#include <Usd/primFlags.h>
#include <Usd/primRange.h>
#include <Usd/primTypeInfo.h>
#include <Usd/primTypeInfoCache.h>
#include <Usd/property.h>
#include <Usd/references.h>
#include <Usd/relationship.h>
#include <Usd/resolveInfo.h>
#include <Usd/resolveTarget.h>
#include <Usd/resolver.h>
#include <Usd/schemaBase.h>
#include <Usd/schemaRegistry.h>
#include <Usd/shared.h>
#include <Usd/specializes.h>
#include <Usd/stageCache.h>
// Excluded: Uses TF_DEFINE_STACKED macro which is incompatible with Swift modules
// #include <Usd/stageCacheContext.h>
#include <Usd/stageLoadRules.h>
#include <Usd/stagePopulationMask.h>
#include <Usd/timeCode.h>
#include <Usd/tokens.h>
#include <Usd/typed.h>
#include <Usd/valueUtils.h>
#include <Usd/variantSets.h>

#include <Usd/stage.h>

#include <Usd/collectionPredicateLibrary.h>
#include <Usd/crateDataTypes.h>
#include <Usd/namespaceEditor.h>
// Validation moved to UsdValidation module in OpenUSD 25.11
// #include <UsdValidation/UsdValidation.h>
#include <Usd/attributeLimits.h>
#include <Usd/colorSpaceAPI.h>
// Excluded: Python TF_WRAP macros - only used for Python bindings
// #include <Usd/generatedSchema.module.h>
#include <Usd/colorSpaceDefinitionAPI.h>

// Swift interoperability bridge functions
#include <Usd/swiftBridge.h>

// Python-related headers - only include when Python support is enabled
#if PXR_PYTHON_SUPPORT_ENABLED
#include <Usd/pyConversions.h>
#include <Usd/pyEditContext.h>
#include <Usd/wrapUtils.h>
#endif // PXR_PYTHON_SUPPORT_ENABLED

#endif  // __PXR_USD_USD_H__
