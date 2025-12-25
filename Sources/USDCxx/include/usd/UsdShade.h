// UsdShade.h - Shading Schemas for SwiftUSD
// Mirrors: pxr/usd/usdShade/*.h (Material, Shader, Input, Output, NodeGraph, MaterialBindingAPI)
// C++ header with Swift interop annotations

#pragma once

#include "../Swift.h"
#include "../base/Gf.h"
#include "../base/Tf.h"
#include "../base/Vt.h"
#include "Sdf.h"
#include "Usd.h"

#include <atomic>
#include <cstring>
#include <string>
#include <vector>
#include <memory>

#if defined(USE_PIXAR_USD)
#include "pxr/pxr.h"
#include "pxr/usd/usdShade/shader.h"
#include "pxr/usd/usdShade/material.h"
#include "pxr/usd/usdShade/nodeGraph.h"
#include "pxr/usd/usdShade/input.h"
#include "pxr/usd/usdShade/output.h"
#include "pxr/usd/usdShade/materialBindingAPI.h"
#include "pxr/usd/usdShade/tokens.h"
#include "pxr/usd/usdShade/connectableAPI.h"
#endif

namespace swiftusd {

// Forward declarations
class ShadeInputRef;
class ShadeOutputRef;
class ShadeShaderRef;
class ShadeNodeGraphRef;
class ShadeMaterialRef;
class ShadeMaterialBindingAPIRef;

// ============================================================================
// MARK: - ShadeConnectionSourceInfo (value type)
// ============================================================================

/// Information about a connection source.
struct ShadeConnectionSourceInfo {
    Path sourcePath;
    Token sourcePropertyName;
    Token sourceType;  // "shader", "nodeGraph", "material"

    ShadeConnectionSourceInfo() SWIFTUSD_NOEXCEPT {}

    ShadeConnectionSourceInfo(const Path& path, const Token& propName, const Token& type) SWIFTUSD_NOEXCEPT
        : sourcePath(path), sourcePropertyName(propName), sourceType(type) {}

    bool IsValid() const SWIFTUSD_NOEXCEPT {
        return !sourcePath.IsEmpty() && !sourcePropertyName.IsEmpty();
    }

} SWIFT_SELF_CONTAINED;

// ============================================================================
// MARK: - ShadeInputRef (UsdShadeInput reference type)
// ============================================================================

/// A shader or node-graph input - a connectable attribute representing a typed value.
///
/// This wraps UsdShadeInput which is an attribute with an "inputs:" namespace prefix.
class ShadeInputRef {
public:
    // Factory - create from attribute
    static ShadeInputRef* Create(AttributeRef* attr) SWIFTUSD_NOEXCEPT {
        if (!attr) return nullptr;
#if defined(USE_PIXAR_USD)
        // For real USD, we'd wrap the attribute as an input
        return new ShadeInputRef(attr);
#else
        return new ShadeInputRef(attr);
#endif
    }

    // Validity
    bool IsValid() const SWIFTUSD_NOEXCEPT {
        return _attr != nullptr;
    }

    // Name access
    Token GetBaseName() const SWIFTUSD_NOEXCEPT {
        if (!_attr) return Token();
        Token fullName = _attr->GetName();
        const char* text = fullName.GetText();
        // Strip "inputs:" prefix (7 characters)
        if (text && std::strncmp(text, "inputs:", 7) == 0) {
            return Token(text + 7);
        }
        return fullName;
    }

    Token GetFullName() const SWIFTUSD_NOEXCEPT {
        return _attr ? _attr->GetName() : Token();
    }

    // Type access
    Token GetTypeName() const SWIFTUSD_NOEXCEPT {
        return _attr ? _attr->GetTypeName() : Token();
    }

    // Value access
    bool GetFloat(TimeCode time, float* outValue) const SWIFTUSD_NOEXCEPT {
        return _attr ? _attr->GetFloat(time, outValue) : false;
    }

    bool GetVec3f(TimeCode time, Vec3f* outValue) const SWIFTUSD_NOEXCEPT {
        return _attr ? _attr->GetVec3f(time, outValue) : false;
    }

    bool GetToken(TimeCode time, Token* outValue) const SWIFTUSD_NOEXCEPT {
        return _attr ? _attr->GetToken(time, outValue) : false;
    }

    bool GetString(TimeCode time, const char** outValue) const SWIFTUSD_NOEXCEPT {
        return _attr ? _attr->GetString(time, outValue) : false;
    }

    // Value setters
    bool SetFloat(float value, TimeCode time = TimeCode::Default()) SWIFTUSD_NOEXCEPT {
        return _attr ? _attr->SetFloat(value, time) : false;
    }

