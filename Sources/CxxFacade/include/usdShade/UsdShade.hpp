// UsdShade.hpp - Shading Schemas facade
// CLEAN HEADER - NO pxr includes!
//
// This module provides wrappers for UsdShade schema classes including shaders,
// materials, node graphs, and material binding APIs.
#pragma once

#include "../CxxFacade.h"
#include "../usd/Usd.hpp"
#include "../usd/Sdf.hpp"
#include "../base/Tf.hpp"
#include "../base/Vt.hpp"

namespace cxxfacade {
namespace usdShade {

// ============================================================================
// Forward Declarations
// ============================================================================

class UsdShadeInput;
class UsdShadeOutput;
class UsdShadeConnectableAPI;
class UsdShadeShader;
class UsdShadeNodeGraph;
class UsdShadeMaterial;
class UsdShadeMaterialBindingAPI;

// ============================================================================
// UsdShadeAttributeType - Type of shading attribute
// ============================================================================

/// Enum for shading attribute types
enum class UsdShadeAttributeType {
    Invalid,
    Input,
    Output
};

// ============================================================================
// UsdShadeConnectionModification - Connection modification mode
// ============================================================================

/// Enum for how connections should be modified
enum class UsdShadeConnectionModification {
    Replace,  // Replace existing connections
    Prepend,  // Add to beginning of connection list
    Append    // Add to end of connection list
};

// ============================================================================
// UsdShadeInput - Shader/NodeGraph input
// ============================================================================

/// UsdShadeInput - Represents an input on a shader or node-graph.
///
/// This class encapsulates a shader or node-graph input, which is a
/// connectable attribute representing a typed value.
class UsdShadeInput {
public:
    UsdShadeInput();
    UsdShadeInput(const UsdShadeInput& other);
    UsdShadeInput(UsdShadeInput&& other) noexcept;
    ~UsdShadeInput();

    UsdShadeInput& operator=(const UsdShadeInput& other);
    UsdShadeInput& operator=(UsdShadeInput&& other) noexcept;

    /// Check if this is a valid input
    bool IsValid() const;
    bool IsDefined() const { return IsValid(); }
    explicit operator bool() const { return IsValid(); }

    /// Get the full name of the attribute (with "inputs:" prefix)
    tf::TfToken GetFullName() const;

    /// Get the base name (without "inputs:" prefix)
    tf::TfToken GetBaseName() const;

    /// Get the type name of this input
    std::string GetTypeName() const;

    /// Get the prim that owns this input
    usd::UsdPrim GetPrim() const;

    /// Get the underlying attribute
    usd::UsdAttribute GetAttr() const;

    /// Get the value at the given time
    bool Get(vt::VtValue* value, usd::UsdTimeCode time = usd::UsdTimeCode::Default()) const;

    /// Set the value at the given time
    bool Set(const vt::VtValue& value, usd::UsdTimeCode time = usd::UsdTimeCode::Default());

    // ----- Render Type -----

    /// Set the render type
    bool SetRenderType(const tf::TfToken& renderType);

    /// Get the render type
    tf::TfToken GetRenderType() const;

    /// Check if a render type has been specified
    bool HasRenderType() const;

    // ----- Connectability -----

    /// Set the connectability of this input
    bool SetConnectability(const tf::TfToken& connectability);

    /// Get the connectability of this input
    tf::TfToken GetConnectability() const;

    /// Clear any authored connectability
    bool ClearConnectability() const;

    // ----- Connections -----

    /// Check if this input can connect to the given source
    bool CanConnect(const UsdShadeInput& source) const;
    bool CanConnect(const UsdShadeOutput& source) const;

    /// Connect to a source path
    bool ConnectToSource(const sdf::SdfPath& sourcePath) const;

    /// Connect to an input
    bool ConnectToSource(const UsdShadeInput& sourceInput) const;

    /// Connect to an output
    bool ConnectToSource(const UsdShadeOutput& sourceOutput) const;

