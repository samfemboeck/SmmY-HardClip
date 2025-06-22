#include "IPlugEffect.h"
#include "IPlug_include_in_plug_src.h"
#include "IControls.h"
#include <iostream>

IPlugEffect::IPlugEffect(const InstanceInfo& info)
: iplug::Plugin(info, MakeConfig(kNumParams, kNumPresets))
{
  GetParam(kPreGain)->InitDouble("Pre Gain", 0., 0, 48., 0.1, "dB");
  GetParam(kPostLoss)->InitDouble("Post Loss", 0., 0, 48., 0.1, "dB");

#if IPLUG_EDITOR // http://bit.ly/2S64BDd
  mMakeGraphicsFunc = [&]() {
    return MakeGraphics(*this, PLUG_WIDTH, PLUG_HEIGHT, PLUG_FPS, GetScaleForScreen(PLUG_WIDTH, PLUG_HEIGHT));
  };

  
  mLayoutFunc = [&](IGraphics* pGraphics) {
    const IRECT b = pGraphics->GetBounds();

    pGraphics->AttachCornerResizer(EUIResizerMode::Scale, false);
    pGraphics->AttachPanelBackground(COLOR_GRAY);
    pGraphics->LoadFont("Roboto-Regular", ROBOTO_FN);

    int knobSize = 80;
    int padding = 10; // Abstand zwischen den Knobs und zum Rand
    float startX = (b.W() - (2 * knobSize + padding)) / 2.0;
    float startY = 10; // Position von oben
    IRECT knob1Bounds(startX, startY, startX + knobSize, startY + knobSize);
    // Berechnung der Position für den zweiten Knob
    IRECT knob2Bounds(startX + knobSize + padding, startY, startX + knobSize + padding + knobSize, startY + knobSize);
    GetUI()->AttachControl(new IVKnobControl(knob1Bounds, kPreGain));
    GetUI()->AttachControl(new IVKnobControl(knob2Bounds, kPostLoss));
  };
#endif
}

#if IPLUG_DSP
void IPlugEffect::ProcessBlock(sample** inputs, sample** outputs, int nFrames)
{
  const double preGain = std::pow(10.0, GetParam(kPreGain)->Value() / 20.0);
  const double postGain = std::pow(10.0, -1 * GetParam(kPostLoss)->Value() / 20.0);
  const int nChans = NOutChansConnected();

  
  for (int s = 0; s < nFrames; s++)
  {
    for (int c = 0; c < nChans; c++)
    {
      double input = inputs[c][s] * preGain;

      if (input > 1)
        input = 1;
      else if (input < -1)
        input = -1;

      outputs[c][s] = static_cast<sample>(input * postGain);
    }
  }
}
#endif
