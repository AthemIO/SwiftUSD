// PxOsd.swift - OpenSubdiv Integration wrapper
// Mirrors: pxr/imaging/pxOsd/*.h (Subdivision surfaces)

import OpenUSDInterop

// MARK: - PxOsd Namespace

/// OpenSubdiv integration for subdivision surfaces.
/// Mirrors pxr::PxOsd from the USD imaging library.
///
/// PxOsd provides:
/// - `SubdivTags`: Subdivision surface parameters (creases, corners, interpolation rules)
/// - `MeshTopology`: Complete mesh connectivity and subdivision data
/// - `MeshTopologyValidation`: Validation result for mesh topology
///
/// ## Usage
/// ```swift
/// // Create a simple quad mesh topology
/// let topology = try PxOsd.MeshTopology(
///     scheme: .catmullClark,
///     orientation: .rightHanded,
///     faceVertexCounts: [4],
///     faceVertexIndices: [0, 1, 2, 3]
/// )
///
/// // Validate the topology
/// let validation = topology.validate()
/// if validation.isValid {
///     print("Topology is valid")
/// }
/// ```
public enum PxOsd {

    // MARK: - Subdivision Scheme

    /// Subdivision scheme for mesh subdivision.
    /// Mirrors PxOsdOpenSubdivTokens for subdivision schemes.
    public enum SubdivScheme: UInt32, Sendable, CustomStringConvertible {
        /// Catmull-Clark subdivision (default for quads).
        case catmullClark = 0
        /// Loop subdivision (for triangles).
        case loop = 1
        /// Bilinear interpolation (no subdivision).
        case bilinear = 2

        public var description: String {
            String(cString: PxOsd_GetSchemeString(PxOsdSubdivScheme(rawValue: rawValue)))
        }

        internal init(_ cValue: PxOsdSubdivScheme) {
            self = SubdivScheme(rawValue: cValue.rawValue) ?? .bilinear
        }

        internal var cValue: PxOsdSubdivScheme {
            PxOsdSubdivScheme(rawValue: rawValue)
        }
    }

    // MARK: - Orientation

    /// Mesh face orientation (winding order).
    public enum Orientation: UInt32, Sendable, CustomStringConvertible {
        /// Right-handed orientation (counter-clockwise, default).
        case rightHanded = 0
        /// Left-handed orientation (clockwise).
        case leftHanded = 1

        public var description: String {
            String(cString: PxOsd_GetOrientationString(PxOsdOrientation(rawValue: rawValue)))
        }

        internal init(_ cValue: PxOsdOrientation) {
            self = Orientation(rawValue: cValue.rawValue) ?? .rightHanded
        }

        internal var cValue: PxOsdOrientation {
            PxOsdOrientation(rawValue: rawValue)
        }
    }

    // MARK: - Vertex Interpolation Rule

    /// Vertex interpolation rule for boundary handling.
    public enum VertexInterpolationRule: UInt32, Sendable, CustomStringConvertible {
        /// No boundary interpolation.
        case none = 0
        /// Smooth edges only.
        case edgeOnly = 1
        /// Smooth edges and corners.
        case edgeAndCorner = 2

        public var description: String {
            String(cString: PxOsd_GetVertexInterpolationRuleString(PxOsdVertexInterpolationRule(rawValue: rawValue)))
        }

        internal init(_ cValue: PxOsdVertexInterpolationRule) {
            self = VertexInterpolationRule(rawValue: cValue.rawValue) ?? .edgeAndCorner
        }

        internal var cValue: PxOsdVertexInterpolationRule {
            PxOsdVertexInterpolationRule(rawValue: rawValue)
        }
    }

    // MARK: - Face-Varying Interpolation Rule

