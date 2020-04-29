/*
  ==============================================================================

    LushEngine.h
    Created: 23 Apr 2020 1:51:45pm
    Author:  Spenser Saling

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Params.h"
#include "LushDelayLine.h"

class LushEngine: public AudioProcessorValueTreeState::Listener {
public:
    LushEngine(AudioProcessorValueTreeState& s) : state(s)
    {
        state.addParameterListener(Params::idDry, this);
        state.addParameterListener(Params::idWet, this);
        state.addParameterListener(Params::idBypass, this);
        
        for (auto voice=1 ; voice <= Params::NUM_VOICES ; voice++)
        {
            delayLines.add(new LushDelayLine(state, voice));
        }
    }
    
    void prepare(const dsp::ProcessSpec& spec) {
        dryWetMix.prepare(spec);
        matchedBypass.prepare(spec);
        dryBuffer.setSize(spec.numChannels, spec.maximumBlockSize);
        
        for (auto voice=1 ; voice <= Params::NUM_VOICES ; voice++)
        {
            delayLines[voice-1]->prepare(spec);
        }
        
        updateParameters();
    }
    
    void process(AudioBuffer<float>& buffer) {
        // Create a copy of the input block to use as the dry signal.
        auto outBlock = juce::dsp::AudioBlock<float> (buffer);
        
        //Lush is designed as a stereo delay and only processes 2 buffers.
        jassert(outBlock.getNumChannels() == 2);
        
        auto numSamples = outBlock.getNumSamples();
        auto dryBlock = dsp::AudioBlock<float>(dryBuffer).getSubBlock(0, numSamples);
        dryBlock.copyFrom(outBlock);
        outBlock.clear();
        
        for (auto voice=1 ; voice <= Params::NUM_VOICES ; voice++)
        {
            delayLines[voice-1]->process(dryBlock, outBlock);
        }
        
        dryWetMix.process(dryBlock, outBlock);
        matchedBypass.process(dryBlock, outBlock);
    }
    
    void reset() {
        for (auto voice=1 ; voice <= Params::NUM_VOICES ; voice++)
        {
            delayLines[voice-1]->reset();
        }
        dryWetMix.reset();
        matchedBypass.reset();
    }
    
    void parameterChanged(const String& parameterID, float newValue ) {
        
        if (parameterID == Params::idDry) {
            dryWetMix.setDryLinear(newValue);
        } else if (parameterID == Params::idWet) {
            dryWetMix.setWetDecibels(newValue);
        } else if (parameterID == Params::idBypass) {
            matchedBypass.setActive(newValue > 0.5 ? true : false);
        }
    }
    
private:
    AudioProcessorValueTreeState& state;
    stm::DryWetMix dryWetMix;
    stm::MatchedBypass matchedBypass;
    
    AudioSampleBuffer dryBuffer;
    
    OwnedArray<LushDelayLine> delayLines;
    
    void updateParameters(){
        dryWetMix.setDryDecibels(*state.getRawParameterValue(Params::idDry));
        dryWetMix.setWetDecibels(*state.getRawParameterValue(Params::idWet));
        matchedBypass.setActive(*state.getRawParameterValue(Params::idBypass));
    }
};
