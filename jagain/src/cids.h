//------------------------------------------------------------------------
// Copyright(c) 2024 Jonathan Abbott
//------------------------------------------------------------------------

#pragma once

#include "pluginterfaces/base/funknown.h"
#include "pluginterfaces/vst/vsttypes.h"

namespace Jtash
{
    static const Steinberg::FUID kJaGainProcessorUID(0x11504032, 0x33895CB7, 0x9D13C67F, 0xDF6E69B3);
    static const Steinberg::FUID kJaGainControllerUID(0x4B6B0244, 0x5B3257F6, 0xA1A170E8, 0x0307B81C);

#define JaGainVST3Category "Fx"
} // namespace Jtash