    /// Face-varying interpolation rule for UV boundaries.
    public enum FaceVaryingInterpolationRule: UInt32, Sendable, CustomStringConvertible {
        /// No face-varying interpolation.
        case none = 0
        /// Corners only.
        case cornersOnly = 1
        /// Corners plus one ring.
        case cornersPlus1 = 2
        /// Corners plus two rings.
        case cornersPlus2 = 3
        /// Boundaries.
        case boundaries = 4
        /// All (default).
        case all = 5

        public var description: String {
            String(cString: PxOsd_GetFaceVaryingInterpolationRuleString(PxOsdFaceVaryingInterpolationRule(rawValue: rawValue)))
        }

        internal init(_ cValue: PxOsdFaceVaryingInterpolationRule) {
            self = FaceVaryingInterpolationRule(rawValue: cValue.rawValue) ?? .cornersPlus1
        }

        internal var cValue: PxOsdFaceVaryingInterpolationRule {
            PxOsdFaceVaryingInterpolationRule(rawValue: rawValue)
        }
    }

    // MARK: - Crease Method

    /// Crease computation method.
    public enum CreaseMethod: UInt32, Sendable, CustomStringConvertible {
        /// Uniform crease weights.
        case uniform = 0
        /// Chaikin crease interpolation.
        case chaikin = 1

        public var description: String {
            String(cString: PxOsd_GetCreaseMethodString(PxOsdCreaseMethod(rawValue: rawValue)))
        }

        internal init(_ cValue: PxOsdCreaseMethod) {
            self = CreaseMethod(rawValue: cValue.rawValue) ?? .uniform
        }

        internal var cValue: PxOsdCreaseMethod {
            PxOsdCreaseMethod(rawValue: rawValue)
        }
    }

    // MARK: - Triangle Subdivision

    /// Triangle subdivision rule.
    public enum TriangleSubdivision: UInt32, Sendable, CustomStringConvertible {
        /// Catmull-Clark style.
        case catmullClark = 0
        /// Smooth interpolation.
        case smooth = 1

        public var description: String {
            String(cString: PxOsd_GetTriangleSubdivisionString(PxOsdTriangleSubdivision(rawValue: rawValue)))
        }

        internal init(_ cValue: PxOsdTriangleSubdivision) {
            self = TriangleSubdivision(rawValue: cValue.rawValue) ?? .catmullClark
        }

        internal var cValue: PxOsdTriangleSubdivision {
            PxOsdTriangleSubdivision(rawValue: rawValue)
        }
    }

    // MARK: - Validation Code

    /// Validation error code for mesh topology.
    public enum ValidationCode: UInt32, Sendable, CustomStringConvertible {
        case ok = 0
        case invalidScheme
        case invalidOrientation
        case invalidTriangleSubdivision
        case invalidVertexInterpolationRule
        case invalidFaceVaryingInterpolationRule
        case invalidCreaseMethod
        case invalidCreaseLengthElement
        case invalidCreaseIndicesSize
        case invalidCreaseIndicesElement
        case invalidCreaseWeightsSize
        case negativeCreaseWeights
        case invalidCornerIndicesElement
        case invalidCornerWeightsSize
        case negativeCornerWeights
        case invalidHoleIndicesElement
        case invalidFaceVertexCountsElement
        case invalidFaceVertexIndicesSize
        case invalidFaceVertexIndicesElement

        public var description: String {
            switch self {
            case .ok: return "OK"
            case .invalidScheme: return "Invalid scheme"
            case .invalidOrientation: return "Invalid orientation"
            case .invalidTriangleSubdivision: return "Invalid triangle subdivision"
            case .invalidVertexInterpolationRule: return "Invalid vertex interpolation rule"
            case .invalidFaceVaryingInterpolationRule: return "Invalid face-varying interpolation rule"
            case .invalidCreaseMethod: return "Invalid crease method"
            case .invalidCreaseLengthElement: return "Invalid crease length element"
            case .invalidCreaseIndicesSize: return "Invalid crease indices size"
            case .invalidCreaseIndicesElement: return "Invalid crease indices element"
            case .invalidCreaseWeightsSize: return "Invalid crease weights size"
            case .negativeCreaseWeights: return "Negative crease weights"
            case .invalidCornerIndicesElement: return "Invalid corner indices element"
            case .invalidCornerWeightsSize: return "Invalid corner weights size"
            case .negativeCornerWeights: return "Negative corner weights"
            case .invalidHoleIndicesElement: return "Invalid hole indices element"
            case .invalidFaceVertexCountsElement: return "Invalid face vertex counts element"
            case .invalidFaceVertexIndicesSize: return "Invalid face vertex indices size"
            case .invalidFaceVertexIndicesElement: return "Invalid face vertex indices element"
            }
        }