    bool SetVec3f(Vec3f value, TimeCode time = TimeCode::Default()) SWIFTUSD_NOEXCEPT {
        return _attr ? _attr->SetVec3f(value, time) : false;
    }

    bool SetToken(Token value, TimeCode time = TimeCode::Default()) SWIFTUSD_NOEXCEPT {
        return _attr ? _attr->SetToken(value, time) : false;
    }

    bool SetString(const char* value, TimeCode time = TimeCode::Default()) SWIFTUSD_NOEXCEPT {
        return _attr ? _attr->SetString(value, time) : false;
    }

    // Connection management
    bool HasConnectedSource() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        // Would need to check connections on the underlying attribute
        return false;  // Simplified
#else
        return !_connectedSource.IsEmpty();
#endif
    }

    Path GetConnectedSource() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return Path();  // Simplified
#else
        return _connectedSource;
#endif
    }

    bool SetConnectedSource(const Path& sourcePath) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return false;  // Simplified
#else
        _connectedSource = sourcePath;
        return true;
#endif
    }

    bool ClearSource() SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return false;  // Simplified
#else
        _connectedSource = Path();
        return true;
#endif
    }

    // Attribute access
    AttributeRef* GetAttr() SWIFTUSD_NOEXCEPT {
        if (_attr) _attr->retain();
        return _attr;
    }

    // Reference counting
    void retain() SWIFTUSD_NOEXCEPT {
        _refCount.fetch_add(1, std::memory_order_relaxed);
    }

    void release() SWIFTUSD_NOEXCEPT {
        if (_refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) {
            delete this;
        }
    }

private:
    explicit ShadeInputRef(AttributeRef* attr) SWIFTUSD_NOEXCEPT
        : _attr(attr), _refCount(1) {
        if (_attr) _attr->retain();
    }

    ~ShadeInputRef() {
        if (_attr) _attr->release();
    }

    AttributeRef* _attr;
    Path _connectedSource;
    std::atomic<int> _refCount;

} SWIFT_UNSAFE_REFERENCE;

// ============================================================================
// MARK: - ShadeOutputRef (UsdShadeOutput reference type)
// ============================================================================

/// A shader or node-graph output - a connectable attribute representing a computed value.
///
/// This wraps UsdShadeOutput which is an attribute with an "outputs:" namespace prefix.
class ShadeOutputRef {
public:
    // Factory - create from attribute
    static ShadeOutputRef* Create(AttributeRef* attr) SWIFTUSD_NOEXCEPT {
        if (!attr) return nullptr;
        return new ShadeOutputRef(attr);
    }

    // Validity
    bool IsValid() const SWIFTUSD_NOEXCEPT {
        return _attr != nullptr;
    }

    // Name access
    Token GetBaseName() const SWIFTUSD_NOEXCEPT {
        if (!_attr) return Token();
        Token fullName = _attr->GetName();
        const char* text = fullName.GetText();
        // Strip "outputs:" prefix (8 characters)
        if (text && std::strncmp(text, "outputs:", 8) == 0) {
            return Token(text + 8);
        }
        return fullName;
    }

    Token GetFullName() const SWIFTUSD_NOEXCEPT {
        return _attr ? _attr->GetName() : Token();
    }

    // Type access
    Token GetTypeName() const SWIFTUSD_NOEXCEPT {
        return _attr ? _attr->GetTypeName() : Token();
    }

    // Connection management
    bool HasConnectedSource() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return false;  // Simplified
#else
        return !_connectedSource.IsEmpty();
#endif
    }

    Path GetConnectedSource() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return Path();  // Simplified
#else
        return _connectedSource;
#endif
    }

    bool SetConnectedSource(const Path& sourcePath) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return false;  // Simplified
#else
        _connectedSource = sourcePath;
        return true;
#endif
    }

    // Attribute access
    AttributeRef* GetAttr() SWIFTUSD_NOEXCEPT {
        if (_attr) _attr->retain();
        return _attr;
    }

    // Reference counting
    void retain() SWIFTUSD_NOEXCEPT {
        _refCount.fetch_add(1, std::memory_order_relaxed);
    }

    void release() SWIFTUSD_NOEXCEPT {
        if (_refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) {
            delete this;
        }
    }

private:
    explicit ShadeOutputRef(AttributeRef* attr) SWIFTUSD_NOEXCEPT
        : _attr(attr), _refCount(1) {
        if (_attr) _attr->retain();
    }

    ~ShadeOutputRef() {
        if (_attr) _attr->release();
    }

    AttributeRef* _attr;
    Path _connectedSource;
    std::atomic<int> _refCount;

} SWIFT_UNSAFE_REFERENCE;

