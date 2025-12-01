#ifndef __PXR_BASE_TRACE_H__
#define __PXR_BASE_TRACE_H__

// Tf umbrella provides all Tf dependencies needed by Trace headers
// (TfPyTraceFnId, TfToken, TfRefPtr, TfWeakPtr, etc.)
#include <Tf/Tf.h>

// Trace
#include <Trace/api.h>

#include <Trace/aggregateNode.h>
#include <Trace/aggregateTree.h>
#include <Trace/aggregateTreeBuilder.h>
#include <Trace/category.h>
#include <Trace/collection.h>
#include <Trace/collectionNotice.h>
#include <Trace/collector.h>
#include <Trace/concurrentList.h>
#include <Trace/counterAccumulator.h>
#include <Trace/dataBuffer.h>
#include <Trace/dynamicKey.h>
#include <Trace/event.h>
#include <Trace/eventContainer.h>
#include <Trace/eventData.h>
#include <Trace/eventList.h>
#include <Trace/eventNode.h>
#include <Trace/eventTree.h>
#include <Trace/eventTreeBuilder.h>
#include <Trace/jsonSerialization.h>
#include <Trace/key.h>
#include <Trace/reporter.h>
#include <Trace/reporterBase.h>
#include <Trace/reporterDataSourceBase.h>
#include <Trace/reporterDataSourceCollection.h>
#include <Trace/reporterDataSourceCollector.h>
#include <Trace/serialization.h>
#include <Trace/staticKeyData.h>
#include <Trace/stringHash.h>
#include <Trace/threads.h>
#include <Trace/traceImpl.h>

#endif // __PXR_BASE_TRACE_H__
