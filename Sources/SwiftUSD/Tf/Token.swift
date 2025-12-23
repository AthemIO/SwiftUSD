// Token.swift - TfToken wrapper
// Mirrors: pxr/base/tf/token.h

import OpenUSDInterop

/// A token is an interned string used for efficient comparison and hashing.
/// Mirrors pxr::TfToken from the USD C++ API.
public struct TfToken: Hashable, Sendable {
    internal var handle: OpaquePointer?

    public init() {
        self.handle = nil
    }

    public init(_ string: String) {
        // TODO: Implement via C interop
        self.handle = nil
    }

    public var text: String {
        // TODO: Implement via C interop
        return ""
    }

    public var isEmpty: Bool {
        return handle == nil
    }
}

extension TfToken: ExpressibleByStringLiteral {
    public init(stringLiteral value: String) {
        self.init(value)
    }
}

extension TfToken: CustomStringConvertible {
    public var description: String {
        return text
    }
}