// ============================================================================
// MARK: - ShadeShaderRef (UsdShadeShader reference type)
// ============================================================================

/// A shader prim - the building block of shading networks.
///
/// Shaders represent individual shading nodes (textures, materials, etc.)
/// with typed inputs and outputs that can be connected.
class ShadeShaderRef {
public:
    // Factory methods
    static ShadeShaderRef* Define(StageRef* stage, const Path& path) SWIFTUSD_NOEXCEPT {
        if (!stage) return nullptr;
#if defined(USE_PIXAR_USD)
        auto shader = PXR_NS::UsdShadeShader::Define(stage->GetUsdStage(), path.GetSdfPath());
        if (!shader) return nullptr;
        return new ShadeShaderRef(shader, stage);
#else
        // Define the prim in the stage first
        auto* prim = stage->DefinePrim(path, Token("Shader"));
        if (!prim) return nullptr;
        prim->release();  // We don't need to hold the reference
        return new ShadeShaderRef(path, stage);
#endif
    }

    static ShadeShaderRef* FromPrim(PrimRef* prim) SWIFTUSD_NOEXCEPT {
        if (!prim) return nullptr;
#if defined(USE_PIXAR_USD)
        auto shader = PXR_NS::UsdShadeShader(prim->GetUsdPrim());
        if (!shader) return nullptr;
        return new ShadeShaderRef(shader, prim->GetStage());
#else
        return new ShadeShaderRef(prim->GetPath(), prim->GetStage());
#endif
    }

    // Validity
    bool IsValid() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return static_cast<bool>(_shader);
#else
        return !_path.IsEmpty() && _stage != nullptr;
#endif
    }

    // Prim access
    PrimRef* GetPrim() SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_shader) return nullptr;
        return new PrimRef(_shader.GetPrim(), _stage);
#else
        return _stage ? _stage->GetPrimAtPath(_path) : nullptr;
#endif
    }

    // Shader ID - identifies the type of shader (e.g., "UsdPreviewSurface")
    Token GetShaderId() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_shader) return Token();
        PXR_NS::TfToken id;
        _shader.GetIdAttr().Get(&id);
        return Token(id);
#else
        return _shaderId;
#endif
    }

    bool SetShaderId(const Token& id) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_shader) return false;
        return _shader.GetIdAttr().Set(id.GetTfToken());
#else
        _shaderId = id;
        return true;
#endif
    }

    // Create/get inputs
    ShadeInputRef* CreateInput(const Token& name, const Token& typeName) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_shader) return nullptr;
        auto sdfType = PXR_NS::SdfValueTypeNames->Find(typeName.GetTfToken().GetString());
        auto input = _shader.CreateInput(name.GetTfToken(), sdfType);
        if (!input) return nullptr;
        auto attr = new AttributeRef(input.GetAttr(), nullptr);  // Would need proper prim ref
        return ShadeInputRef::Create(attr);
#else
        // Store input locally and create a standalone attribute
        std::string inputName = "inputs:" + std::string(name.GetText());
        _inputs[std::string(name.GetText())] = typeName;

        // Create a standalone attribute for the input
        auto* attr = new AttributeRef(Token(inputName.c_str()), typeName, nullptr, false);
        return ShadeInputRef::Create(attr);
#endif
    }

    ShadeInputRef* GetInput(const Token& name) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_shader) return nullptr;
        auto input = _shader.GetInput(name.GetTfToken());
        if (!input) return nullptr;
        auto attr = new AttributeRef(input.GetAttr(), nullptr);
        return ShadeInputRef::Create(attr);
#else
        auto it = _inputs.find(std::string(name.GetText()));
        if (it == _inputs.end()) return nullptr;

        std::string inputName = "inputs:" + std::string(name.GetText());
        auto* attr = new AttributeRef(Token(inputName.c_str()), it->second, nullptr, false);
        return ShadeInputRef::Create(attr);
#endif
    }

    // Create/get outputs
    ShadeOutputRef* CreateOutput(const Token& name, const Token& typeName) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_shader) return nullptr;
        auto sdfType = PXR_NS::SdfValueTypeNames->Find(typeName.GetTfToken().GetString());
        auto output = _shader.CreateOutput(name.GetTfToken(), sdfType);
        if (!output) return nullptr;
        auto attr = new AttributeRef(output.GetAttr(), nullptr);
        return ShadeOutputRef::Create(attr);
