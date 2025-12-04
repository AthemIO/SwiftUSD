//
// Copyright 2018 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
// MaterialX Swift Bridge - Provides Swift-compatible wrappers for MaterialX
// shared_ptr types using SWIFT_SHARED_REFERENCE.
//

#ifndef PXR_USD_USDMTLX_SWIFT_BRIDGE_H
#define PXR_USD_USDMTLX_SWIFT_BRIDGE_H

#include "pxr/pxrns.h"
#include "Arch/swiftInterop.h"
#include "UsdMtlx/api.h"
#include "UsdMtlx/utils.h"
#include "UsdMtlx/reader.h"
#include "Sdf/path.h"
#include "Usd/stage.h"
#include <MaterialX/MXCoreDocument.h>
#include <MaterialX/MXCoreElement.h>
#include <MaterialX/MXCoreNode.h>
#include <string>

// Forward declarations for retain/release functions (outside namespace for Swift)
namespace PXR_INTERNAL_NS {
    class MxDocumentHandle;
    class MxConstDocumentHandle;
    class MxElementHandle;
    class MxConstElementHandle;
    class MxNodeDefHandle;
}

// Retain/release functions declared outside namespace for Swift interop
void UsdMtlxRetainMxDocument(PXR_NS::MxDocumentHandle* handle);
void UsdMtlxReleaseMxDocument(PXR_NS::MxDocumentHandle* handle);
void UsdMtlxRetainMxConstDocument(PXR_NS::MxConstDocumentHandle* handle);
void UsdMtlxReleaseMxConstDocument(PXR_NS::MxConstDocumentHandle* handle);
void UsdMtlxRetainMxElement(PXR_NS::MxElementHandle* handle);
void UsdMtlxReleaseMxElement(PXR_NS::MxElementHandle* handle);
void UsdMtlxRetainMxConstElement(PXR_NS::MxConstElementHandle* handle);
void UsdMtlxReleaseMxConstElement(PXR_NS::MxConstElementHandle* handle);
void UsdMtlxRetainMxNodeDef(PXR_NS::MxNodeDefHandle* handle);
void UsdMtlxReleaseMxNodeDef(PXR_NS::MxNodeDefHandle* handle);

PXR_NAMESPACE_OPEN_SCOPE

/// \class MxDocumentHandle
/// Swift-compatible handle wrapper for MaterialX::DocumentPtr.
///
/// This class wraps a std::shared_ptr<MaterialX::Document> and is annotated
/// with SWIFT_SHARED_REFERENCE so Swift can manage its lifetime through
/// automatic reference counting.
class SWIFT_SHARED_REFERENCE(UsdMtlxRetainMxDocument, UsdMtlxReleaseMxDocument)
MxDocumentHandle
{
public:
    /// Construct an empty (null) handle.
    USDMTLX_API
    MxDocumentHandle();

    /// Construct from a MaterialX::DocumentPtr.
    USDMTLX_API
    explicit MxDocumentHandle(const MaterialX::DocumentPtr& ptr);

    /// Copy constructor - increments reference count.
    USDMTLX_API
    MxDocumentHandle(const MxDocumentHandle& other);

    /// Move constructor.
    USDMTLX_API
    MxDocumentHandle(MxDocumentHandle&& other) noexcept;

    /// Destructor.
    USDMTLX_API
    ~MxDocumentHandle();

    /// Copy assignment.
    USDMTLX_API
    MxDocumentHandle& operator=(const MxDocumentHandle& other);

    /// Move assignment.
    USDMTLX_API
    MxDocumentHandle& operator=(MxDocumentHandle&& other) noexcept;

    /// Check if the handle is valid (non-null).
    USDMTLX_API
    bool IsValid() const;

    /// Get the underlying DocumentPtr.
    USDMTLX_API
    const MaterialX::DocumentPtr& GetPtr() const;

    /// Get the document name.
    USDMTLX_API
    std::string GetName() const;

    /// Export the document to XML string.
    USDMTLX_API
    std::string ExportToXmlString() const;

private:
    friend void ::UsdMtlxRetainMxDocument(MxDocumentHandle* handle);
    friend void ::UsdMtlxReleaseMxDocument(MxDocumentHandle* handle);

    MaterialX::DocumentPtr _ptr;
    std::atomic<int> _refCount{1};
};

