/*
  ==============================================================================

    LushDelayLine.h
    Created: 28 Apr 2020 8:32:00am
    Author:  Spenser Saling

  ==============================================================================
*/

#pragma once
#include "Params.h"

class LushDelayLine : public AudioProcessorValueTreeState::Listener
{
public:
    LushDelayLine(AudioProcessorValueTreeState& s, int voice) : state(s) {
    
        idEnabled = Params::getVoiceParamId(voice, Params::ENABLED);
        idDelay = Params::getVoiceParamId(voice, Params::DELAY);
        idGain = Params::getVoiceParamId(voice, Params::GAIN);
        
        isEnabled = state.getRawParameterValue(idEnabled);
        delay = state.getRawParameterValue(idDelay);
        gain = state.getRawParameterValue(idGain);
        
        gainRamper.prepare(Decibels::decibelsToGain(gain->load()), 0.001);
        
        state.addParameterListener(idGain, this);
    }
    
    void prepare(const dsp::ProcessSpec& spec) {
        samplesPerMS = spec.sampleRate / 1000.0f;
        
        int delaySamples = int (Params::MAX_DELAY_MS * samplesPerMS) + 1;
        int modSamples = int (Params::MAX_MOD_MS * samplesPerMS) + 1;
        
        bufferL.prepare(delaySamples + modSamples);
        bufferR.prepare(delaySamples + modSamples);
    }
    
    void process(const dsp::AudioBlock<float>& dryBlock, const dsp::AudioBlock<float>& outBlock) {
        //LushDelayLine is only designed to handle stereo input.
        jassert(outBlock.getNumChannels() == 2);
        
        if (*isEnabled > 0.5f){
            for (auto sampleIndex = 0 ; sampleIndex < dryBlock.getNumSamples() ; sampleIndex++)
            {
                bufferL.push(dryBlock.getSample(0, sampleIndex));
                bufferR.push(dryBlock.getSample(1, sampleIndex));
                
                float gain = gainRamper.getNext();
                float delaySampleL = bufferL.getSample(int(*delay * samplesPerMS));
                float delaySampleR = bufferR.getSample(int(*delay * samplesPerMS));
                
                outBlock.addSample(0, sampleIndex, delaySampleL * gain);
                outBlock.addSample(1, sampleIndex, delaySampleR * gain);
                
                bufferL.increment();
                bufferR.increment();
            }
        }
    }
        
    void reset() {
        
    }
        
    void parameterChanged(const String& parameterID, float newValue ) override
    {
        stm::DebugDisplay::set(1, parameterID + ": " + String(newValue));
        
        if (parameterID == idEnabled) {
            
        } else if (parameterID == idDelay) {
            
        } else if (parameterID == idGain) {
            gainRamper.updateTarget(Decibels::decibelsToGain(newValue));
        }
    }
    
private:
    AudioProcessorValueTreeState& state;
    stm::RecircBuffer bufferL, bufferR;
    String idEnabled, idDelay, idGain;
    double samplesPerMS = 0.0;
    
    //Raw parameter values for params that aren't updated via listener
    std::atomic<float> *delay;
    std::atomic<float> *isEnabled;
    std::atomic<float> *gain;
    
    stm::RamperLinear gainRamper;
};
