// Placeholder source file for USDCxx target
// Header-only library - this file ensures the target has at least one source file

#include "../include/base/Arch.h"
#include "../include/base/Gf.h"
#include "../include/base/Vt.h"

// Force instantiation of header functions
namespace {
    [[maybe_unused]] void placeholder() {
        (void)swiftusd::getPageSize();

        // Instantiate Vt array types
        auto intArr = swiftusd::IntArray::Create();
        auto floatArr = swiftusd::FloatArray::Create();
        auto doubleArr = swiftusd::DoubleArray::Create();
        auto vec3fArr = swiftusd::Vec3fArray::Create();
        auto vec3dArr = swiftusd::Vec3dArray::Create();
        auto mat4dArr = swiftusd::Matrix4dArray::Create();

        // Clean up (manually delete since we're using SWIFT_UNSAFE_REFERENCE)
        delete intArr;
        delete floatArr;
        delete doubleArr;
        delete vec3fArr;
        delete vec3dArr;
        delete mat4dArr;
    }
}