        internal init(_ cValue: PxOsdValidationCode) {
            self = ValidationCode(rawValue: cValue.rawValue) ?? .ok
        }
    }
}

// MARK: - PxOsd Error

/// Errors that can occur in PxOsd operations.
public enum PxOsdError: Error, CustomStringConvertible {
    case creationFailed(String)
    case invalidArgument(String)
    case validationFailed([PxOsd.ValidationError])

    public var description: String {
        switch self {
        case .creationFailed(let message):
            return "PxOsd creation failed: \(message)"
        case .invalidArgument(let message):
            return "PxOsd invalid argument: \(message)"
        case .validationFailed(let errors):
            let messages = errors.map { "\($0.code): \($0.message)" }.joined(separator: "; ")
            return "PxOsd validation failed: \(messages)"
        }
    }
}

// MARK: - Validation Error

extension PxOsd {

    /// A validation error with code and message.
    public struct ValidationError: Sendable, CustomStringConvertible {
        public let code: ValidationCode
        public let message: String

        public var description: String {
            "\(code): \(message)"
        }

        internal init(_ cError: PxOsdValidationError) {
            self.code = ValidationCode(cError.code)
            self.message = withUnsafePointer(to: cError.message) { ptr in
                ptr.withMemoryRebound(to: CChar.self, capacity: 256) {
                    String(cString: $0)
                }
            }
        }
    }
}

// MARK: - SubdivTags

extension PxOsd {

    /// Subdivision surface parameters including creases, corners, and interpolation rules.
    ///
    /// SubdivTags controls how subdivision surfaces are computed:
    /// - Vertex and face-varying interpolation rules
    /// - Crease edges and their sharpness weights
    /// - Corner vertices and their sharpness weights
    ///
    /// ## Usage
    /// ```swift
    /// var tags = PxOsd.SubdivTags()
    ///
    /// // Set interpolation rules
    /// tags.vertexInterpolationRule = .edgeAndCorner
    /// tags.faceVaryingInterpolationRule = .cornersPlus1
    ///
    /// // Add a crease edge between vertices 0 and 1
    /// tags.creaseLengths = [2]
    /// tags.creaseIndices = [0, 1]
    /// tags.creaseWeights = [3.0]  // Sharpness of 3
    ///
    /// // Add a corner at vertex 5
    /// tags.cornerIndices = [5]
    /// tags.cornerWeights = [2.0]  // Sharpness of 2
    /// ```
    public final class SubdivTags: @unchecked Sendable {
        internal let handle: PxOsdSubdivTagsRef

        // MARK: Lifecycle

        /// Creates empty subdivision tags with default values.
        public init() throws {
            guard let handle = PxOsdSubdivTags_Create() else {
                throw PxOsdError.creationFailed("Failed to create SubdivTags")
            }
            self.handle = handle
        }

        internal init(handle: PxOsdSubdivTagsRef) {
            self.handle = handle
        }

        deinit {
            PxOsdSubdivTags_Release(handle)
        }

        /// Creates a copy of this SubdivTags.
        public func copy() throws -> SubdivTags {
            guard let newHandle = PxOsdSubdivTags_Copy(handle) else {
                throw PxOsdError.creationFailed("Failed to copy SubdivTags")
            }
            return SubdivTags(handle: newHandle)
        }