#else
        // Store output locally and create a standalone attribute
        std::string outputName = "outputs:" + std::string(name.GetText());
        _outputs[std::string(name.GetText())] = typeName;

        auto* attr = new AttributeRef(Token(outputName.c_str()), typeName, nullptr, false);
        return ShadeOutputRef::Create(attr);
#endif
    }

    ShadeOutputRef* GetOutput(const Token& name) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_shader) return nullptr;
        auto output = _shader.GetOutput(name.GetTfToken());
        if (!output) return nullptr;
        auto attr = new AttributeRef(output.GetAttr(), nullptr);
        return ShadeOutputRef::Create(attr);
#else
        auto it = _outputs.find(std::string(name.GetText()));
        if (it == _outputs.end()) return nullptr;

        std::string outputName = "outputs:" + std::string(name.GetText());
        auto* attr = new AttributeRef(Token(outputName.c_str()), it->second, nullptr, false);
        return ShadeOutputRef::Create(attr);
#endif
    }

    // Get number of inputs/outputs
    size_t GetInputCount() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_shader) return 0;
        return _shader.GetInputs().size();
#else
        return _inputs.size();
#endif
    }

    size_t GetOutputCount() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_shader) return 0;
        return _shader.GetOutputs().size();
#else
        return _outputs.size();
#endif
    }

    // Reference counting
    void retain() SWIFTUSD_NOEXCEPT {
        _refCount.fetch_add(1, std::memory_order_relaxed);
    }

    void release() SWIFTUSD_NOEXCEPT {
        if (_refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) {
            delete this;
        }
    }

private:
#if defined(USE_PIXAR_USD)
    ShadeShaderRef(const PXR_NS::UsdShadeShader& shader, StageRef* stage) SWIFTUSD_NOEXCEPT
        : _shader(shader), _stage(stage), _refCount(1) {
        if (_stage) _stage->retain();
    }

    ~ShadeShaderRef() {
        if (_stage) _stage->release();
    }

    PXR_NS::UsdShadeShader _shader;
#else
    ShadeShaderRef(const Path& path, StageRef* stage) SWIFTUSD_NOEXCEPT
        : _path(path), _stage(stage), _refCount(1) {
        if (_stage) _stage->retain();
    }

    ~ShadeShaderRef() {
        if (_stage) _stage->release();
    }

    Path _path;
    Token _shaderId;
    std::map<std::string, Token> _inputs;
    std::map<std::string, Token> _outputs;
#endif

    StageRef* _stage;
    std::atomic<int> _refCount;

} SWIFT_UNSAFE_REFERENCE;

// ============================================================================
// MARK: - ShadeNodeGraphRef (UsdShadeNodeGraph reference type)
// ============================================================================

/// A container for shading nodes and other node-graphs.
///
/// NodeGraphs provide an interface with public inputs and outputs.
class ShadeNodeGraphRef {
public:
    // Factory methods
    static ShadeNodeGraphRef* Define(StageRef* stage, const Path& path) SWIFTUSD_NOEXCEPT {
        if (!stage) return nullptr;
#if defined(USE_PIXAR_USD)
        auto nodeGraph = PXR_NS::UsdShadeNodeGraph::Define(stage->GetUsdStage(), path.GetSdfPath());
        if (!nodeGraph) return nullptr;
        return new ShadeNodeGraphRef(nodeGraph, stage);
#else
        // Define the prim in the stage first
        auto* prim = stage->DefinePrim(path, Token("NodeGraph"));
        if (!prim) return nullptr;
        prim->release();
        return new ShadeNodeGraphRef(path, stage);
#endif
    }

    static ShadeNodeGraphRef* FromPrim(PrimRef* prim) SWIFTUSD_NOEXCEPT {
        if (!prim) return nullptr;
#if defined(USE_PIXAR_USD)
        auto nodeGraph = PXR_NS::UsdShadeNodeGraph(prim->GetUsdPrim());
        if (!nodeGraph) return nullptr;
        return new ShadeNodeGraphRef(nodeGraph, prim->GetStage());
#else
        return new ShadeNodeGraphRef(prim->GetPath(), prim->GetStage());
#endif
    }

    // Validity
    bool IsValid() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return static_cast<bool>(_nodeGraph);
#else
        return !_path.IsEmpty() && _stage != nullptr;
#endif
    }

    // Prim access
    PrimRef* GetPrim() SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_nodeGraph) return nullptr;
        return new PrimRef(_nodeGraph.GetPrim(), _stage);
#else
        return _stage ? _stage->GetPrimAtPath(_path) : nullptr;
