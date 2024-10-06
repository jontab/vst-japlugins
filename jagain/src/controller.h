//------------------------------------------------------------------------
// Copyright(c) 2024 Jonathan Abbott
//------------------------------------------------------------------------

#pragma once

#include "public.sdk/source/vst/vsteditcontroller.h"

namespace Jtash
{

	//------------------------------------------------------------------------
	//  JaGainController
	//------------------------------------------------------------------------
	class JaGainController : public Steinberg::Vst::EditControllerEx1
	{
	public:
		//------------------------------------------------------------------------
		JaGainController() = default;
		~JaGainController() SMTG_OVERRIDE = default;

		// Create function
		static Steinberg::FUnknown *createInstance(void * /*context*/)
		{
			return (Steinberg::Vst::IEditController *)new JaGainController;
		}

		//--- from IPluginBase -----------------------------------------------
		Steinberg::tresult PLUGIN_API initialize(Steinberg::FUnknown *context) SMTG_OVERRIDE;
		Steinberg::tresult PLUGIN_API terminate() SMTG_OVERRIDE;

		//--- from EditController --------------------------------------------
		Steinberg::tresult PLUGIN_API setComponentState(Steinberg::IBStream *state) SMTG_OVERRIDE;
		Steinberg::IPlugView *PLUGIN_API createView(Steinberg::FIDString name) SMTG_OVERRIDE;
		Steinberg::tresult PLUGIN_API setState(Steinberg::IBStream *state) SMTG_OVERRIDE;
		Steinberg::tresult PLUGIN_API getState(Steinberg::IBStream *state) SMTG_OVERRIDE;

		//---Interface---------
		DEFINE_INTERFACES
		// Here you can add more supported VST3 interfaces
		// DEF_INTERFACE (Vst::IXXX)
		END_DEFINE_INTERFACES(EditController)
		DELEGATE_REFCOUNT(EditController)

		//------------------------------------------------------------------------
	protected:
	};

	//------------------------------------------------------------------------
} // namespace Jtash