        // MARK: Interpolation Rules

        /// Vertex interpolation rule for boundary handling.
        public var vertexInterpolationRule: VertexInterpolationRule {
            get {
                VertexInterpolationRule(PxOsdSubdivTags_GetVertexInterpolationRule(handle))
            }
            set {
                PxOsdSubdivTags_SetVertexInterpolationRule(handle, newValue.cValue)
            }
        }

        /// Face-varying interpolation rule for UV boundaries.
        public var faceVaryingInterpolationRule: FaceVaryingInterpolationRule {
            get {
                FaceVaryingInterpolationRule(PxOsdSubdivTags_GetFaceVaryingInterpolationRule(handle))
            }
            set {
                PxOsdSubdivTags_SetFaceVaryingInterpolationRule(handle, newValue.cValue)
            }
        }

        /// Crease computation method.
        public var creaseMethod: CreaseMethod {
            get {
                CreaseMethod(PxOsdSubdivTags_GetCreaseMethod(handle))
            }
            set {
                PxOsdSubdivTags_SetCreaseMethod(handle, newValue.cValue)
            }
        }

        /// Triangle subdivision rule.
        public var triangleSubdivision: TriangleSubdivision {
            get {
                TriangleSubdivision(PxOsdSubdivTags_GetTriangleSubdivision(handle))
            }
            set {
                PxOsdSubdivTags_SetTriangleSubdivision(handle, newValue.cValue)
            }
        }

        // MARK: Crease Data

        /// Number of vertices in each crease chain.
        public var creaseLengths: [Int32] {
            get {
                let count = Int(PxOsdSubdivTags_GetCreaseLengthCount(handle))
                guard count > 0 else { return [] }
                var buffer = [Int32](repeating: 0, count: count)
                let copied = buffer.withUnsafeMutableBufferPointer { ptr in
                    PxOsdSubdivTags_GetCreaseLengths(handle, ptr.baseAddress!, count)
                }
                return copied < count ? Array(buffer.prefix(Int(copied))) : buffer
            }
            set {
                newValue.withUnsafeBufferPointer { ptr in
                    PxOsdSubdivTags_SetCreaseLengths(handle, ptr.baseAddress, ptr.count)
                }
            }
        }

        /// Vertex indices that form crease edges.
        public var creaseIndices: [Int32] {
            get {
                let count = Int(PxOsdSubdivTags_GetCreaseIndexCount(handle))
                guard count > 0 else { return [] }
                var buffer = [Int32](repeating: 0, count: count)
                let copied = buffer.withUnsafeMutableBufferPointer { ptr in
                    PxOsdSubdivTags_GetCreaseIndices(handle, ptr.baseAddress!, count)
                }
                return copied < count ? Array(buffer.prefix(Int(copied))) : buffer
            }
            set {
                newValue.withUnsafeBufferPointer { ptr in
                    PxOsdSubdivTags_SetCreaseIndices(handle, ptr.baseAddress, ptr.count)
                }
            }
        }

        /// Sharpness weights for creases (per-chain or per-edge).
        public var creaseWeights: [Float] {
            get {
                let count = Int(PxOsdSubdivTags_GetCreaseWeightCount(handle))
                guard count > 0 else { return [] }
                var buffer = [Float](repeating: 0, count: count)
                let copied = buffer.withUnsafeMutableBufferPointer { ptr in
                    PxOsdSubdivTags_GetCreaseWeights(handle, ptr.baseAddress!, count)
                }
                return copied < count ? Array(buffer.prefix(Int(copied))) : buffer
            }
            set {
                newValue.withUnsafeBufferPointer { ptr in
                    PxOsdSubdivTags_SetCreaseWeights(handle, ptr.baseAddress, ptr.count)
                }
            }
        }

        // MARK: Corner Data

