// Usd.hpp - Universal Scene Description facade
#pragma once

#include "../CxxFacade.h"
#include "../base/Tf.hpp"
#include "../base/Vt.hpp"
#include "Sdf.hpp"

namespace cxxfacade {
namespace usd {

// Forward declarations
class UsdStage;
class UsdPrim;
class UsdProperty;
class UsdAttribute;
class UsdRelationship;
class UsdSchemaBase;
class UsdTyped;
class UsdAPISchemaBase;
class UsdEditTarget;
class UsdEditContext;

// ============================================================================
// UsdSchemaKind - Kind of schema
// ============================================================================

/// Enum representing the kind of schema a class is
enum class UsdSchemaKind {
    Invalid,
    AbstractBase,
    AbstractTyped,
    ConcreteTyped,
    NonAppliedAPI,
    SingleApplyAPI,
    MultipleApplyAPI
};

// ============================================================================
// UsdTimeCode - Time value for animation queries
// ============================================================================

/// UsdTimeCode - Time value for animation queries
class UsdTimeCode {
public:
    UsdTimeCode() : time_(0.0), isDefault_(true) {}
    explicit UsdTimeCode(double time) : time_(time), isDefault_(false) {}

    static UsdTimeCode Default() { return UsdTimeCode(); }
    static UsdTimeCode EarliestTime();

    double GetValue() const { return time_; }
    bool IsDefault() const { return isDefault_; }
    bool IsNumeric() const { return !isDefault_; }

    bool operator==(const UsdTimeCode& other) const {
        return time_ == other.time_ && isDefault_ == other.isDefault_;
    }
    bool operator!=(const UsdTimeCode& other) const { return !(*this == other); }

private:
    double time_;
    bool isDefault_;
};

// ============================================================================
// UsdEditTarget - Defines where edits should be directed
// ============================================================================

/// UsdEditTarget - Defines a mapping from scene graph paths to spec paths
/// in an SdfLayer where edits should be directed.
class UsdEditTarget {
public:
    /// Construct a null EditTarget
    UsdEditTarget();

    /// Construct an EditTarget for a layer
    explicit UsdEditTarget(const sdf::SdfLayer& layer);

    /// Copy constructor
    UsdEditTarget(const UsdEditTarget& other);

    /// Move constructor
    UsdEditTarget(UsdEditTarget&& other) noexcept;

    /// Destructor
    ~UsdEditTarget();

    /// Copy assignment
    UsdEditTarget& operator=(const UsdEditTarget& other);

    /// Move assignment
    UsdEditTarget& operator=(UsdEditTarget&& other) noexcept;

    /// Return true if this EditTarget is null
    bool IsNull() const;

    /// Return true if this EditTarget is valid
    bool IsValid() const;

    /// Return the layer this EditTarget contains
    sdf::SdfLayer GetLayer() const;

    /// Map the provided scenePath into a spec path for the EditTarget's layer
    sdf::SdfPath MapToSpecPath(const sdf::SdfPath& scenePath) const;

    /// Comparison operators
    bool operator==(const UsdEditTarget& other) const;
    bool operator!=(const UsdEditTarget& other) const;

    void* GetImpl() const { return impl_; }
    static UsdEditTarget FromImpl(void* impl);

private:
    void* impl_;
};

// ============================================================================
// UsdEditContext - RAII class to temporarily modify edit target
// ============================================================================

/// UsdEditContext - A utility class to temporarily modify a stage's
/// current EditTarget during an execution scope.
class UsdEditContext {
public:
    /// Construct without modifying stage's current EditTarget
    explicit UsdEditContext(UsdStage& stage);

    /// Construct and set a new EditTarget, restoring original on destruction
    UsdEditContext(UsdStage& stage, const UsdEditTarget& editTarget);

    /// Destructor - restores original EditTarget
    ~UsdEditContext();

    /// Non-copyable
    UsdEditContext(const UsdEditContext&) = delete;
    UsdEditContext& operator=(const UsdEditContext&) = delete;

private:
    void* impl_;
};

// ============================================================================
// UsdProperty - Base class for UsdAttribute and UsdRelationship
// ============================================================================

/// UsdProperty - Base class for UsdAttribute and UsdRelationship
class UsdProperty {
public:
    UsdProperty();
    UsdProperty(const UsdProperty& other);
    UsdProperty(UsdProperty&& other) noexcept;
    ~UsdProperty();