    /// Check if this input has a connected source
    bool HasConnectedSource() const;

    /// Disconnect source for this input
    bool DisconnectSource() const;

    /// Clear all connections
    bool ClearSources() const;

    // ----- Documentation -----

    /// Set documentation
    bool SetDocumentation(const std::string& docs);

    /// Get documentation
    std::string GetDocumentation() const;

    /// Set display group
    bool SetDisplayGroup(const std::string& displayGroup);

    /// Get display group
    std::string GetDisplayGroup() const;

    bool operator==(const UsdShadeInput& other) const;
    bool operator!=(const UsdShadeInput& other) const { return !(*this == other); }

    void* GetImpl() const { return impl_; }
    static UsdShadeInput FromImpl(void* impl);

private:
    void* impl_;
};

// ============================================================================
// UsdShadeOutput - Shader/NodeGraph output
// ============================================================================

/// UsdShadeOutput - Represents an output on a shader or node-graph.
///
/// This class encapsulates a shader or node-graph output, which is a
/// connectable attribute representing a typed, externally computed value.
class UsdShadeOutput {
public:
    UsdShadeOutput();
    UsdShadeOutput(const UsdShadeOutput& other);
    UsdShadeOutput(UsdShadeOutput&& other) noexcept;
    ~UsdShadeOutput();

    UsdShadeOutput& operator=(const UsdShadeOutput& other);
    UsdShadeOutput& operator=(UsdShadeOutput&& other) noexcept;

    /// Check if this is a valid output
    bool IsValid() const;
    bool IsDefined() const { return IsValid(); }
    explicit operator bool() const { return IsValid(); }

    /// Get the full name of the attribute (with "outputs:" prefix)
    tf::TfToken GetFullName() const;

    /// Get the base name (without "outputs:" prefix)
    tf::TfToken GetBaseName() const;

    /// Get the type name of this output
    std::string GetTypeName() const;

    /// Get the prim that owns this output
    usd::UsdPrim GetPrim() const;

    /// Get the underlying attribute
    usd::UsdAttribute GetAttr() const;

    /// Set the value at the given time
    bool Set(const vt::VtValue& value, usd::UsdTimeCode time = usd::UsdTimeCode::Default());

    // ----- Render Type -----

    /// Set the render type
    bool SetRenderType(const tf::TfToken& renderType);

    /// Get the render type
    tf::TfToken GetRenderType() const;

    /// Check if a render type has been specified
    bool HasRenderType() const;

    // ----- Connections -----

    /// Check if this output can connect to the given source
    bool CanConnect(const UsdShadeInput& source) const;
    bool CanConnect(const UsdShadeOutput& source) const;

    /// Connect to a source path
    bool ConnectToSource(const sdf::SdfPath& sourcePath) const;

    /// Connect to an input
    bool ConnectToSource(const UsdShadeInput& sourceInput) const;

    /// Connect to an output
    bool ConnectToSource(const UsdShadeOutput& sourceOutput) const;

    /// Check if this output has a connected source
    bool HasConnectedSource() const;

    /// Disconnect source for this output
    bool DisconnectSource() const;

    /// Clear all connections
    bool ClearSources() const;

    bool operator==(const UsdShadeOutput& other) const;
    bool operator!=(const UsdShadeOutput& other) const { return !(*this == other); }

    void* GetImpl() const { return impl_; }
    static UsdShadeOutput FromImpl(void* impl);

private:
    void* impl_;
};

// ============================================================================
// UsdShadeConnectableAPI - API for connectable prims
// ============================================================================

/// UsdShadeConnectableAPI - API schema for creating outputs and connections.
///
/// This provides a common interface for shaders and node-graphs to create
/// inputs/outputs and establish connections between shading parameters.
class UsdShadeConnectableAPI : public usd::UsdAPISchemaBase {
public:
    explicit UsdShadeConnectableAPI(const usd::UsdPrim& prim = usd::UsdPrim());
    explicit UsdShadeConnectableAPI(const usd::UsdSchemaBase& schemaObj);
    virtual ~UsdShadeConnectableAPI();