        /// Vertex indices marked as corners.
        public var cornerIndices: [Int32] {
            get {
                let count = Int(PxOsdSubdivTags_GetCornerIndexCount(handle))
                guard count > 0 else { return [] }
                var buffer = [Int32](repeating: 0, count: count)
                let copied = buffer.withUnsafeMutableBufferPointer { ptr in
                    PxOsdSubdivTags_GetCornerIndices(handle, ptr.baseAddress!, count)
                }
                return copied < count ? Array(buffer.prefix(Int(copied))) : buffer
            }
            set {
                newValue.withUnsafeBufferPointer { ptr in
                    PxOsdSubdivTags_SetCornerIndices(handle, ptr.baseAddress, ptr.count)
                }
            }
        }

        /// Sharpness weights for corners.
        public var cornerWeights: [Float] {
            get {
                let count = Int(PxOsdSubdivTags_GetCornerWeightCount(handle))
                guard count > 0 else { return [] }
                var buffer = [Float](repeating: 0, count: count)
                let copied = buffer.withUnsafeMutableBufferPointer { ptr in
                    PxOsdSubdivTags_GetCornerWeights(handle, ptr.baseAddress!, count)
                }
                return copied < count ? Array(buffer.prefix(Int(copied))) : buffer
            }
            set {
                newValue.withUnsafeBufferPointer { ptr in
                    PxOsdSubdivTags_SetCornerWeights(handle, ptr.baseAddress, ptr.count)
                }
            }
        }

        // MARK: Hashing

        /// Computes a hash value for caching/instancing.
        public func computeHash() -> Int {
            Int(PxOsdSubdivTags_ComputeHash(handle))
        }
    }
}

// MARK: - SubdivTags Equatable

extension PxOsd.SubdivTags: Equatable {
    public static func == (lhs: PxOsd.SubdivTags, rhs: PxOsd.SubdivTags) -> Bool {
        PxOsdSubdivTags_Equal(lhs.handle, rhs.handle)
    }
}

// MARK: - SubdivTags CustomStringConvertible

extension PxOsd.SubdivTags: CustomStringConvertible {
    public var description: String {
        "SubdivTags(vertex: \(vertexInterpolationRule), fvar: \(faceVaryingInterpolationRule), creases: \(creaseLengths.count), corners: \(cornerIndices.count))"
    }
}

// MARK: - MeshTopology

extension PxOsd {

    /// Immutable container for complete mesh connectivity and subdivision parameters.
    ///
    /// MeshTopology holds:
    /// - Subdivision scheme (Catmull-Clark, Loop, bilinear)
    /// - Face orientation (winding order)
    /// - Face vertex counts and indices
    /// - Optional hole indices
    /// - Optional subdivision tags
    ///
    /// ## Usage
    /// ```swift
    /// // Create a cube topology (6 quad faces)
    /// let counts: [Int32] = [4, 4, 4, 4, 4, 4]
    /// let indices: [Int32] = [
    ///     0, 1, 2, 3,  // front
    ///     4, 5, 6, 7,  // back
    ///     0, 4, 5, 1,  // bottom
    ///     2, 6, 7, 3,  // top
    ///     0, 3, 7, 4,  // left
    ///     1, 5, 6, 2   // right
    /// ]
    ///
    /// let topology = try PxOsd.MeshTopology(
    ///     scheme: .catmullClark,
    ///     orientation: .rightHanded,
    ///     faceVertexCounts: counts,
    ///     faceVertexIndices: indices
    /// )
    /// ```
    public final class MeshTopology: @unchecked Sendable {
        internal let handle: PxOsdMeshTopologyRef

        // MARK: Lifecycle

        /// Creates an empty mesh topology with default values (bilinear, right-handed).
        public init() throws {
            guard let handle = PxOsdMeshTopology_Create() else {
                throw PxOsdError.creationFailed("Failed to create MeshTopology")
            }
            self.handle = handle
        }