    UsdProperty& operator=(const UsdProperty& other);
    UsdProperty& operator=(UsdProperty&& other) noexcept;

    /// Check if this is a valid property reference
    bool IsValid() const;

    /// Get the property's name
    tf::TfToken GetName() const;

    /// Get the property's path
    sdf::SdfPath GetPath() const;

    /// Return this property's name with all namespace prefixes removed
    tf::TfToken GetBaseName() const;

    /// Return this property's complete namespace prefix
    tf::TfToken GetNamespace() const;

    /// Return true if this is a custom property
    bool IsCustom() const;

    /// Return true if this property is defined
    bool IsDefined() const;

    /// Return true if there are any authored opinions for this property
    bool IsAuthored() const;

    /// Get the display group
    std::string GetDisplayGroup() const;

    /// Set the display group
    bool SetDisplayGroup(const std::string& displayGroup);

    /// Check if this property is an attribute
    bool IsAttribute() const;

    /// Check if this property is a relationship
    bool IsRelationship() const;

    bool operator==(const UsdProperty& other) const;
    bool operator!=(const UsdProperty& other) const { return !(*this == other); }

    void* GetImpl() const { return impl_; }
    static UsdProperty FromImpl(void* impl);

protected:
    void* impl_;
};

// ============================================================================
// UsdAttribute - Scene attribute (typed value on a prim)
// ============================================================================

/// UsdAttribute - Scene attribute (typed value on a prim)
class UsdAttribute {
public:
    UsdAttribute();
    UsdAttribute(const UsdAttribute& other);
    UsdAttribute(UsdAttribute&& other) noexcept;
    ~UsdAttribute();

    UsdAttribute& operator=(const UsdAttribute& other);
    UsdAttribute& operator=(UsdAttribute&& other) noexcept;

    bool IsValid() const;
    tf::TfToken GetName() const;
    sdf::SdfPath GetPath() const;

    /// Get the value at the given time
    bool Get(vt::VtValue* value, UsdTimeCode time = UsdTimeCode::Default()) const;

    /// Set the value at the given time
    bool Set(const vt::VtValue& value, UsdTimeCode time = UsdTimeCode::Default());

    /// Get the type name
    std::string GetTypeName() const;

    /// Check if the attribute has any authored values
    bool HasValue() const;

    /// Check if the attribute has an authored value at the given time
    bool HasAuthoredValue() const;

    void* GetImpl() const { return impl_; }
    static UsdAttribute FromImpl(void* impl);

private:
    void* impl_;
};

/// UsdRelationship - Scene relationship (connects prims)
class UsdRelationship {
public:
    UsdRelationship();
    UsdRelationship(const UsdRelationship& other);
    UsdRelationship(UsdRelationship&& other) noexcept;
    ~UsdRelationship();

    UsdRelationship& operator=(const UsdRelationship& other);
    UsdRelationship& operator=(UsdRelationship&& other) noexcept;

    bool IsValid() const;
    tf::TfToken GetName() const;
    sdf::SdfPath GetPath() const;

    /// Get the targets of this relationship
    bool GetTargets(std::vector<sdf::SdfPath>* targets) const;

    /// Set the targets of this relationship
    bool SetTargets(const std::vector<sdf::SdfPath>& targets);

    /// Add a target to this relationship
    bool AddTarget(const sdf::SdfPath& target);

    void* GetImpl() const { return impl_; }
    static UsdRelationship FromImpl(void* impl);

private:
    void* impl_;
};

/// UsdPrim - Scene element (node in the scene graph)
class UsdPrim {
public:
    UsdPrim();
    UsdPrim(const UsdPrim& other);
    UsdPrim(UsdPrim&& other) noexcept;
    ~UsdPrim();

    UsdPrim& operator=(const UsdPrim& other);
    UsdPrim& operator=(UsdPrim&& other) noexcept;

    /// Check if this is a valid prim reference
    bool IsValid() const;