#endif
    }

    // Interface inputs (public inputs exposed on the node graph)
    ShadeInputRef* CreateInput(const Token& name, const Token& typeName) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_nodeGraph) return nullptr;
        auto sdfType = PXR_NS::SdfValueTypeNames->Find(typeName.GetTfToken().GetString());
        auto input = _nodeGraph.CreateInput(name.GetTfToken(), sdfType);
        if (!input) return nullptr;
        auto attr = new AttributeRef(input.GetAttr(), nullptr);
        return ShadeInputRef::Create(attr);
#else
        std::string inputName = "inputs:" + std::string(name.GetText());
        _inputs[std::string(name.GetText())] = typeName;

        auto* attr = new AttributeRef(Token(inputName.c_str()), typeName, nullptr, false);
        return ShadeInputRef::Create(attr);
#endif
    }

    ShadeInputRef* GetInput(const Token& name) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_nodeGraph) return nullptr;
        auto input = _nodeGraph.GetInput(name.GetTfToken());
        if (!input) return nullptr;
        auto attr = new AttributeRef(input.GetAttr(), nullptr);
        return ShadeInputRef::Create(attr);
#else
        auto it = _inputs.find(std::string(name.GetText()));
        if (it == _inputs.end()) return nullptr;

        std::string inputName = "inputs:" + std::string(name.GetText());
        auto* attr = new AttributeRef(Token(inputName.c_str()), it->second, nullptr, false);
        return ShadeInputRef::Create(attr);
#endif
    }

    // Public outputs
    ShadeOutputRef* CreateOutput(const Token& name, const Token& typeName) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_nodeGraph) return nullptr;
        auto sdfType = PXR_NS::SdfValueTypeNames->Find(typeName.GetTfToken().GetString());
        auto output = _nodeGraph.CreateOutput(name.GetTfToken(), sdfType);
        if (!output) return nullptr;
        auto attr = new AttributeRef(output.GetAttr(), nullptr);
        return ShadeOutputRef::Create(attr);
#else
        std::string outputName = "outputs:" + std::string(name.GetText());
        _outputs[std::string(name.GetText())] = typeName;

        auto* attr = new AttributeRef(Token(outputName.c_str()), typeName, nullptr, false);
        return ShadeOutputRef::Create(attr);
#endif
    }

    ShadeOutputRef* GetOutput(const Token& name) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_nodeGraph) return nullptr;
        auto output = _nodeGraph.GetOutput(name.GetTfToken());
        if (!output) return nullptr;
        auto attr = new AttributeRef(output.GetAttr(), nullptr);
        return ShadeOutputRef::Create(attr);
#else
        auto it = _outputs.find(std::string(name.GetText()));
        if (it == _outputs.end()) return nullptr;

        std::string outputName = "outputs:" + std::string(name.GetText());
        auto* attr = new AttributeRef(Token(outputName.c_str()), it->second, nullptr, false);
        return ShadeOutputRef::Create(attr);
#endif
    }

    // Counts
    size_t GetInputCount() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_nodeGraph) return 0;
        return _nodeGraph.GetInputs().size();
#else
        return _inputs.size();
#endif
    }

    size_t GetOutputCount() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_nodeGraph) return 0;
        return _nodeGraph.GetOutputs().size();
#else
        return _outputs.size();
#endif
    }

    // Reference counting
    void retain() SWIFTUSD_NOEXCEPT {
        _refCount.fetch_add(1, std::memory_order_relaxed);
    }

    void release() SWIFTUSD_NOEXCEPT {
        if (_refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) {
            delete this;
        }
    }

private:
#if defined(USE_PIXAR_USD)
    ShadeNodeGraphRef(const PXR_NS::UsdShadeNodeGraph& nodeGraph, StageRef* stage) SWIFTUSD_NOEXCEPT
        : _nodeGraph(nodeGraph), _stage(stage), _refCount(1) {
        if (_stage) _stage->retain();
    }

    ~ShadeNodeGraphRef() {
        if (_stage) _stage->release();
    }

    PXR_NS::UsdShadeNodeGraph _nodeGraph;
#else
    ShadeNodeGraphRef(const Path& path, StageRef* stage) SWIFTUSD_NOEXCEPT
        : _path(path), _stage(stage), _refCount(1) {
        if (_stage) _stage->retain();
    }

    ~ShadeNodeGraphRef() {
        if (_stage) _stage->release();
    }

    Path _path;
    std::map<std::string, Token> _inputs;
    std::map<std::string, Token> _outputs;
#endif

    StageRef* _stage;
    std::atomic<int> _refCount;

} SWIFT_UNSAFE_REFERENCE;

// ============================================================================
// MARK: - ShadeMaterialRef (UsdShadeMaterial reference type)
// ============================================================================

