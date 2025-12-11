//
// Copyright 2025 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//

#include "Hgi/swiftBridge.h"

// Include full headers for implementation (NOT exposed to Swift directly)
#include "Hgi/hgi.h"
#include "Hgi/graphicsCmds.h"
#include "Hgi/blitCmds.h"
#include "Hgi/computeCmds.h"

PXR_NAMESPACE_OPEN_SCOPE

// ---------------------------------------------------------------------------
// Hgi Instance Bridge Functions
// ---------------------------------------------------------------------------

void*
Hgi_Swift_CreatePlatformDefaultHgi()
{
    HgiUniquePtr hgi = Hgi::CreatePlatformDefaultHgi();
    // Release ownership from unique_ptr and return raw pointer
    // Caller is responsible for calling Hgi_Swift_DestroyHgi
    return static_cast<void*>(hgi.release());
}

void*
Hgi_Swift_CreateNamedHgi(const TfToken& hgiToken)
{
    HgiUniquePtr hgi = Hgi::CreateNamedHgi(hgiToken);
    // Release ownership from unique_ptr and return raw pointer
    return static_cast<void*>(hgi.release());
}

void
Hgi_Swift_DestroyHgi(void* hgi)
{
    if (hgi) {
        delete static_cast<Hgi*>(hgi);
    }
}

TfToken
Hgi_Swift_GetAPIName(void* hgi)
{
    if (hgi) {
        return static_cast<Hgi*>(hgi)->GetAPIName();
    }
    return TfToken();
}

bool
Hgi_Swift_IsBackendSupported(void* hgi)
{
    if (hgi) {
        return static_cast<Hgi*>(hgi)->IsBackendSupported();
    }
    return false;
}

// ---------------------------------------------------------------------------
// HgiGraphicsCmds Bridge Functions
// ---------------------------------------------------------------------------

void*
Hgi_Swift_CreateGraphicsCmds(void* hgi, const HgiGraphicsCmdsDesc& desc)
{
    if (hgi) {
        HgiGraphicsCmdsUniquePtr cmds =
            static_cast<Hgi*>(hgi)->CreateGraphicsCmds(desc);
        // Release ownership from unique_ptr
        return static_cast<void*>(cmds.release());
    }
    return nullptr;
}

void
Hgi_Swift_DestroyGraphicsCmds(void* cmds)
{
    if (cmds) {
        delete static_cast<HgiGraphicsCmds*>(cmds);
    }
}

// ---------------------------------------------------------------------------
// HgiBlitCmds Bridge Functions
// ---------------------------------------------------------------------------

void*
Hgi_Swift_CreateBlitCmds(void* hgi)
{
    if (hgi) {
        HgiBlitCmdsUniquePtr cmds = static_cast<Hgi*>(hgi)->CreateBlitCmds();
        // Release ownership from unique_ptr
        return static_cast<void*>(cmds.release());
    }
    return nullptr;
}

void
Hgi_Swift_DestroyBlitCmds(void* cmds)
{
    if (cmds) {
        delete static_cast<HgiBlitCmds*>(cmds);
    }
}

// ---------------------------------------------------------------------------
// HgiComputeCmds Bridge Functions
// ---------------------------------------------------------------------------

void*
Hgi_Swift_CreateComputeCmds(void* hgi, const HgiComputeCmdsDesc& desc)
{
    if (hgi) {
        HgiComputeCmdsUniquePtr cmds =
            static_cast<Hgi*>(hgi)->CreateComputeCmds(desc);
        // Release ownership from unique_ptr
        return static_cast<void*>(cmds.release());
    }
    return nullptr;
}

void
Hgi_Swift_DestroyComputeCmds(void* cmds)
{
    if (cmds) {
        delete static_cast<HgiComputeCmds*>(cmds);
    }
}

// ---------------------------------------------------------------------------
// Command Submission Bridge Functions
// ---------------------------------------------------------------------------

void
Hgi_Swift_SubmitCmds(void* hgi, void* cmds, HgiSubmitWaitType wait)
{
    if (hgi && cmds) {
        static_cast<Hgi*>(hgi)->SubmitCmds(
            static_cast<HgiCmds*>(cmds), wait);
    }
}

// ---------------------------------------------------------------------------
// Frame Management Bridge Functions
// ---------------------------------------------------------------------------

void
Hgi_Swift_StartFrame(void* hgi)
{
    if (hgi) {
        static_cast<Hgi*>(hgi)->StartFrame();
    }
}

void
Hgi_Swift_EndFrame(void* hgi)
{
    if (hgi) {
        static_cast<Hgi*>(hgi)->EndFrame();
    }
}

void
Hgi_Swift_GarbageCollect(void* hgi)
{
    if (hgi) {
        static_cast<Hgi*>(hgi)->GarbageCollect();
    }
}

PXR_NAMESPACE_CLOSE_SCOPE