        /// Creates a mesh topology with the specified parameters.
        ///
        /// - Parameters:
        ///   - scheme: Subdivision scheme.
        ///   - orientation: Face winding order.
        ///   - faceVertexCounts: Number of vertices per face.
        ///   - faceVertexIndices: Vertex indices for all faces (flattened).
        public init(
            scheme: SubdivScheme,
            orientation: Orientation,
            faceVertexCounts: [Int32],
            faceVertexIndices: [Int32]
        ) throws {
            guard let handle = faceVertexCounts.withUnsafeBufferPointer({ counts in
                faceVertexIndices.withUnsafeBufferPointer { indices in
                    PxOsdMeshTopology_CreateWithTopology(
                        scheme.cValue,
                        orientation.cValue,
                        counts.baseAddress,
                        counts.count,
                        indices.baseAddress,
                        indices.count
                    )
                }
            }) else {
                throw PxOsdError.creationFailed("Failed to create MeshTopology with topology")
            }
            self.handle = handle
        }

        /// Creates a mesh topology with subdivision tags.
        public init(
            scheme: SubdivScheme,
            orientation: Orientation,
            faceVertexCounts: [Int32],
            faceVertexIndices: [Int32],
            subdivTags: SubdivTags
        ) throws {
            guard let handle = faceVertexCounts.withUnsafeBufferPointer({ counts in
                faceVertexIndices.withUnsafeBufferPointer { indices in
                    PxOsdMeshTopology_CreateWithSubdivTags(
                        scheme.cValue,
                        orientation.cValue,
                        counts.baseAddress,
                        counts.count,
                        indices.baseAddress,
                        indices.count,
                        subdivTags.handle
                    )
                }
            }) else {
                throw PxOsdError.creationFailed("Failed to create MeshTopology with subdiv tags")
            }
            self.handle = handle
        }

        /// Creates a mesh topology with hole indices.
        public init(
            scheme: SubdivScheme,
            orientation: Orientation,
            faceVertexCounts: [Int32],
            faceVertexIndices: [Int32],
            holeIndices: [Int32]
        ) throws {
            guard let handle = faceVertexCounts.withUnsafeBufferPointer({ counts in
                faceVertexIndices.withUnsafeBufferPointer { indices in
                    holeIndices.withUnsafeBufferPointer { holes in
                        PxOsdMeshTopology_CreateWithHoles(
                            scheme.cValue,
                            orientation.cValue,
                            counts.baseAddress,
                            counts.count,
                            indices.baseAddress,
                            indices.count,
                            holes.baseAddress,
                            holes.count
                        )
                    }
                }
            }) else {
                throw PxOsdError.creationFailed("Failed to create MeshTopology with holes")
            }
            self.handle = handle
        }

        /// Creates a mesh topology with holes and subdivision tags.
        public init(
            scheme: SubdivScheme,
            orientation: Orientation,
            faceVertexCounts: [Int32],
            faceVertexIndices: [Int32],
            holeIndices: [Int32],
            subdivTags: SubdivTags
        ) throws {
            guard let handle = faceVertexCounts.withUnsafeBufferPointer({ counts in
                faceVertexIndices.withUnsafeBufferPointer { indices in
                    holeIndices.withUnsafeBufferPointer { holes in
                        PxOsdMeshTopology_CreateFull(
                            scheme.cValue,
                            orientation.cValue,
                            counts.baseAddress,
                            counts.count,
                            indices.baseAddress,
                            indices.count,
                            holes.baseAddress,
                            holes.count,
                            subdivTags.handle
                        )
                    }
                }
            }) else {
                throw PxOsdError.creationFailed("Failed to create MeshTopology with full params")
            }
            self.handle = handle
        }

        internal init(handle: PxOsdMeshTopologyRef) {
            self.handle = handle
        }

        deinit {
            PxOsdMeshTopology_Release(handle)
        }