/// A material prim - container for render context shading data.
///
/// Materials contain shaders that define the appearance of geometry.
/// They provide outputs for surface, displacement, and volume shading.
class ShadeMaterialRef {
public:
    // Factory methods
    static ShadeMaterialRef* Define(StageRef* stage, const Path& path) SWIFTUSD_NOEXCEPT {
        if (!stage) return nullptr;
#if defined(USE_PIXAR_USD)
        auto material = PXR_NS::UsdShadeMaterial::Define(stage->GetUsdStage(), path.GetSdfPath());
        if (!material) return nullptr;
        return new ShadeMaterialRef(material, stage);
#else
        // Define the prim in the stage first
        auto* prim = stage->DefinePrim(path, Token("Material"));
        if (!prim) return nullptr;
        prim->release();
        return new ShadeMaterialRef(path, stage);
#endif
    }

    static ShadeMaterialRef* FromPrim(PrimRef* prim) SWIFTUSD_NOEXCEPT {
        if (!prim) return nullptr;
#if defined(USE_PIXAR_USD)
        auto material = PXR_NS::UsdShadeMaterial(prim->GetUsdPrim());
        if (!material) return nullptr;
        return new ShadeMaterialRef(material, prim->GetStage());
#else
        return new ShadeMaterialRef(prim->GetPath(), prim->GetStage());
#endif
    }

    // Validity
    bool IsValid() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return static_cast<bool>(_material);
#else
        return !_path.IsEmpty() && _stage != nullptr;
#endif
    }

    // Prim access
    PrimRef* GetPrim() SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_material) return nullptr;
        return new PrimRef(_material.GetPrim(), _stage);
#else
        return _stage ? _stage->GetPrimAtPath(_path) : nullptr;
#endif
    }

    // Surface output - the main shading output
    ShadeOutputRef* CreateSurfaceOutput(const Token& renderContext = Token()) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_material) return nullptr;
        auto output = _material.CreateSurfaceOutput(renderContext.GetTfToken());
        if (!output) return nullptr;
        auto attr = new AttributeRef(output.GetAttr(), nullptr);
        return ShadeOutputRef::Create(attr);
#else
        std::string outputName = renderContext.IsEmpty()
            ? "outputs:surface"
            : "outputs:" + std::string(renderContext.GetText()) + ":surface";
        _outputs[outputName] = Token("token");

        auto* attr = new AttributeRef(Token(outputName.c_str()), Token("token"), nullptr, false);
        return ShadeOutputRef::Create(attr);
#endif
    }

    ShadeOutputRef* GetSurfaceOutput(const Token& renderContext = Token()) const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_material) return nullptr;
        auto output = _material.GetSurfaceOutput(renderContext.GetTfToken());
        if (!output) return nullptr;
        auto attr = new AttributeRef(output.GetAttr(), nullptr);
        return ShadeOutputRef::Create(attr);
#else
        std::string outputName = renderContext.IsEmpty()
            ? "outputs:surface"
            : "outputs:" + std::string(renderContext.GetText()) + ":surface";
        auto it = _outputs.find(outputName);
        if (it == _outputs.end()) return nullptr;

        auto* attr = new AttributeRef(Token(outputName.c_str()), it->second, nullptr, false);
        return ShadeOutputRef::Create(attr);
#endif
    }

    // Displacement output
    ShadeOutputRef* CreateDisplacementOutput(const Token& renderContext = Token()) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_material) return nullptr;
        auto output = _material.CreateDisplacementOutput(renderContext.GetTfToken());
        if (!output) return nullptr;
        auto attr = new AttributeRef(output.GetAttr(), nullptr);
        return ShadeOutputRef::Create(attr);
#else
        std::string outputName = renderContext.IsEmpty()
            ? "outputs:displacement"
            : "outputs:" + std::string(renderContext.GetText()) + ":displacement";
        _outputs[outputName] = Token("token");

        auto* attr = new AttributeRef(Token(outputName.c_str()), Token("token"), nullptr, false);
        return ShadeOutputRef::Create(attr);
#endif
    }

    ShadeOutputRef* GetDisplacementOutput(const Token& renderContext = Token()) const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_material) return nullptr;
        auto output = _material.GetDisplacementOutput(renderContext.GetTfToken());
        if (!output) return nullptr;
        auto attr = new AttributeRef(output.GetAttr(), nullptr);
        return ShadeOutputRef::Create(attr);
#else
        std::string outputName = renderContext.IsEmpty()
            ? "outputs:displacement"
            : "outputs:" + std::string(renderContext.GetText()) + ":displacement";
        auto it = _outputs.find(outputName);
        if (it == _outputs.end()) return nullptr;

        auto* attr = new AttributeRef(Token(outputName.c_str()), it->second, nullptr, false);
        return ShadeOutputRef::Create(attr);