    /// Check if this prim is active
    bool IsActive() const;

    /// Check if this prim is defined
    bool IsDefined() const;

    /// Get the prim's name
    tf::TfToken GetName() const;

    /// Get the prim's path
    sdf::SdfPath GetPath() const;

    /// Get the prim's type name
    tf::TfToken GetTypeName() const;

    /// Get the parent prim
    UsdPrim GetParent() const;

    /// Get the stage this prim belongs to
    UsdStage GetStage() const;

    /// Get all children
    std::vector<UsdPrim> GetChildren() const;

    /// Get an attribute by name
    UsdAttribute GetAttribute(const tf::TfToken& name) const;

    /// Create a new attribute
    UsdAttribute CreateAttribute(const tf::TfToken& name,
                                  const std::string& typeName,
                                  bool custom = true);

    /// Get a relationship by name
    UsdRelationship GetRelationship(const tf::TfToken& name) const;

    /// Create a new relationship
    UsdRelationship CreateRelationship(const tf::TfToken& name,
                                        bool custom = true);

    /// Check if this prim has a specific API schema applied
    bool HasAPI(const tf::TfToken& schemaName) const;

    /// Apply an API schema
    bool ApplyAPI(const tf::TfToken& schemaName);

    bool operator==(const UsdPrim& other) const;
    bool operator!=(const UsdPrim& other) const { return !(*this == other); }

    void* GetImpl() const { return impl_; }
    static UsdPrim FromImpl(void* impl);

private:
    void* impl_;
};

/// UsdStage - USD scene container
class UsdStage {
public:
    UsdStage();
    UsdStage(const UsdStage& other);
    UsdStage(UsdStage&& other) noexcept;
    ~UsdStage();

    UsdStage& operator=(const UsdStage& other);
    UsdStage& operator=(UsdStage&& other) noexcept;

    /// Check if this is a valid stage reference
    bool IsValid() const;

    /// Create a new in-memory stage
    static UsdStage CreateInMemory();

    /// Create a new stage with a root layer
    static UsdStage CreateNew(const std::string& identifier);

    /// Open an existing stage
    static UsdStage Open(const std::string& identifier);

    /// Get the root layer
    sdf::SdfLayer GetRootLayer() const;

    /// Get the pseudo-root prim
    UsdPrim GetPseudoRoot() const;

    /// Get the default prim
    UsdPrim GetDefaultPrim() const;

    /// Set the default prim
    bool SetDefaultPrim(const UsdPrim& prim);

    /// Get a prim by path
    UsdPrim GetPrimAtPath(const sdf::SdfPath& path) const;

    /// Define a prim at the given path
    UsdPrim DefinePrim(const sdf::SdfPath& path, const tf::TfToken& typeName = tf::TfToken());

    /// Override a prim at the given path
    UsdPrim OverridePrim(const sdf::SdfPath& path);

    /// Remove a prim
    bool RemovePrim(const sdf::SdfPath& path);

    /// Traverse all prims
    std::vector<UsdPrim> Traverse() const;

    /// Get the stage's up-axis
    tf::TfToken GetUpAxis() const;

    /// Set the stage's up-axis
    bool SetUpAxis(const tf::TfToken& axis);

    /// Get the stage's meters per unit
    double GetMetersPerUnit() const;

    /// Set the stage's meters per unit
    bool SetMetersPerUnit(double metersPerUnit);

    /// Get the start time code
    double GetStartTimeCode() const;

    /// Get the end time code
    double GetEndTimeCode() const;

    /// Set the time code range
    void SetStartTimeCode(double startTime);
    void SetEndTimeCode(double endTime);

    /// Save the stage
    bool Save() const;

    /// Export to a file
    bool Export(const std::string& filename) const;

    /// Get the current edit target
    UsdEditTarget GetEditTarget() const;

    /// Set the edit target
    void SetEditTarget(const UsdEditTarget& editTarget);

    /// Get the session layer
    sdf::SdfLayer GetSessionLayer() const;

    /// Get an edit target for the given layer
    UsdEditTarget GetEditTargetForLocalLayer(const sdf::SdfLayer& layer) const;

