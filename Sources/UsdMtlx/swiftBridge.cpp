//
// Copyright 2018 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
// MaterialX Swift Bridge Implementation
//

#include "UsdMtlx/swiftBridge.h"
#include "UsdMtlx/utils.h"
#include "UsdMtlx/reader.h"
#include <MaterialX/MXFormatXmlIo.h>

// ============================================================================
// Retain/Release Functions for Swift Reference Counting
// (Defined outside namespace to match header declarations)
// ============================================================================

void UsdMtlxRetainMxDocument(PXR_NS::MxDocumentHandle* handle)
{
    if (handle) {
        handle->_refCount.fetch_add(1, std::memory_order_relaxed);
    }
}

void UsdMtlxReleaseMxDocument(PXR_NS::MxDocumentHandle* handle)
{
    if (handle) {
        if (handle->_refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) {
            delete handle;
        }
    }
}

void UsdMtlxRetainMxConstDocument(PXR_NS::MxConstDocumentHandle* handle)
{
    if (handle) {
        handle->_refCount.fetch_add(1, std::memory_order_relaxed);
    }
}

void UsdMtlxReleaseMxConstDocument(PXR_NS::MxConstDocumentHandle* handle)
{
    if (handle) {
        if (handle->_refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) {
            delete handle;
        }
    }
}

void UsdMtlxRetainMxElement(PXR_NS::MxElementHandle* handle)
{
    if (handle) {
        handle->_refCount.fetch_add(1, std::memory_order_relaxed);
    }
}

void UsdMtlxReleaseMxElement(PXR_NS::MxElementHandle* handle)
{
    if (handle) {
        if (handle->_refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) {
            delete handle;
        }
    }
}

void UsdMtlxRetainMxConstElement(PXR_NS::MxConstElementHandle* handle)
{
    if (handle) {
        handle->_refCount.fetch_add(1, std::memory_order_relaxed);
    }
}

void UsdMtlxReleaseMxConstElement(PXR_NS::MxConstElementHandle* handle)
{
    if (handle) {
        if (handle->_refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) {
            delete handle;
        }
    }
}

void UsdMtlxRetainMxNodeDef(PXR_NS::MxNodeDefHandle* handle)
{
    if (handle) {
        handle->_refCount.fetch_add(1, std::memory_order_relaxed);
    }
}

void UsdMtlxReleaseMxNodeDef(PXR_NS::MxNodeDefHandle* handle)
{
    if (handle) {
        if (handle->_refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) {
            delete handle;
        }
    }
}

PXR_NAMESPACE_OPEN_SCOPE

// ============================================================================
// MxDocumentHandle Implementation
// ============================================================================

MxDocumentHandle::MxDocumentHandle()
    : _ptr(nullptr), _refCount(1)
{
}

MxDocumentHandle::MxDocumentHandle(const MaterialX::DocumentPtr& ptr)
    : _ptr(ptr), _refCount(1)
{
}

MxDocumentHandle::MxDocumentHandle(const MxDocumentHandle& other)
    : _ptr(other._ptr), _refCount(1)
{
}

MxDocumentHandle::MxDocumentHandle(MxDocumentHandle&& other) noexcept
    : _ptr(std::move(other._ptr)), _refCount(1)
{
}

MxDocumentHandle::~MxDocumentHandle()
{
    _ptr.reset();
}

MxDocumentHandle& MxDocumentHandle::operator=(const MxDocumentHandle& other)
{
    if (this != &other) {
        _ptr = other._ptr;
    }
    return *this;
}

MxDocumentHandle& MxDocumentHandle::operator=(MxDocumentHandle&& other) noexcept
{
    if (this != &other) {
        _ptr = std::move(other._ptr);
    }
    return *this;
}

bool MxDocumentHandle::IsValid() const
{
    return _ptr != nullptr;
}

const MaterialX::DocumentPtr& MxDocumentHandle::GetPtr() const
{
    return _ptr;
}

std::string MxDocumentHandle::GetName() const
{
    if (_ptr) {
        return _ptr->getName();
    }
    return std::string();
}

std::string MxDocumentHandle::ExportToXmlString() const
{
    if (_ptr) {
        return MaterialX::writeToXmlString(_ptr);
    }
    return std::string();
}

// ============================================================================
// MxConstDocumentHandle Implementation
// ============================================================================

MxConstDocumentHandle::MxConstDocumentHandle()
    : _ptr(nullptr), _refCount(1)
{
}

MxConstDocumentHandle::MxConstDocumentHandle(const MaterialX::ConstDocumentPtr& ptr)
    : _ptr(ptr), _refCount(1)
{
}