    /// Get this API on the prim at path on stage
    static UsdShadeConnectableAPI Get(const usd::UsdStage& stage, const sdf::SdfPath& path);

    /// Check if the prim is a container (like a NodeGraph)
    bool IsContainer() const;

    /// Check if container encapsulation rules should be respected
    bool RequiresEncapsulation() const;

    // ----- Inputs -----

    /// Create an input
    UsdShadeInput CreateInput(const tf::TfToken& name, const std::string& typeName);

    /// Get an input by name
    UsdShadeInput GetInput(const tf::TfToken& name) const;

    /// Get all inputs
    std::vector<UsdShadeInput> GetInputs(bool onlyAuthored = true) const;

    // ----- Outputs -----

    /// Create an output
    UsdShadeOutput CreateOutput(const tf::TfToken& name, const std::string& typeName);

    /// Get an output by name
    UsdShadeOutput GetOutput(const tf::TfToken& name) const;

    /// Get all outputs
    std::vector<UsdShadeOutput> GetOutputs(bool onlyAuthored = true) const;

    // ----- Static Connection Methods -----

    /// Check if an input can connect to a source
    static bool CanConnect(const UsdShadeInput& input, const usd::UsdAttribute& source);

    /// Check if an output can connect to a source
    static bool CanConnect(const UsdShadeOutput& output, const usd::UsdAttribute& source);

    /// Connect an attribute to a source path
    static bool ConnectToSource(const usd::UsdAttribute& attr, const sdf::SdfPath& sourcePath);

    /// Check if an attribute has a connected source
    static bool HasConnectedSource(const usd::UsdAttribute& attr);

    /// Disconnect an attribute from its source
    static bool DisconnectSource(const usd::UsdAttribute& attr);

    /// Clear all connections for an attribute
    static bool ClearSources(const usd::UsdAttribute& attr);

protected:
    usd::UsdSchemaKind _GetSchemaKind() const override;
};

// ============================================================================
// UsdShadeShader - Shader prim
// ============================================================================

/// UsdShadeShader - Base class for all USD shaders.
///
/// Shaders are the building blocks of shading networks. They represent
/// a single shading object like a texture, noise function, or mix node.
class UsdShadeShader : public usd::UsdTyped {
public:
    explicit UsdShadeShader(const usd::UsdPrim& prim = usd::UsdPrim());
    explicit UsdShadeShader(const usd::UsdSchemaBase& schemaObj);
    virtual ~UsdShadeShader();

    /// Get this prim at path on stage
    static UsdShadeShader Get(const usd::UsdStage& stage, const sdf::SdfPath& path);

    /// Define a shader at the given path
    static UsdShadeShader Define(usd::UsdStage& stage, const sdf::SdfPath& path);

    /// Get the ConnectableAPI for this shader
    UsdShadeConnectableAPI ConnectableAPI() const;

    // ----- Shader ID -----

    /// Get the implementation source attribute
    usd::UsdAttribute GetImplementationSourceAttr() const;

    /// Get the shader ID attribute
    usd::UsdAttribute GetIdAttr() const;

    /// Create the shader ID attribute
    usd::UsdAttribute CreateIdAttr();

    /// Get the implementation source token
    tf::TfToken GetImplementationSource() const;

    /// Set the shader ID
    bool SetShaderId(const tf::TfToken& id);

    /// Get the shader ID
    bool GetShaderId(tf::TfToken* id) const;

    // ----- Source Asset -----

    /// Set the source asset
    bool SetSourceAsset(const sdf::SdfAssetPath& sourceAsset,
                        const tf::TfToken& sourceType = tf::TfToken());

    /// Get the source asset
    bool GetSourceAsset(sdf::SdfAssetPath* sourceAsset,
                        const tf::TfToken& sourceType = tf::TfToken()) const;

