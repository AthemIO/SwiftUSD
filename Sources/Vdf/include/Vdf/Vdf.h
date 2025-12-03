/* ----------------------------------------------------------------
 * :: :  M  E  T  A  V  E  R  S  E  :                            ::
 * ----------------------------------------------------------------
 * This software is Licensed under the terms of the Apache License,
 * version 2.0 (the "Apache License") with the following additional
 * modification; you may not use this file except within compliance
 * of the Apache License and the following modification made to it.
 * Section 6. Trademarks. is deleted and replaced with:
 *
 * Trademarks. This License does not grant permission to use any of
 * its trade names, trademarks, service marks, or the product names
 * of this Licensor or its affiliates, except as required to comply
 * with Section 4(c.) of this License, and to reproduce the content
 * of the NOTICE file.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND without even an
 * implied warranty of MERCHANTABILITY, or FITNESS FOR A PARTICULAR
 * PURPOSE. See the Apache License for more details.
 *
 * You should have received a copy for this software license of the
 * Apache License along with this program; or, if not, please write
 * to the Free Software Foundation Inc., with the following address
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 *         Copyright (C) 2024 Wabi Foundation. All Rights Reserved.
 * ----------------------------------------------------------------
 *  . x x x . o o o . x x x . : : : .    o  x  o    . : : : .
 * ---------------------------------------------------------------- */

#ifndef __PXR_EXEC_VDF_H__
#define __PXR_EXEC_VDF_H__

// Vdf (Vectorized Data Flow) umbrella header for Swift module builds
// Foundation for building and evaluating vectorized dataflow networks

// API and core types
#include <Vdf/api.h>
#include <Vdf/types.h>
#include <Vdf/tokens.h>
#include <Vdf/debugCodes.h>
#include <Vdf/error.h>

// Container traits and utilities
#include <Vdf/traits.h>
#include <Vdf/boxedContainer.h>
#include <Vdf/boxedContainerTraits.h>
#include <Vdf/connectorMap.h>
#include <Vdf/estimateSize.h>
#include <Vdf/forEachCommonType.h>

// Iterator types
#include <Vdf/countingIterator.h>
#include <Vdf/iterator.h>
#include <Vdf/iteratorRange.h>
#include <Vdf/iterators.h>
#include <Vdf/readIterator.h>
#include <Vdf/readIteratorRange.h>
#include <Vdf/readWriteIterator.h>
#include <Vdf/readWriteIteratorRange.h>
#include <Vdf/maskedIterator.h>
#include <Vdf/weightedIterator.h>

// Vector types
#include <Vdf/vector.h>
#include <Vdf/vectorAccessor.h>
#include <Vdf/vectorData.h>
#include <Vdf/vectorDataTyped.h>
#include <Vdf/typedVector.h>
#include <Vdf/vectorImpl_Boxed.h>
#include <Vdf/vectorImpl_Compressed.h>
#include <Vdf/vectorImpl_Contiguous.h>
#include <Vdf/vectorImpl_Dispatch.h>
#include <Vdf/vectorImpl_Empty.h>
#include <Vdf/vectorImpl_Shared.h>
#include <Vdf/vectorImpl_Single.h>
#include <Vdf/vectorSubrangeAccessor.h>

// Data structures
#include <Vdf/allocateBoxedValue.h>
#include <Vdf/compressedIndexMapping.h>
#include <Vdf/indexedData.h>
#include <Vdf/indexedDataIterator.h>
#include <Vdf/indexedWeights.h>
#include <Vdf/indexedWeightsOperand.h>
#include <Vdf/lruCache.h>
#include <Vdf/mask.h>
#include <Vdf/maskMemoizer.h>
#include <Vdf/subrangeView.h>

// Input/Output specs
#include <Vdf/input.h>
#include <Vdf/inputSpec.h>
#include <Vdf/inputVector.h>
#include <Vdf/inputValuesPointer.h>
#include <Vdf/inputAndOutputSpecs.h>
#include <Vdf/output.h>
#include <Vdf/outputSpec.h>
#include <Vdf/maskedOutput.h>
#include <Vdf/maskedOutputVector.h>