MxConstDocumentHandle::MxConstDocumentHandle(const MxConstDocumentHandle& other)
    : _ptr(other._ptr), _refCount(1)
{
}

MxConstDocumentHandle::MxConstDocumentHandle(MxConstDocumentHandle&& other) noexcept
    : _ptr(std::move(other._ptr)), _refCount(1)
{
}

MxConstDocumentHandle::~MxConstDocumentHandle()
{
    _ptr.reset();
}

MxConstDocumentHandle& MxConstDocumentHandle::operator=(const MxConstDocumentHandle& other)
{
    if (this != &other) {
        _ptr = other._ptr;
    }
    return *this;
}

MxConstDocumentHandle& MxConstDocumentHandle::operator=(MxConstDocumentHandle&& other) noexcept
{
    if (this != &other) {
        _ptr = std::move(other._ptr);
    }
    return *this;
}

bool MxConstDocumentHandle::IsValid() const
{
    return _ptr != nullptr;
}

const MaterialX::ConstDocumentPtr& MxConstDocumentHandle::GetPtr() const
{
    return _ptr;
}

std::string MxConstDocumentHandle::GetName() const
{
    if (_ptr) {
        return _ptr->getName();
    }
    return std::string();
}

// ============================================================================
// MxElementHandle Implementation
// ============================================================================

MxElementHandle::MxElementHandle()
    : _ptr(nullptr), _refCount(1)
{
}

MxElementHandle::MxElementHandle(const MaterialX::ElementPtr& ptr)
    : _ptr(ptr), _refCount(1)
{
}

MxElementHandle::MxElementHandle(const MxElementHandle& other)
    : _ptr(other._ptr), _refCount(1)
{
}

MxElementHandle::MxElementHandle(MxElementHandle&& other) noexcept
    : _ptr(std::move(other._ptr)), _refCount(1)
{
}

MxElementHandle::~MxElementHandle()
{
    _ptr.reset();
}

MxElementHandle& MxElementHandle::operator=(const MxElementHandle& other)
{
    if (this != &other) {
        _ptr = other._ptr;
    }
    return *this;
}

MxElementHandle& MxElementHandle::operator=(MxElementHandle&& other) noexcept
{
    if (this != &other) {
        _ptr = std::move(other._ptr);
    }
    return *this;
}

bool MxElementHandle::IsValid() const
{
    return _ptr != nullptr;
}

const MaterialX::ElementPtr& MxElementHandle::GetPtr() const
{
    return _ptr;
}

std::string MxElementHandle::GetName() const
{
    if (_ptr) {
        return _ptr->getName();
    }
    return std::string();
}

std::string MxElementHandle::GetCategory() const
{
    if (_ptr) {
        return _ptr->getCategory();
    }
    return std::string();
}

// ============================================================================
// MxConstElementHandle Implementation
// ============================================================================

MxConstElementHandle::MxConstElementHandle()
    : _ptr(nullptr), _refCount(1)
{
}

MxConstElementHandle::MxConstElementHandle(const MaterialX::ConstElementPtr& ptr)
    : _ptr(ptr), _refCount(1)
{
}

MxConstElementHandle::MxConstElementHandle(const MxConstElementHandle& other)
    : _ptr(other._ptr), _refCount(1)
{
}

MxConstElementHandle::MxConstElementHandle(MxConstElementHandle&& other) noexcept
    : _ptr(std::move(other._ptr)), _refCount(1)
{
}

MxConstElementHandle::~MxConstElementHandle()
{
    _ptr.reset();
}

MxConstElementHandle& MxConstElementHandle::operator=(const MxConstElementHandle& other)
{
    if (this != &other) {
        _ptr = other._ptr;
    }
    return *this;
}

MxConstElementHandle& MxConstElementHandle::operator=(MxConstElementHandle&& other) noexcept
{
    if (this != &other) {
        _ptr = std::move(other._ptr);
    }
    return *this;
}

bool MxConstElementHandle::IsValid() const
{
    return _ptr != nullptr;
}

const MaterialX::ConstElementPtr& MxConstElementHandle::GetPtr() const
{
    return _ptr;
}

std::string MxConstElementHandle::GetName() const
{
    if (_ptr) {
        return _ptr->getName();
    }
    return std::string();
}

std::string MxConstElementHandle::GetCategory() const
{
    if (_ptr) {
        return _ptr->getCategory();
    }
    return std::string();
}

// ============================================================================
// MxNodeDefHandle Implementation
// ============================================================================

MxNodeDefHandle::MxNodeDefHandle()
    : _ptr(nullptr), _refCount(1)
{
}

