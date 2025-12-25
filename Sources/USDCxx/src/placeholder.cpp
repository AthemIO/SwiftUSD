// Placeholder source file for USDCxx target
// Header-only library - this file ensures the target has at least one source file

#include "../include/base/Arch.h"

// Force instantiation of header functions
namespace {
    [[maybe_unused]] void placeholder() {
        (void)swiftusd::getPageSize();
    }
}