#endif
    }

    // Volume output
    ShadeOutputRef* CreateVolumeOutput(const Token& renderContext = Token()) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_material) return nullptr;
        auto output = _material.CreateVolumeOutput(renderContext.GetTfToken());
        if (!output) return nullptr;
        auto attr = new AttributeRef(output.GetAttr(), nullptr);
        return ShadeOutputRef::Create(attr);
#else
        std::string outputName = renderContext.IsEmpty()
            ? "outputs:volume"
            : "outputs:" + std::string(renderContext.GetText()) + ":volume";
        _outputs[outputName] = Token("token");

        auto* attr = new AttributeRef(Token(outputName.c_str()), Token("token"), nullptr, false);
        return ShadeOutputRef::Create(attr);
#endif
    }

    // Interface inputs (inherited from NodeGraph behavior)
    ShadeInputRef* CreateInput(const Token& name, const Token& typeName) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_material) return nullptr;
        auto sdfType = PXR_NS::SdfValueTypeNames->Find(typeName.GetTfToken().GetString());
        auto input = _material.CreateInput(name.GetTfToken(), sdfType);
        if (!input) return nullptr;
        auto attr = new AttributeRef(input.GetAttr(), nullptr);
        return ShadeInputRef::Create(attr);
#else
        std::string inputName = "inputs:" + std::string(name.GetText());
        _inputs[std::string(name.GetText())] = typeName;

        auto* attr = new AttributeRef(Token(inputName.c_str()), typeName, nullptr, false);
        return ShadeInputRef::Create(attr);
#endif
    }

    ShadeInputRef* GetInput(const Token& name) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_material) return nullptr;
        auto input = _material.GetInput(name.GetTfToken());
        if (!input) return nullptr;
        auto attr = new AttributeRef(input.GetAttr(), nullptr);
        return ShadeInputRef::Create(attr);
#else
        auto it = _inputs.find(std::string(name.GetText()));
        if (it == _inputs.end()) return nullptr;

        std::string inputName = "inputs:" + std::string(name.GetText());
        auto* attr = new AttributeRef(Token(inputName.c_str()), it->second, nullptr, false);
        return ShadeInputRef::Create(attr);
#endif
    }

    // Reference counting
    void retain() SWIFTUSD_NOEXCEPT {
        _refCount.fetch_add(1, std::memory_order_relaxed);
    }

    void release() SWIFTUSD_NOEXCEPT {
        if (_refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) {
            delete this;
        }
    }

private:
#if defined(USE_PIXAR_USD)
    ShadeMaterialRef(const PXR_NS::UsdShadeMaterial& material, StageRef* stage) SWIFTUSD_NOEXCEPT
        : _material(material), _stage(stage), _refCount(1) {
        if (_stage) _stage->retain();
    }

    ~ShadeMaterialRef() {
        if (_stage) _stage->release();
    }

    PXR_NS::UsdShadeMaterial _material;
#else
    ShadeMaterialRef(const Path& path, StageRef* stage) SWIFTUSD_NOEXCEPT
        : _path(path), _stage(stage), _refCount(1) {
        if (_stage) _stage->retain();
    }

    ~ShadeMaterialRef() {
        if (_stage) _stage->release();
    }

    Path _path;
    mutable std::map<std::string, Token> _inputs;
    mutable std::map<std::string, Token> _outputs;
#endif

    StageRef* _stage;
    std::atomic<int> _refCount;

} SWIFT_UNSAFE_REFERENCE;

// ============================================================================
// MARK: - ShadeMaterialBindingAPIRef (UsdShadeMaterialBindingAPI reference type)
// ============================================================================

/// API schema for binding materials to geometry.
///
/// MaterialBindingAPI provides the interface for binding materials to prims
/// and querying the bound material.
class ShadeMaterialBindingAPIRef {
public:
    // Factory methods
    static ShadeMaterialBindingAPIRef* Apply(PrimRef* prim) SWIFTUSD_NOEXCEPT {
        if (!prim) return nullptr;
#if defined(USE_PIXAR_USD)
        auto api = PXR_NS::UsdShadeMaterialBindingAPI::Apply(prim->GetUsdPrim());
        if (!api) return nullptr;
        return new ShadeMaterialBindingAPIRef(api, prim->GetStage());
#else
        return new ShadeMaterialBindingAPIRef(prim->GetPath(), prim->GetStage());
#endif
    }

