//------------------------------------------------------------------------
// Copyright(c) 2024 Jonathan Abbott
//------------------------------------------------------------------------

#include "controller.h"
#include "cids.h"
#include "plugids.h"
#include "vstgui/plugin-bindings/vst3editor.h"
#include "base/source/fstreamer.h"

using namespace Steinberg;

namespace Jtash
{

	//------------------------------------------------------------------------
	// JaGainController Implementation
	//------------------------------------------------------------------------
	tresult PLUGIN_API JaGainController::initialize(FUnknown *context)
	{
		tresult result = EditControllerEx1::initialize(context);
		if (result != kResultOk)
		{
			return result;
		}

		parameters.addParameter(STR16("Gain"), STR16("dB"), 0, .5, Vst::ParameterInfo::kCanAutomate, GainParams::kParamGainId, 0);
		return result;
	}

	//------------------------------------------------------------------------
	tresult PLUGIN_API JaGainController::terminate()
	{
		return EditControllerEx1::terminate();
	}

	//------------------------------------------------------------------------
	tresult PLUGIN_API JaGainController::setComponentState(IBStream *state)
	{
		if (!state)
			return kResultFalse;
		IBStreamer streamer(state, kLittleEndian);

		// --- Read Gain parameter --- //
		float savedGain = 0.;
		if (streamer.readFloat(savedGain) == false)
			return kResultFalse;

		if (auto param = parameters.getParameter(GainParams::kParamGainId))
			param->setNormalized(savedGain);

		return kResultOk;
	}

	//------------------------------------------------------------------------
	tresult PLUGIN_API JaGainController::setState(IBStream *state)
	{
		return kResultTrue;
	}

	//------------------------------------------------------------------------
	tresult PLUGIN_API JaGainController::getState(IBStream *state)
	{
		return kResultTrue;
	}

	//------------------------------------------------------------------------
	IPlugView *PLUGIN_API JaGainController::createView(FIDString name)
	{
		if (FIDStringsEqual(name, Vst::ViewType::kEditor))
		{
			auto *view = new VSTGUI::VST3Editor(this, "view", "editor.uidesc");
			return view;
		}
		return nullptr;
	}

} // namespace Jtash