    /// Set the source asset sub-identifier
    bool SetSourceAssetSubIdentifier(const tf::TfToken& subIdentifier,
                                      const tf::TfToken& sourceType = tf::TfToken());

    /// Get the source asset sub-identifier
    bool GetSourceAssetSubIdentifier(tf::TfToken* subIdentifier,
                                      const tf::TfToken& sourceType = tf::TfToken()) const;

    // ----- Source Code -----

    /// Set source code
    bool SetSourceCode(const std::string& sourceCode,
                       const tf::TfToken& sourceType = tf::TfToken());

    /// Get source code
    bool GetSourceCode(std::string* sourceCode,
                       const tf::TfToken& sourceType = tf::TfToken()) const;

    /// Get the source types
    std::vector<std::string> GetSourceTypes() const;

    // ----- Inputs/Outputs -----

    /// Create an input
    UsdShadeInput CreateInput(const tf::TfToken& name, const std::string& typeName);

    /// Get an input by name
    UsdShadeInput GetInput(const tf::TfToken& name) const;

    /// Get all inputs
    std::vector<UsdShadeInput> GetInputs(bool onlyAuthored = true) const;

    /// Create an output
    UsdShadeOutput CreateOutput(const tf::TfToken& name, const std::string& typeName);

    /// Get an output by name
    UsdShadeOutput GetOutput(const tf::TfToken& name) const;

    /// Get all outputs
    std::vector<UsdShadeOutput> GetOutputs(bool onlyAuthored = true) const;

protected:
    usd::UsdSchemaKind _GetSchemaKind() const override;
};

// ============================================================================
// UsdShadeNodeGraph - Node graph container
// ============================================================================

/// UsdShadeNodeGraph - A container for shading nodes.
///
/// A node-graph is a container for shading nodes, as well as other node-graphs.
/// It has a public input interface and provides a list of public outputs.
class UsdShadeNodeGraph : public usd::UsdTyped {
public:
    explicit UsdShadeNodeGraph(const usd::UsdPrim& prim = usd::UsdPrim());
    explicit UsdShadeNodeGraph(const usd::UsdSchemaBase& schemaObj);
    virtual ~UsdShadeNodeGraph();

    /// Get this prim at path on stage
    static UsdShadeNodeGraph Get(const usd::UsdStage& stage, const sdf::SdfPath& path);

    /// Define a node graph at the given path
    static UsdShadeNodeGraph Define(usd::UsdStage& stage, const sdf::SdfPath& path);

    /// Get the ConnectableAPI for this node graph
    UsdShadeConnectableAPI ConnectableAPI() const;

    // ----- Inputs -----

    /// Create an input (interface attribute)
    UsdShadeInput CreateInput(const tf::TfToken& name, const std::string& typeName);

    /// Get an input by name
    UsdShadeInput GetInput(const tf::TfToken& name) const;

    /// Get all inputs
    std::vector<UsdShadeInput> GetInputs(bool onlyAuthored = true) const;

    /// Get all interface inputs
    std::vector<UsdShadeInput> GetInterfaceInputs() const;

    // ----- Outputs -----

    /// Create an output
    UsdShadeOutput CreateOutput(const tf::TfToken& name, const std::string& typeName);

    /// Get an output by name
    UsdShadeOutput GetOutput(const tf::TfToken& name) const;

    /// Get all outputs
    std::vector<UsdShadeOutput> GetOutputs(bool onlyAuthored = true) const;

