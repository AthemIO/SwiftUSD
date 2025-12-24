// Specifier.swift - UsdSpecifier and related enums
// Mirrors: pxr/usd/sdf/types.h

import OpenUSDInterop

// MARK: - Specifier

/// Specifies the type of definition for a prim.
public enum Specifier: Sendable {
    /// A concrete prim definition.
    case def
    /// An override for an existing prim.
    case over
    /// An abstract class definition.
    case `class`

    internal init(cValue: UsdSpecifier) {
        switch cValue {
        case USD_SPECIFIER_DEF:
            self = .def
        case USD_SPECIFIER_OVER:
            self = .over
        case USD_SPECIFIER_CLASS:
            self = .class
        default:
            self = .def
        }
    }

    internal var cValue: UsdSpecifier {
        switch self {
        case .def: return USD_SPECIFIER_DEF
        case .over: return USD_SPECIFIER_OVER
        case .class: return USD_SPECIFIER_CLASS
        }
    }
}

// MARK: - CustomStringConvertible

extension Specifier: CustomStringConvertible {

    public var description: String {
        switch self {
        case .def: return "def"
        case .over: return "over"
        case .class: return "class"
        }
    }
}

// MARK: - Variability

/// Specifies whether an attribute can vary over time.
public enum Variability: Sendable {
    /// The attribute can have time-sampled values.
    case varying
    /// The attribute has a single uniform value.
    case uniform

    internal init(cValue: UsdVariability) {
        switch cValue {
        case USD_VARIABILITY_VARYING:
            self = .varying
        case USD_VARIABILITY_UNIFORM:
            self = .uniform
        default:
            self = .varying
        }
    }

    internal var cValue: UsdVariability {
        switch self {
        case .varying: return USD_VARIABILITY_VARYING
        case .uniform: return USD_VARIABILITY_UNIFORM
        }
    }
}

// MARK: - CustomStringConvertible

extension Variability: CustomStringConvertible {

    public var description: String {
        switch self {
        case .varying: return "varying"
        case .uniform: return "uniform"
        }
    }
}

// MARK: - ListPosition

/// Specifies where to add items in list-editing operations.
public enum ListPosition: Sendable {
    /// At the front of the prepend list.
    case frontOfPrependList
    /// At the back of the prepend list.
    case backOfPrependList
    /// At the front of the append list.
    case frontOfAppendList
    /// At the back of the append list.
    case backOfAppendList

    internal init(cValue: UsdListPosition) {
        switch cValue {
        case USD_LIST_POSITION_FRONT_OF_PREPEND_LIST:
            self = .frontOfPrependList
        case USD_LIST_POSITION_BACK_OF_PREPEND_LIST:
            self = .backOfPrependList
        case USD_LIST_POSITION_FRONT_OF_APPEND_LIST:
            self = .frontOfAppendList
        case USD_LIST_POSITION_BACK_OF_APPEND_LIST:
            self = .backOfAppendList
        default:
            self = .backOfPrependList
        }
    }

    internal var cValue: UsdListPosition {
        switch self {
        case .frontOfPrependList: return USD_LIST_POSITION_FRONT_OF_PREPEND_LIST
        case .backOfPrependList: return USD_LIST_POSITION_BACK_OF_PREPEND_LIST
        case .frontOfAppendList: return USD_LIST_POSITION_FRONT_OF_APPEND_LIST
        case .backOfAppendList: return USD_LIST_POSITION_BACK_OF_APPEND_LIST
        }
    }
}

// MARK: - CustomStringConvertible

extension ListPosition: CustomStringConvertible {

    public var description: String {
        switch self {
        case .frontOfPrependList: return "frontOfPrependList"
        case .backOfPrependList: return "backOfPrependList"
        case .frontOfAppendList: return "frontOfAppendList"
        case .backOfAppendList: return "backOfAppendList"
        }
    }
}