MxNodeDefHandle::MxNodeDefHandle(const MaterialX::NodeDefPtr& ptr)
    : _ptr(ptr), _refCount(1)
{
}

MxNodeDefHandle::MxNodeDefHandle(const MxNodeDefHandle& other)
    : _ptr(other._ptr), _refCount(1)
{
}

MxNodeDefHandle::MxNodeDefHandle(MxNodeDefHandle&& other) noexcept
    : _ptr(std::move(other._ptr)), _refCount(1)
{
}

MxNodeDefHandle::~MxNodeDefHandle()
{
    _ptr.reset();
}

MxNodeDefHandle& MxNodeDefHandle::operator=(const MxNodeDefHandle& other)
{
    if (this != &other) {
        _ptr = other._ptr;
    }
    return *this;
}

MxNodeDefHandle& MxNodeDefHandle::operator=(MxNodeDefHandle&& other) noexcept
{
    if (this != &other) {
        _ptr = std::move(other._ptr);
    }
    return *this;
}

bool MxNodeDefHandle::IsValid() const
{
    return _ptr != nullptr;
}

const MaterialX::NodeDefPtr& MxNodeDefHandle::GetPtr() const
{
    return _ptr;
}

std::string MxNodeDefHandle::GetName() const
{
    if (_ptr) {
        return _ptr->getName();
    }
    return std::string();
}

std::string MxNodeDefHandle::GetNodeString() const
{
    if (_ptr) {
        return _ptr->getNodeString();
    }
    return std::string();
}

// ============================================================================
// Swift-Friendly Factory Functions
// ============================================================================

MxDocumentHandle*
UsdMtlxSwiftReadDocument(const std::string& resolvedPath)
{
    MaterialX::DocumentPtr doc = UsdMtlxReadDocument(resolvedPath);
    if (doc) {
        return new MxDocumentHandle(doc);
    }
    return nullptr;
}

MxConstDocumentHandle*
UsdMtlxSwiftGetDocument(const std::string& resolvedUri)
{
    MaterialX::ConstDocumentPtr doc = UsdMtlxGetDocument(resolvedUri);
    if (doc) {
        return new MxConstDocumentHandle(doc);
    }
    return nullptr;
}

MxConstDocumentHandle*
UsdMtlxSwiftGetDocumentFromString(const std::string& mtlxXml)
{
    MaterialX::ConstDocumentPtr doc = UsdMtlxGetDocumentFromString(mtlxXml);
    if (doc) {
        return new MxConstDocumentHandle(doc);
    }
    return nullptr;
}

MxDocumentHandle*
UsdMtlxSwiftCreateDocument()
{
    MaterialX::DocumentPtr doc = MaterialX::createDocument();
    if (doc) {
        return new MxDocumentHandle(doc);
    }
    return nullptr;
}

// ============================================================================
// Swift-Friendly Document Operations
// ============================================================================

void
UsdMtlxSwiftRead(
    const MxConstDocumentHandle* mtlxHandle,
    const UsdStageRefPtr& stage,
    const SdfPath& internalPath,
    const SdfPath& externalPath)
{
    if (mtlxHandle && mtlxHandle->IsValid() && stage) {
        // Convert UsdStageRefPtr to UsdStagePtr (weak pointer) for UsdMtlxRead
        UsdStagePtr weakStage(stage);
        UsdMtlxRead(mtlxHandle->GetPtr(), weakStage, internalPath, externalPath);
    }
}

void
UsdMtlxSwiftReadNodeGraphs(
    const MxConstDocumentHandle* mtlxHandle,
    const UsdStageRefPtr& stage,
    const SdfPath& internalPath)
{
    if (mtlxHandle && mtlxHandle->IsValid() && stage) {
        // Convert UsdStageRefPtr to UsdStagePtr (weak pointer) for UsdMtlxReadNodeGraphs
        UsdStagePtr weakStage(stage);
        UsdMtlxReadNodeGraphs(mtlxHandle->GetPtr(), weakStage, internalPath);
    }
}

std::string
UsdMtlxSwiftGetSourceURI(const MxConstElementHandle* elementHandle)
{
    if (elementHandle && elementHandle->IsValid()) {
        return UsdMtlxGetSourceURI(elementHandle->GetPtr());
    }
    return std::string();
}

VtValue
UsdMtlxSwiftGetUsdValue(
    const MxConstElementHandle* elementHandle,
    bool getDefaultValue)
{
    if (elementHandle && elementHandle->IsValid()) {
        return UsdMtlxGetUsdValue(elementHandle->GetPtr(), getDefaultValue);
    }
    return VtValue();
}

PXR_NAMESPACE_CLOSE_SCOPE