    void* GetImpl() const { return impl_; }
    static UsdStage FromImpl(void* impl);

private:
    void* impl_;
};

// ============================================================================
// UsdSchemaBase - Base class for all schema types
// ============================================================================

/// UsdSchemaBase - The base class for all schema types in Usd.
///
/// Schema objects hold a UsdPrim internally and provide a layer of specific
/// named API atop the underlying scene graph.
class UsdSchemaBase {
public:
    /// Compile time constant representing what kind of schema this class is
    static const UsdSchemaKind schemaKind = UsdSchemaKind::AbstractBase;

    /// Construct a UsdSchemaBase on UsdPrim
    explicit UsdSchemaBase(const UsdPrim& prim = UsdPrim());

    /// Copy constructor
    UsdSchemaBase(const UsdSchemaBase& otherSchema);

    /// Destructor
    virtual ~UsdSchemaBase();

    /// Return the held prim
    UsdPrim GetPrim() const;

    /// Get the prim's path
    sdf::SdfPath GetPath() const;

    /// Returns whether or not this class corresponds to a concrete prim type
    bool IsConcrete() const;

    /// Returns whether or not this class inherits from UsdTyped
    bool IsTyped() const;

    /// Returns whether this is an API schema
    bool IsAPISchema() const;

    /// Returns whether this is an applied API schema
    bool IsAppliedAPISchema() const;

    /// Returns whether this is a multiple-apply API schema
    bool IsMultipleApplyAPISchema() const;

    /// Returns the kind of schema this class is
    UsdSchemaKind GetSchemaKind() const;

    /// Return true if this schema object is valid
    explicit operator bool() const;

    void* GetImpl() const { return impl_; }
    static UsdSchemaBase FromImpl(void* impl);

protected:
    virtual UsdSchemaKind _GetSchemaKind() const;

    void* impl_;
};

// ============================================================================
// UsdTyped - Base class for typed schemas
// ============================================================================

/// UsdTyped - The base class for all typed schemas.
///
/// Typed schemas can impart a typeName to a UsdPrim, and therefore
/// are the base class for all concrete, instantiable "IsA" schemas.
class UsdTyped : public UsdSchemaBase {
public:
    /// Compile time constant
    static const UsdSchemaKind schemaKind = UsdSchemaKind::AbstractBase;

    /// Construct a UsdTyped on UsdPrim
    explicit UsdTyped(const UsdPrim& prim = UsdPrim());

    /// Construct from another schema
    explicit UsdTyped(const UsdSchemaBase& schemaObj);

    /// Destructor
    virtual ~UsdTyped();

    /// Return a UsdTyped holding the prim at path on stage
    static UsdTyped Get(const UsdStage& stage, const sdf::SdfPath& path);

protected:
    UsdSchemaKind _GetSchemaKind() const override;
};

// ============================================================================
// UsdAPISchemaBase - Base class for API schemas
// ============================================================================

/// UsdAPISchemaBase - The base class for all API schemas.
///
/// An API schema provides an interface to a prim's qualities, but does not
/// specify a typeName for the underlying prim.
class UsdAPISchemaBase : public UsdSchemaBase {
public:
    /// Compile time constant
    static const UsdSchemaKind schemaKind = UsdSchemaKind::AbstractBase;

    /// Construct a UsdAPISchemaBase on UsdPrim
    explicit UsdAPISchemaBase(const UsdPrim& prim = UsdPrim());

    /// Construct from another schema
    explicit UsdAPISchemaBase(const UsdSchemaBase& schemaObj);

    /// Destructor
    virtual ~UsdAPISchemaBase();

protected:
    /// Construct for multiple-apply API schema with instance name
    explicit UsdAPISchemaBase(const UsdPrim& prim, const tf::TfToken& instanceName);

    /// Construct from another schema with instance name
    explicit UsdAPISchemaBase(const UsdSchemaBase& schemaObj, const tf::TfToken& instanceName);

    /// Returns the instance name for multiple-apply API schemas
    tf::TfToken GetInstanceName() const;

    UsdSchemaKind _GetSchemaKind() const override;

private:
    tf::TfToken instanceName_;
};

} // namespace usd
} // namespace cxxfacade
