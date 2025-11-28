//
// Copyright 2017 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
#ifndef PXR_IMAGING_PLUGIN_HD_STORM_RENDERER_PLUGIN_H
#define PXR_IMAGING_PLUGIN_HD_STORM_RENDERER_PLUGIN_H

#include "pxr/pxrns.h"
#include "Hd/rendererPlugin.h"

PXR_NAMESPACE_OPEN_SCOPE

class HdStormRendererPlugin final : public HdRendererPlugin
{
 public:
  HdStormRendererPlugin() = default;
  virtual ~HdStormRendererPlugin() = default;

  virtual HdRenderDelegate *CreateRenderDelegate() override;
  virtual HdRenderDelegate *CreateRenderDelegate(HdRenderSettingsMap const &settingsMap) override;

  virtual void DeleteRenderDelegate(HdRenderDelegate *renderDelegate) override;

  // Hydra 2.0 API (required - pure virtual in base)
  virtual bool IsSupported(
      HdRendererCreateArgs const &rendererCreateArgs,
      std::string *reasonWhyNot = nullptr) const override;

  // Deprecated Hydra 1.0 API
  virtual bool IsSupported(bool gpuEnabled = true) const override;

 private:
  HdStormRendererPlugin(const HdStormRendererPlugin &) = delete;
  HdStormRendererPlugin &operator=(const HdStormRendererPlugin &) = delete;
};

PXR_NAMESPACE_CLOSE_SCOPE

#endif  // PXR_IMAGING_PLUGIN_HD_STORM_RENDERER_PLUGIN_H
