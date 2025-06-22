#pragma once

#include "IPlug_include_in_plug_hdr.h"

const int kNumPresets = 1;

enum EParams
{
  kPreGain = 0,
  kPostLoss,
  kNumParams
};

using namespace iplug;
using namespace igraphics;

class IPlugEffect final : public Plugin
{
public:
  IPlugEffect(const InstanceInfo& info);

#if IPLUG_DSP // http://bit.ly/2S64BDd
  void ProcessBlock(sample** inputs, sample** outputs, int nFrames) override;
#endif
};