    /// Compute the output source for the given output
    UsdShadeShader ComputeOutputSource(const tf::TfToken& outputName,
                                        tf::TfToken* sourceName,
                                        UsdShadeAttributeType* sourceType) const;

protected:
    usd::UsdSchemaKind _GetSchemaKind() const override;
};

// ============================================================================
// UsdShadeMaterial - Material prim
// ============================================================================

/// UsdShadeMaterial - A material for shading geometry.
///
/// A Material provides a container into which multiple "render contexts"
/// can add data that defines a "shading material" for a renderer.
class UsdShadeMaterial : public UsdShadeNodeGraph {
public:
    explicit UsdShadeMaterial(const usd::UsdPrim& prim = usd::UsdPrim());
    explicit UsdShadeMaterial(const usd::UsdSchemaBase& schemaObj);
    virtual ~UsdShadeMaterial();

    /// Get this prim at path on stage
    static UsdShadeMaterial Get(const usd::UsdStage& stage, const sdf::SdfPath& path);

    /// Define a material at the given path
    static UsdShadeMaterial Define(usd::UsdStage& stage, const sdf::SdfPath& path);

    // ----- Surface -----

    /// Get the surface attribute
    usd::UsdAttribute GetSurfaceAttr() const;

    /// Create the surface attribute
    usd::UsdAttribute CreateSurfaceAttr();

    /// Create a surface output for the given render context
    UsdShadeOutput CreateSurfaceOutput(const tf::TfToken& renderContext = tf::TfToken());

    /// Get the surface output for the given render context
    UsdShadeOutput GetSurfaceOutput(const tf::TfToken& renderContext = tf::TfToken()) const;

    /// Get all surface outputs
    std::vector<UsdShadeOutput> GetSurfaceOutputs() const;

    /// Compute the surface source shader
    UsdShadeShader ComputeSurfaceSource(const tf::TfToken& renderContext = tf::TfToken(),
                                         tf::TfToken* sourceName = nullptr,
                                         UsdShadeAttributeType* sourceType = nullptr) const;

    // ----- Displacement -----

    /// Get the displacement attribute
    usd::UsdAttribute GetDisplacementAttr() const;

    /// Create the displacement attribute
    usd::UsdAttribute CreateDisplacementAttr();

    /// Create a displacement output for the given render context
    UsdShadeOutput CreateDisplacementOutput(const tf::TfToken& renderContext = tf::TfToken());

    /// Get the displacement output for the given render context
    UsdShadeOutput GetDisplacementOutput(const tf::TfToken& renderContext = tf::TfToken()) const;

    /// Get all displacement outputs
    std::vector<UsdShadeOutput> GetDisplacementOutputs() const;

    /// Compute the displacement source shader
    UsdShadeShader ComputeDisplacementSource(const tf::TfToken& renderContext = tf::TfToken(),
                                              tf::TfToken* sourceName = nullptr,
                                              UsdShadeAttributeType* sourceType = nullptr) const;

    // ----- Volume -----

    /// Get the volume attribute
    usd::UsdAttribute GetVolumeAttr() const;

    /// Create the volume attribute
    usd::UsdAttribute CreateVolumeAttr();

    /// Create a volume output for the given render context
    UsdShadeOutput CreateVolumeOutput(const tf::TfToken& renderContext = tf::TfToken());

    /// Get the volume output for the given render context
    UsdShadeOutput GetVolumeOutput(const tf::TfToken& renderContext = tf::TfToken()) const;

    /// Get all volume outputs
    std::vector<UsdShadeOutput> GetVolumeOutputs() const;

    /// Compute the volume source shader
    UsdShadeShader ComputeVolumeSource(const tf::TfToken& renderContext = tf::TfToken(),
                                        tf::TfToken* sourceName = nullptr,
                                        UsdShadeAttributeType* sourceType = nullptr) const;

    // ----- Base Material -----

    /// Get the base material
    UsdShadeMaterial GetBaseMaterial() const;

    /// Get the base material path
    sdf::SdfPath GetBaseMaterialPath() const;

    /// Set the base material
    void SetBaseMaterial(const UsdShadeMaterial& baseMaterial);

    /// Set the base material path
    void SetBaseMaterialPath(const sdf::SdfPath& baseMaterialPath);

    /// Clear the base material
    void ClearBaseMaterial();