        /// Creates a copy of this MeshTopology.
        public func copy() throws -> MeshTopology {
            guard let newHandle = PxOsdMeshTopology_Copy(handle) else {
                throw PxOsdError.creationFailed("Failed to copy MeshTopology")
            }
            return MeshTopology(handle: newHandle)
        }

        // MARK: Properties

        /// The subdivision scheme.
        public var scheme: SubdivScheme {
            SubdivScheme(PxOsdMeshTopology_GetScheme(handle))
        }

        /// The face orientation (winding order).
        public var orientation: Orientation {
            Orientation(PxOsdMeshTopology_GetOrientation(handle))
        }

        /// The number of faces.
        public var faceCount: Int {
            Int(PxOsdMeshTopology_GetFaceCount(handle))
        }

        /// The face vertex counts (number of vertices per face).
        public var faceVertexCounts: [Int32] {
            let count = faceCount
            guard count > 0 else { return [] }
            var buffer = [Int32](repeating: 0, count: count)
            let copied = buffer.withUnsafeMutableBufferPointer { ptr in
                PxOsdMeshTopology_GetFaceVertexCounts(handle, ptr.baseAddress!, count)
            }
            return copied < count ? Array(buffer.prefix(Int(copied))) : buffer
        }

        /// The total number of face vertex indices.
        public var faceVertexIndexCount: Int {
            Int(PxOsdMeshTopology_GetFaceVertexIndexCount(handle))
        }

        /// The face vertex indices.
        public var faceVertexIndices: [Int32] {
            let count = faceVertexIndexCount
            guard count > 0 else { return [] }
            var buffer = [Int32](repeating: 0, count: count)
            let copied = buffer.withUnsafeMutableBufferPointer { ptr in
                PxOsdMeshTopology_GetFaceVertexIndices(handle, ptr.baseAddress!, count)
            }
            return copied < count ? Array(buffer.prefix(Int(copied))) : buffer
        }

        /// The number of hole indices.
        public var holeIndexCount: Int {
            Int(PxOsdMeshTopology_GetHoleIndexCount(handle))
        }

        /// The hole indices (face indices to treat as holes).
        public var holeIndices: [Int32] {
            let count = holeIndexCount
            guard count > 0 else { return [] }
            var buffer = [Int32](repeating: 0, count: count)
            let copied = buffer.withUnsafeMutableBufferPointer { ptr in
                PxOsdMeshTopology_GetHoleIndices(handle, ptr.baseAddress!, count)
            }
            return copied < count ? Array(buffer.prefix(Int(copied))) : buffer
        }

        /// The subdivision tags, or nil if none set.
        public var subdivTags: SubdivTags? {
            guard let tagsHandle = PxOsdMeshTopology_GetSubdivTags(handle) else {
                return nil
            }
            return SubdivTags(handle: tagsHandle)
        }

        // MARK: Builder Methods (Return Modified Copies)

        /// Returns a copy with a different subdivision scheme.
        public func withScheme(_ scheme: SubdivScheme) throws -> MeshTopology {
            guard let newHandle = PxOsdMeshTopology_WithScheme(handle, scheme.cValue) else {
                throw PxOsdError.creationFailed("Failed to create MeshTopology with new scheme")
            }
            return MeshTopology(handle: newHandle)
        }

        /// Returns a copy with a different orientation.
        public func withOrientation(_ orientation: Orientation) throws -> MeshTopology {
            guard let newHandle = PxOsdMeshTopology_WithOrientation(handle, orientation.cValue) else {
                throw PxOsdError.creationFailed("Failed to create MeshTopology with new orientation")
            }
            return MeshTopology(handle: newHandle)
        }

        /// Returns a copy with different subdivision tags.
        public func withSubdivTags(_ subdivTags: SubdivTags?) throws -> MeshTopology {
            guard let newHandle = PxOsdMeshTopology_WithSubdivTags(handle, subdivTags?.handle) else {
                throw PxOsdError.creationFailed("Failed to create MeshTopology with new subdiv tags")
            }
            return MeshTopology(handle: newHandle)
        }