/// \class MxConstDocumentHandle
/// Swift-compatible handle wrapper for MaterialX::ConstDocumentPtr.
class SWIFT_SHARED_REFERENCE(UsdMtlxRetainMxConstDocument, UsdMtlxReleaseMxConstDocument)
MxConstDocumentHandle
{
public:
    /// Construct an empty (null) handle.
    USDMTLX_API
    MxConstDocumentHandle();

    /// Construct from a MaterialX::ConstDocumentPtr.
    USDMTLX_API
    explicit MxConstDocumentHandle(const MaterialX::ConstDocumentPtr& ptr);

    /// Copy constructor.
    USDMTLX_API
    MxConstDocumentHandle(const MxConstDocumentHandle& other);

    /// Move constructor.
    USDMTLX_API
    MxConstDocumentHandle(MxConstDocumentHandle&& other) noexcept;

    /// Destructor.
    USDMTLX_API
    ~MxConstDocumentHandle();

    /// Copy assignment.
    USDMTLX_API
    MxConstDocumentHandle& operator=(const MxConstDocumentHandle& other);

    /// Move assignment.
    USDMTLX_API
    MxConstDocumentHandle& operator=(MxConstDocumentHandle&& other) noexcept;

    /// Check if the handle is valid (non-null).
    USDMTLX_API
    bool IsValid() const;

    /// Get the underlying ConstDocumentPtr.
    USDMTLX_API
    const MaterialX::ConstDocumentPtr& GetPtr() const;

    /// Get the document name.
    USDMTLX_API
    std::string GetName() const;

private:
    friend void ::UsdMtlxRetainMxConstDocument(MxConstDocumentHandle* handle);
    friend void ::UsdMtlxReleaseMxConstDocument(MxConstDocumentHandle* handle);

    MaterialX::ConstDocumentPtr _ptr;
    std::atomic<int> _refCount{1};
};

/// \class MxElementHandle
/// Swift-compatible handle wrapper for MaterialX::ElementPtr.
class SWIFT_SHARED_REFERENCE(UsdMtlxRetainMxElement, UsdMtlxReleaseMxElement)
MxElementHandle
{
public:
    USDMTLX_API
    MxElementHandle();

    USDMTLX_API
    explicit MxElementHandle(const MaterialX::ElementPtr& ptr);

    USDMTLX_API
    MxElementHandle(const MxElementHandle& other);

    USDMTLX_API
    MxElementHandle(MxElementHandle&& other) noexcept;

    USDMTLX_API
    ~MxElementHandle();

    USDMTLX_API
    MxElementHandle& operator=(const MxElementHandle& other);

    USDMTLX_API
    MxElementHandle& operator=(MxElementHandle&& other) noexcept;

    USDMTLX_API
    bool IsValid() const;

    USDMTLX_API
    const MaterialX::ElementPtr& GetPtr() const;

    USDMTLX_API
    std::string GetName() const;

    USDMTLX_API
    std::string GetCategory() const;

private:
    friend void ::UsdMtlxRetainMxElement(MxElementHandle* handle);
    friend void ::UsdMtlxReleaseMxElement(MxElementHandle* handle);

    MaterialX::ElementPtr _ptr;
    std::atomic<int> _refCount{1};
};

/// \class MxConstElementHandle
/// Swift-compatible handle wrapper for MaterialX::ConstElementPtr.
class SWIFT_SHARED_REFERENCE(UsdMtlxRetainMxConstElement, UsdMtlxReleaseMxConstElement)
MxConstElementHandle
{
public:
    USDMTLX_API
    MxConstElementHandle();

    USDMTLX_API
    explicit MxConstElementHandle(const MaterialX::ConstElementPtr& ptr);

    USDMTLX_API
    MxConstElementHandle(const MxConstElementHandle& other);

    USDMTLX_API
    MxConstElementHandle(MxConstElementHandle&& other) noexcept;

    USDMTLX_API
    ~MxConstElementHandle();

    USDMTLX_API
    MxConstElementHandle& operator=(const MxConstElementHandle& other);

    USDMTLX_API
    MxConstElementHandle& operator=(MxConstElementHandle&& other) noexcept;

    USDMTLX_API
    bool IsValid() const;

    USDMTLX_API
    const MaterialX::ConstElementPtr& GetPtr() const;

    USDMTLX_API
    std::string GetName() const;

    USDMTLX_API
    std::string GetCategory() const;

private:
    friend void ::UsdMtlxRetainMxConstElement(MxConstElementHandle* handle);
    friend void ::UsdMtlxReleaseMxConstElement(MxConstElementHandle* handle);

    MaterialX::ConstElementPtr _ptr;
    std::atomic<int> _refCount{1};
};