    /// Check if this material has a base material
    bool HasBaseMaterial() const;

protected:
    usd::UsdSchemaKind _GetSchemaKind() const override;
};

// ============================================================================
// UsdShadeMaterialBindingAPI - API for binding materials
// ============================================================================

/// UsdShadeMaterialBindingAPI - API schema for binding materials to prims.
///
/// This provides an interface for binding materials to prims or collections
/// of prims.
class UsdShadeMaterialBindingAPI : public usd::UsdAPISchemaBase {
public:
    explicit UsdShadeMaterialBindingAPI(const usd::UsdPrim& prim = usd::UsdPrim());
    explicit UsdShadeMaterialBindingAPI(const usd::UsdSchemaBase& schemaObj);
    virtual ~UsdShadeMaterialBindingAPI();

    /// Get this API on the prim at path on stage
    static UsdShadeMaterialBindingAPI Get(const usd::UsdStage& stage, const sdf::SdfPath& path);

    /// Apply this API schema to the given prim
    static UsdShadeMaterialBindingAPI Apply(const usd::UsdPrim& prim);

    /// Check if this API can be applied to the given prim
    static bool CanApply(const usd::UsdPrim& prim, std::string* whyNot = nullptr);

    // ----- Direct Binding -----

    /// Get the direct binding relationship
    usd::UsdRelationship GetDirectBindingRel(const tf::TfToken& materialPurpose = tf::TfToken()) const;

    /// Bind a material directly
    bool Bind(const UsdShadeMaterial& material,
              const tf::TfToken& bindingStrength = tf::TfToken(),
              const tf::TfToken& materialPurpose = tf::TfToken());

    /// Unbind the direct binding
    bool UnbindDirectBinding(const tf::TfToken& materialPurpose = tf::TfToken());

    /// Unbind all bindings
    bool UnbindAllBindings();

    // ----- Computed Binding -----

    /// Compute the bound material for the given purpose
    UsdShadeMaterial ComputeBoundMaterial(const tf::TfToken& materialPurpose = tf::TfToken(),
                                           usd::UsdRelationship* bindingRel = nullptr) const;

    // ----- Binding Strength -----

    /// Get the material binding strength
    static tf::TfToken GetMaterialBindingStrength(const usd::UsdRelationship& bindingRel);

    /// Set the material binding strength
    static bool SetMaterialBindingStrength(const usd::UsdRelationship& bindingRel,
                                            const tf::TfToken& bindingStrength);

    /// Get the list of material purposes
    static std::vector<tf::TfToken> GetMaterialPurposes();

    /// Get the resolved target path from a binding relationship
    static sdf::SdfPath GetResolvedTargetPathFromBindingRel(const usd::UsdRelationship& bindingRel);

protected:
    usd::UsdSchemaKind _GetSchemaKind() const override;
};

// ============================================================================
// Token Constants
// ============================================================================

/// Common tokens used in UsdShade
namespace UsdShadeTokens {
    // Implementation sources
    tf::TfToken id();
    tf::TfToken sourceAsset();
    tf::TfToken sourceCode();

    // Universal source type
    tf::TfToken universalSourceType();

    // Universal render context
    tf::TfToken universalRenderContext();

    // Connectability
    tf::TfToken full();
    tf::TfToken interfaceOnly();

    // Material purposes
    tf::TfToken allPurpose();
    tf::TfToken preview();
    tf::TfToken fullPurpose();  // Named to avoid conflict with 'full'

    // Binding strength
    tf::TfToken fallbackStrength();
    tf::TfToken strongerThanDescendants();
    tf::TfToken weakerThanDescendants();

    // Terminal names
    tf::TfToken surface();
    tf::TfToken displacement();
    tf::TfToken volume();

    // Inputs/Outputs prefixes
    tf::TfToken inputs();
    tf::TfToken outputs();
}

} // namespace usdShade
} // namespace cxxfacade
