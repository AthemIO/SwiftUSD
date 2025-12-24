import XCTest
@testable import SwiftUSD

final class GarchTests: XCTestCase {

    // MARK: - Platform Detection Tests

    func testPlatformType() {
        let platform = Garch.platform
        // Should return a valid platform type
        XCTAssertTrue(
            platform == .none ||
            platform == .openGL ||
            platform == .metal ||
            platform == .vulkan
        )
        print("Detected platform: \(platform)")
    }

    func testPlatformDescription() {
        let platform = Garch.platform
        XCTAssertFalse(platform.description.isEmpty)
    }

    func testHasOpenGL() {
        // Just verify it doesn't crash and returns a boolean
        let hasGL = Garch.hasOpenGL
        print("Has OpenGL: \(hasGL)")
        // OpenGL should be available on macOS
        #if os(macOS)
        XCTAssertTrue(hasGL)
        #endif
    }

    func testHasMetal() {
        let hasMetal = Garch.hasMetal
        print("Has Metal: \(hasMetal)")
        // Metal should be available on macOS 10.11+
        #if os(macOS)
        XCTAssertTrue(hasMetal)
        #endif
    }

    func testHasVulkan() {
        let hasVulkan = Garch.hasVulkan
        print("Has Vulkan: \(hasVulkan)")
        // Vulkan is typically not available on macOS without MoltenVK
        #if os(macOS)
        // Don't assert - Vulkan support is optional
        #endif
    }

    // MARK: - GL Context State Tests

    func testNullContextState() {
        let nullState = Garch.GLContextState.null
        XCTAssertFalse(nullState.isValid)
        XCTAssertEqual(nullState.hashValue, 0)
    }

    func testCurrentContextState() {
        // In standalone mode without a GL context, this should return nil or an invalid state
        let current = Garch.GLContextState.current
        if let state = current {
            // If we got a state, check its properties
            print("Current context valid: \(state.isValid)")
            print("Current context hash: \(state.hashValue)")
        } else {
            // No current context is expected in test environment
            print("No current GL context (expected in test environment)")
        }
    }

    func testContextStateEquality() {
        let null1 = Garch.GLContextState.null
        let null2 = Garch.GLContextState.null
        XCTAssertTrue(null1.equals(null2))
    }

    func testDoneCurrent() {
        // Should not crash
        Garch.GLContextState.doneCurrent()
    }

    // MARK: - GL Debug Context Tests

    func testDebugContextCreation() {
        // In standalone mode, this may return nil
        let context = Garch.GLDebugContext(
            majorVersion: 4,
            minorVersion: 1,
            coreProfile: true,
            directRendering: true
        )

        if let ctx = context {
            print("Created debug context")
            // Try to make it current (may fail in test environment)
            do {
                try ctx.makeCurrent()
            } catch {
                print("Could not make debug context current: \(error)")
            }
        } else {
            print("Could not create debug context (expected in standalone mode)")
        }
    }

    func testDebugOutputEnabled() {
        // Just verify this doesn't crash
        let enabled = Garch.GLDebugContext.isDebugOutputEnabled
        print("Debug output enabled: \(enabled)")
    }

    func testCoreProfileEnabled() {
        let enabled = Garch.GLDebugContext.isCoreProfileEnabled
        print("Core profile enabled: \(enabled)")
        // Should default to true
        XCTAssertTrue(enabled)
    }

    // MARK: - GL API Information Tests

    func testGLAPIInfo() {
        // In standalone mode without a GL context, this returns nil
        if let info = Garch.glAPIInfo {
            print("GL API: \(info)")
            XCTAssertGreaterThanOrEqual(info.majorVersion, 0)
            XCTAssertGreaterThanOrEqual(info.minorVersion, 0)
        } else {
            print("No GL API info available (expected without GL context)")
        }
    }

    func testGLVersionString() {
        if let version = Garch.glVersionString {
            XCTAssertFalse(version.isEmpty)
            print("GL Version: \(version)")
        } else {
            print("No GL version string (expected in standalone mode)")
        }
    }

    func testGLRendererString() {
        if let renderer = Garch.glRendererString {
            XCTAssertFalse(renderer.isEmpty)
            print("GL Renderer: \(renderer)")
        } else {
            print("No GL renderer string (expected in standalone mode)")
        }
    }

    func testGLVendorString() {
        if let vendor = Garch.glVendorString {
            XCTAssertFalse(vendor.isEmpty)
            print("GL Vendor: \(vendor)")
        } else {
            print("No GL vendor string (expected in standalone mode)")
        }
    }

    func testGLSLVersionString() {
        if let version = Garch.glslVersionString {
            XCTAssertFalse(version.isEmpty)
            print("GLSL Version: \(version)")
        } else {
            print("No GLSL version string (expected in standalone mode)")
        }
    }

    // MARK: - GL Extension Tests

    func testExtensionCount() {
        let count = Garch.Extensions.count
        print("GL Extension count: \(count)")
        // In standalone mode, count will be 0
        XCTAssertGreaterThanOrEqual(count, 0)
    }

    func testAllExtensions() {
        let extensions = Garch.Extensions.all
        print("Found \(extensions.count) GL extensions")
        // In standalone mode, this will be empty
        for ext in extensions.prefix(5) {
            print("  - \(ext)")
        }
    }

    func testHasExtension() {
        // Test a common extension - in standalone mode this returns false
        let hasTextureStorage = Garch.Extensions.has("GL_ARB_texture_storage")
        print("Has GL_ARB_texture_storage: \(hasTextureStorage)")
    }

    func testExtensionSubscript() {
        // Same as has, but using subscript
        let hasShaderObjects = Garch.Extensions["GL_ARB_shader_objects"]
        print("Has GL_ARB_shader_objects: \(hasShaderObjects)")
    }

    // MARK: - Error Type Tests

    func testGarchError() {
        let contextError = GarchError.contextError("Test error")
        XCTAssertTrue(contextError.description.contains("Test error"))

        let platformError = GarchError.platformNotSupported("TestPlatform")
        XCTAssertTrue(platformError.description.contains("TestPlatform"))
    }

    // MARK: - Integration Tests

    func testPlatformCapabilitiesSummary() {
        print("\n=== Garch Platform Capabilities ===")
        print("Primary platform: \(Garch.platform)")
        print("OpenGL available: \(Garch.hasOpenGL)")
        print("Metal available: \(Garch.hasMetal)")
        print("Vulkan available: \(Garch.hasVulkan)")
        print("Core profile enabled: \(Garch.GLDebugContext.isCoreProfileEnabled)")
        print("Debug output enabled: \(Garch.GLDebugContext.isDebugOutputEnabled)")
        print("===================================\n")
    }
}