        /// Returns a copy with different hole indices.
        public func withHoleIndices(_ holeIndices: [Int32]) throws -> MeshTopology {
            guard let newHandle = holeIndices.withUnsafeBufferPointer({ holes in
                PxOsdMeshTopology_WithHoleIndices(handle, holes.baseAddress, holes.count)
            }) else {
                throw PxOsdError.creationFailed("Failed to create MeshTopology with new hole indices")
            }
            return MeshTopology(handle: newHandle)
        }

        // MARK: Validation

        /// Validates the mesh topology and returns a validation result.
        public func validate() -> MeshTopologyValidation {
            guard let validationHandle = PxOsdMeshTopology_Validate(handle) else {
                // Return an invalid validation with an error
                return MeshTopologyValidation(isValid: false, errors: [
                    ValidationError(PxOsdValidationError(code: PXOSD_VALIDATION_OK, message: (0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)))
                ])
            }
            return MeshTopologyValidation(handle: validationHandle)
        }

        // MARK: Hashing

        /// Computes a hash value for caching/instancing.
        public func computeHash() -> Int {
            Int(PxOsdMeshTopology_ComputeHash(handle))
        }
    }
}

// MARK: - MeshTopology Equatable

extension PxOsd.MeshTopology: Equatable {
    public static func == (lhs: PxOsd.MeshTopology, rhs: PxOsd.MeshTopology) -> Bool {
        PxOsdMeshTopology_Equal(lhs.handle, rhs.handle)
    }
}

// MARK: - MeshTopology CustomStringConvertible

extension PxOsd.MeshTopology: CustomStringConvertible {
    public var description: String {
        "MeshTopology(scheme: \(scheme), orientation: \(orientation), faces: \(faceCount), indices: \(faceVertexIndexCount), holes: \(holeIndexCount))"
    }
}

// MARK: - MeshTopologyValidation

extension PxOsd {

    /// Validation result for mesh topology.
    public final class MeshTopologyValidation: @unchecked Sendable {
        private var handle: PxOsdMeshTopologyValidationRef?
        private var _isValid: Bool
        private var _errors: [ValidationError]

        internal init(handle: PxOsdMeshTopologyValidationRef) {
            self.handle = handle
            self._isValid = PxOsdMeshTopologyValidation_IsValid(handle)

            // Load errors
            let errorCount = Int(PxOsdMeshTopologyValidation_GetErrorCount(handle))
            if errorCount > 0 {
                var buffer = [PxOsdValidationError](repeating: PxOsdValidationError(code: PXOSD_VALIDATION_OK, message: (0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)), count: errorCount)
                let copied = buffer.withUnsafeMutableBufferPointer { ptr in
                    PxOsdMeshTopologyValidation_GetErrors(handle, ptr.baseAddress!, errorCount)
                }
                self._errors = buffer.prefix(Int(copied)).map { ValidationError($0) }
            } else {
                self._errors = []
            }
        }

        internal init(isValid: Bool, errors: [ValidationError]) {
            self.handle = nil
            self._isValid = isValid
            self._errors = errors
        }

        deinit {
            if let handle = handle {
                PxOsdMeshTopologyValidation_Release(handle)
            }
        }

        /// Returns true if the validation passed (no errors).
        public var isValid: Bool {
            _isValid
        }

        /// The validation errors, if any.
        public var errors: [ValidationError] {
            _errors
        }
    }
}

// MARK: - MeshTopologyValidation CustomStringConvertible

extension PxOsd.MeshTopologyValidation: CustomStringConvertible {
    public var description: String {
        if isValid {
            return "MeshTopologyValidation(valid)"
        } else {
            return "MeshTopologyValidation(invalid, errors: \(errors.count))"
        }
    }
}