// Network components
#include <Vdf/connection.h>
#include <Vdf/connectorSpecs.h>
#include <Vdf/context.h>
#include <Vdf/network.h>
#include <Vdf/networkStats.h>
#include <Vdf/networkUtil.h>
#include <Vdf/node.h>
#include <Vdf/nodeSet.h>
#include <Vdf/object.h>
#include <Vdf/rootNode.h>
#include <Vdf/extensibleNode.h>
#include <Vdf/speculationNode.h>

// Scheduling
#include <Vdf/schedule.h>
#include <Vdf/scheduleNode.h>
#include <Vdf/scheduleTasks.h>
#include <Vdf/scheduler.h>

// Execution infrastructure
#include <Vdf/evaluationState.h>
#include <Vdf/executionStats.h>
#include <Vdf/executionStatsProcessor.h>
#include <Vdf/executionTypeRegistry.h>
#include <Vdf/executorInterface.h>
#include <Vdf/executorBufferData.h>
#include <Vdf/executorDataVector.h>
#include <Vdf/executorErrorLogger.h>
#include <Vdf/executorInvalidationData.h>
#include <Vdf/executorInvalidator.h>
#include <Vdf/executorObserver.h>
#include <Vdf/executorFactory.h>
#include <Vdf/executorFactoryBase.h>

// Data manager infrastructure
#include <Vdf/dataManagerBasedExecutor.h>
#include <Vdf/dataManagerBasedSubExecutor.h>
#include <Vdf/dataManagerFacade.h>
#include <Vdf/dataManagerHashTable.h>
#include <Vdf/dataManagerVector.h>
#include <Vdf/datalessExecutor.h>
#include <Vdf/defaultInitAllocator.h>
#include <Vdf/executorDataManager.h>
#include <Vdf/executorDataManagerInterface.h>
#include <Vdf/fixedSizeHolder.h>
#include <Vdf/fixedSizePolymorphicHolder.h>

// Parallel execution
#include <Vdf/parallelDataManagerVector.h>
#include <Vdf/parallelExecutorDataManager.h>
#include <Vdf/parallelExecutorDataManagerInterface.h>
#include <Vdf/parallelExecutorDataVector.h>
#include <Vdf/parallelExecutorEngine.h>
#include <Vdf/parallelExecutorEngineBase.h>
#include <Vdf/parallelSpeculationExecutorEngine.h>
#include <Vdf/parallelTaskSync.h>
#include <Vdf/parallelTaskWaitlist.h>
#include <Vdf/pullBasedExecutorEngine.h>

// Speculation execution
#include <Vdf/speculationExecutor.h>
#include <Vdf/speculationExecutorBase.h>
#include <Vdf/speculationExecutorEngine.h>

// Simple/basic execution
#include <Vdf/simpleExecutor.h>

// Traversal utilities
#include <Vdf/sparseInputPathFinder.h>
#include <Vdf/sparseInputTraverser.h>
#include <Vdf/sparseOutputTraverser.h>
#include <Vdf/sparseVectorizedInputTraverser.h>
#include <Vdf/sparseVectorizedOutputTraverser.h>

// Request handling
#include <Vdf/request.h>
#include <Vdf/requiredInputsPredicate.h>

// Graphing/visualization
#include <Vdf/grapher.h>
#include <Vdf/grapherOptions.h>

// Isolated network support
#include <Vdf/isolatedSubnetwork.h>

// Raw value access
#include <Vdf/rawValueAccessor.h>
#include <Vdf/readWriteAccessor.h>

// Node invalidation
#include <Vdf/nodeProcessInvalidationInterface.h>

// Pool chain indexing
#include <Vdf/poolChainIndex.h>

// Debug names
#include <Vdf/execNodeDebugName.h>

// SMBL data
#include <Vdf/smblData.h>

// Test utilities
#include <Vdf/testUtils.h>

#endif  // __PXR_EXEC_VDF_H__