/// \class MxNodeDefHandle
/// Swift-compatible handle wrapper for MaterialX::NodeDefPtr.
class SWIFT_SHARED_REFERENCE(UsdMtlxRetainMxNodeDef, UsdMtlxReleaseMxNodeDef)
MxNodeDefHandle
{
public:
    USDMTLX_API
    MxNodeDefHandle();

    USDMTLX_API
    explicit MxNodeDefHandle(const MaterialX::NodeDefPtr& ptr);

    USDMTLX_API
    MxNodeDefHandle(const MxNodeDefHandle& other);

    USDMTLX_API
    MxNodeDefHandle(MxNodeDefHandle&& other) noexcept;

    USDMTLX_API
    ~MxNodeDefHandle();

    USDMTLX_API
    MxNodeDefHandle& operator=(const MxNodeDefHandle& other);

    USDMTLX_API
    MxNodeDefHandle& operator=(MxNodeDefHandle&& other) noexcept;

    USDMTLX_API
    bool IsValid() const;

    USDMTLX_API
    const MaterialX::NodeDefPtr& GetPtr() const;

    USDMTLX_API
    std::string GetName() const;

    USDMTLX_API
    std::string GetNodeString() const;

private:
    friend void ::UsdMtlxRetainMxNodeDef(MxNodeDefHandle* handle);
    friend void ::UsdMtlxReleaseMxNodeDef(MxNodeDefHandle* handle);

    MaterialX::NodeDefPtr _ptr;
    std::atomic<int> _refCount{1};
};

// ============================================================================
// Swift-Friendly Factory Functions
// ============================================================================

/// Read a MaterialX document from a file path.
/// Returns a Swift-compatible handle wrapping the DocumentPtr.
USDMTLX_API
SWIFT_RETURNS_RETAINED
MxDocumentHandle*
UsdMtlxSwiftReadDocument(const std::string& resolvedPath);

/// Get a (possibly cached) MaterialX document by URI.
/// Returns a Swift-compatible handle wrapping the ConstDocumentPtr.
USDMTLX_API
SWIFT_RETURNS_RETAINED
MxConstDocumentHandle*
UsdMtlxSwiftGetDocument(const std::string& resolvedUri);

/// Get a MaterialX document from an XML string.
/// Returns a Swift-compatible handle wrapping the ConstDocumentPtr.
USDMTLX_API
SWIFT_RETURNS_RETAINED
MxConstDocumentHandle*
UsdMtlxSwiftGetDocumentFromString(const std::string& mtlxXml);

/// Create a new empty MaterialX document.
USDMTLX_API
SWIFT_RETURNS_RETAINED
MxDocumentHandle*
UsdMtlxSwiftCreateDocument();

// ============================================================================
// Swift-Friendly Document Operations
// ============================================================================

/// Read a MaterialX document into a USD stage.
/// This wraps UsdMtlxRead() for Swift compatibility.
USDMTLX_API
void
UsdMtlxSwiftRead(
    const MxConstDocumentHandle* mtlxHandle,
    const UsdStageRefPtr& stage,
    const SdfPath& internalPath = SdfPath("/MaterialX"),
    const SdfPath& externalPath = SdfPath("/ModelRoot"));

/// Read MaterialX node graphs into a USD stage.
/// This wraps UsdMtlxReadNodeGraphs() for Swift compatibility.
USDMTLX_API
void
UsdMtlxSwiftReadNodeGraphs(
    const MxConstDocumentHandle* mtlxHandle,
    const UsdStageRefPtr& stage,
    const SdfPath& internalPath = SdfPath("/MaterialX"));

/// Get the source URI for a MaterialX element.
USDMTLX_API
std::string
UsdMtlxSwiftGetSourceURI(const MxConstElementHandle* elementHandle);

/// Get the value of a MaterialX element as a VtValue.
USDMTLX_API
VtValue
UsdMtlxSwiftGetUsdValue(
    const MxConstElementHandle* elementHandle,
    bool getDefaultValue = false);

PXR_NAMESPACE_CLOSE_SCOPE

#endif // PXR_USD_USDMTLX_SWIFT_BRIDGE_H