    static ShadeMaterialBindingAPIRef* FromPrim(PrimRef* prim) SWIFTUSD_NOEXCEPT {
        if (!prim) return nullptr;
#if defined(USE_PIXAR_USD)
        auto api = PXR_NS::UsdShadeMaterialBindingAPI(prim->GetUsdPrim());
        if (!api) return nullptr;
        return new ShadeMaterialBindingAPIRef(api, prim->GetStage());
#else
        return new ShadeMaterialBindingAPIRef(prim->GetPath(), prim->GetStage());
#endif
    }

    // Validity
    bool IsValid() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return static_cast<bool>(_api);
#else
        return !_path.IsEmpty() && _stage != nullptr;
#endif
    }

    // Prim access
    PrimRef* GetPrim() SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_api) return nullptr;
        return new PrimRef(_api.GetPrim(), _stage);
#else
        return _stage ? _stage->GetPrimAtPath(_path) : nullptr;
#endif
    }

    // Bind a material to this prim
    bool Bind(ShadeMaterialRef* material, const Token& bindingStrength = Token(),
              const Token& materialPurpose = Token()) SWIFTUSD_NOEXCEPT {
        if (!material) return false;
#if defined(USE_PIXAR_USD)
        if (!_api) return false;
        PXR_NS::UsdShadeMaterialBindingAPI::BindingStrength strength =
            PXR_NS::UsdShadeMaterialBindingAPI::GetMaterialBindingStrength(
                _api.GetDirectBindingRel(materialPurpose.GetTfToken()));
        return _api.Bind(material->_material, strength, materialPurpose.GetTfToken());
#else
        auto* materialPrim = material->GetPrim();
        if (!materialPrim) return false;
        _boundMaterialPath = materialPrim->GetPath();
        materialPrim->release();
        return true;
#endif
    }

    // Unbind the material from this prim
    bool Unbind(const Token& materialPurpose = Token()) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_api) return false;
        return _api.UnbindDirectBinding(materialPurpose.GetTfToken());
#else
        _boundMaterialPath = Path();
        return true;
#endif
    }

    // Get the bound material
    ShadeMaterialRef* GetBoundMaterial(const Token& materialPurpose = Token()) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_api) return nullptr;
        auto material = _api.ComputeBoundMaterial(materialPurpose.GetTfToken());
        if (!material) return nullptr;
        return new ShadeMaterialRef(material, _stage);
#else
        if (_boundMaterialPath.IsEmpty() || !_stage) return nullptr;
        auto* prim = _stage->GetPrimAtPath(_boundMaterialPath);
        if (!prim) return nullptr;
        auto* material = ShadeMaterialRef::FromPrim(prim);
        prim->release();
        return material;
#endif
    }

    // Check if a material is bound
    bool HasBoundMaterial(const Token& materialPurpose = Token()) const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_api) return false;
        return static_cast<bool>(_api.ComputeBoundMaterial(materialPurpose.GetTfToken()));
#else
        return !_boundMaterialPath.IsEmpty();
#endif
    }

    // Get the path to the bound material (without resolving)
    Path GetDirectlyBoundMaterialPath(const Token& materialPurpose = Token()) const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_api) return Path();
        auto binding = _api.GetDirectBinding(materialPurpose.GetTfToken());
        if (!binding.IsBound()) return Path();
        return Path(binding.GetMaterialPath());
#else
        return _boundMaterialPath;
#endif
    }

    // Reference counting
    void retain() SWIFTUSD_NOEXCEPT {
        _refCount.fetch_add(1, std::memory_order_relaxed);
    }

    void release() SWIFTUSD_NOEXCEPT {
        if (_refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) {
            delete this;
        }
    }

private:
#if defined(USE_PIXAR_USD)
    ShadeMaterialBindingAPIRef(const PXR_NS::UsdShadeMaterialBindingAPI& api, StageRef* stage) SWIFTUSD_NOEXCEPT
        : _api(api), _stage(stage), _refCount(1) {
        if (_stage) _stage->retain();
    }

    ~ShadeMaterialBindingAPIRef() {
        if (_stage) _stage->release();
    }

    PXR_NS::UsdShadeMaterialBindingAPI _api;
#else
    ShadeMaterialBindingAPIRef(const Path& path, StageRef* stage) SWIFTUSD_NOEXCEPT
        : _path(path), _stage(stage), _refCount(1) {
        if (_stage) _stage->retain();
    }

    ~ShadeMaterialBindingAPIRef() {
        if (_stage) _stage->release();
    }

    Path _path;
    Path _boundMaterialPath;
#endif

    StageRef* _stage;
    std::atomic<int> _refCount;

} SWIFT_UNSAFE_REFERENCE;

} // namespace swiftusd
