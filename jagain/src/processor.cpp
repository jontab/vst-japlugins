//------------------------------------------------------------------------
// Copyright(c) 2024 Jonathan Abbott
//------------------------------------------------------------------------

#include "processor.h"
#include "cids.h"
#include "plugids.h"

#include "base/source/fstreamer.h"
#include "pluginterfaces/vst/ivstparameterchanges.h"
#include "public.sdk/source/vst/vstaudioprocessoralgo.h"

using namespace Steinberg;

namespace Jtash
{
	//------------------------------------------------------------------------
	// JaGainProcessor
	//------------------------------------------------------------------------
	JaGainProcessor::JaGainProcessor()
	{
		setControllerClass(kJaGainControllerUID);
	}

	//------------------------------------------------------------------------
	JaGainProcessor::~JaGainProcessor()
	{
	}

	//------------------------------------------------------------------------
	tresult PLUGIN_API JaGainProcessor::initialize(FUnknown *context)
	{
		tresult result = AudioEffect::initialize(context);
		if (result != kResultOk)
		{
			return result;
		}

		// --- Create Audio IO ------ //
		addAudioInput(STR16("Stereo In"), Steinberg::Vst::SpeakerArr::kStereo);
		addAudioOutput(STR16("Stereo Out"), Steinberg::Vst::SpeakerArr::kStereo);
		return kResultOk;
	}

	//------------------------------------------------------------------------
	tresult PLUGIN_API JaGainProcessor::terminate()
	{
		return AudioEffect::terminate();
	}

	//------------------------------------------------------------------------
	tresult PLUGIN_API JaGainProcessor::setActive(TBool state)
	{
		return AudioEffect::setActive(state);
	}

	//------------------------------------------------------------------------
	tresult PLUGIN_API JaGainProcessor::process(Vst::ProcessData &data)
	{
		// --- Check input parameter changes --- //
		if (data.inputParameterChanges)
		{
			int32 numParamsChanged = data.inputParameterChanges->getParameterCount();
			for (int32 index = 0; index < numParamsChanged; index++)
			{
				if (auto *paramQueue = data.inputParameterChanges->getParameterData(index))
				{
					Vst::ParamValue value;
					int32 sampleOffset;
					int32 numPoints = paramQueue->getPointCount();
					switch (paramQueue->getParameterId())
					{
					case GainParams::kParamGainId:
						if (paramQueue->getPoint(numPoints - 1, sampleOffset, value) == kResultTrue)
							mGain = value;
						break;
					}
				}
			}
		}

		// --- Only parameter changes --- //
		if ((data.numInputs == 0) || (data.numSamples == 0))
			return kResultOk;

		// --- Processing --- //
		uint32 sampleFramesSize = Vst::getSampleFramesSizeInBytes(processSetup, data.numSamples);
		void **in = Vst::getChannelBuffersPointer(processSetup, data.inputs[0]);
		void **out = Vst::getChannelBuffersPointer(processSetup, data.outputs[0]);

		data.outputs[0].silenceFlags = 0;

		for (int32 chanIdx = 0; chanIdx < data.inputs[0].numChannels; chanIdx++)
		{
			Vst::Sample32 *ptrIn = (Vst::Sample32 *)in[chanIdx];
			Vst::Sample32 *ptrOut = (Vst::Sample32 *)out[chanIdx];
			for (int32 sampIdx = 0; sampIdx < data.numSamples; sampIdx++)
				ptrOut[sampIdx] = ptrIn[sampIdx] * mGain;
		}

		// --- Check silence --- //
		if (data.inputs[0].silenceFlags)
			for (int32 chanIdx = 0; chanIdx < data.inputs[0].numChannels; chanIdx++)
				if (in[chanIdx] != out[chanIdx])
					memset(out[chanIdx], 0, sampleFramesSize);

		return kResultOk;
	}

	//------------------------------------------------------------------------
	tresult PLUGIN_API JaGainProcessor::setupProcessing(Vst::ProcessSetup &newSetup)
	{
		return AudioEffect::setupProcessing(newSetup);
	}

	//------------------------------------------------------------------------
	tresult PLUGIN_API JaGainProcessor::canProcessSampleSize(int32 symbolicSampleSize)
	{
		if (symbolicSampleSize == Vst::kSample32)
			return kResultTrue;
		return kResultFalse;
	}

	//------------------------------------------------------------------------
	tresult PLUGIN_API JaGainProcessor::setState(IBStream *state)
	{
		if (!state)
			return kResultFalse;
		IBStreamer streamer(state, kLittleEndian);

		// --- Load mGain --- //
		float savedGain = 0.;
		if (streamer.readFloat(savedGain) == false)
			return kResultFalse;
		mGain = savedGain;

		return kResultOk;
	}

	//------------------------------------------------------------------------
	tresult PLUGIN_API JaGainProcessor::getState(IBStream *state)
	{
		IBStreamer streamer(state, kLittleEndian);
		streamer.writeFloat(mGain);
		return kResultOk;
	}

} // namespace Jtash
