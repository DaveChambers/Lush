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

class LushEngine: public dsp::ProcessorBase, public AudioProcessorValueTreeState::Listener {
public:
    LushEngine(AudioProcessorValueTreeState& p) : parameters(p)
    {
        parameters.addParameterListener(Params::idDry, this);
        parameters.addParameterListener(Params::idWet, this);
        parameters.addParameterListener(Params::idBypass, this);
    }
    
    void prepare(const dsp::ProcessSpec& spec) override {
        dryWetMix.prepare(spec);
        matchedBypass.prepare(spec);
        dryBuffer.setSize(spec.numChannels, spec.maximumBlockSize);
        
        updateParameters();
    }
    
    void process(const dsp::ProcessContextReplacing<float>& context) override {
        // Create a copy of the input block to use as the dry signal.
        auto inBlock = context.getInputBlock();
        size_t numSamples = inBlock.getNumSamples();
        auto dryBlock = dsp::AudioBlock<float>(dryBuffer).getSubBlock(0, numSamples);
        dryBlock.copyFrom(inBlock);
        
        dryWetMix.process(context, dryBlock);
        matchedBypass.process(context, dryBlock);
    }
    
    void reset() override {
        dryWetMix.reset();
        matchedBypass.reset();
    }
    
    void parameterChanged(const String& parameterID, float newValue ) override {
        if (parameterID == Params::idDry) {
            dryWetMix.setDryDecibels(newValue);
        } else if (parameterID == Params::idWet) {
            dryWetMix.setWetDecibels(newValue);
        } else if (parameterID == Params::idBypass) {
            matchedBypass.setActive(newValue > 0.5 ? true : false);
        }
    }
    
private:
    AudioProcessorValueTreeState& parameters;
    stm::DryWetMix dryWetMix;
    stm::MatchedBypass matchedBypass;
    
    AudioSampleBuffer dryBuffer;
    
    void updateParameters(){
        dryWetMix.setDryDecibels(*parameters.getRawParameterValue(Params::idDry));
        dryWetMix.setWetDecibels(*parameters.getRawParameterValue(Params::idWet));
        matchedBypass.setActive(*parameters.getRawParameterValue(Params::idBypass));
    }
};
