/*
  ==============================================================================

    HighLowFilter.h
    Created: 24 Apr 2020 5:24:04pm
    Author:  Spenser Saling

  ==============================================================================
*/

#pragma once

class HighLowFilter : public dsp::ProcessorBase, public AudioProcessorValueTreeState::Listener {
public:
    
    InputFilterVariable(AudioProcessorValueTreeState& params) : parameters(params)
    {
        parameters.addParameterListener(Constants::ID_LOW_PASS_FREQ, this);
        parameters.addParameterListener(Constants::ID_HI_PASS_FREQ, this);
    }
    
    void prepare (const dsp::ProcessSpec& spec) override
    {
        sampleRate = spec.sampleRate;
        
        float lowPassFreq = *parameters.getRawParameterValue(Constants::ID_LOW_PASS_FREQ);
        setLowPassFreq(lowPassFreq);
        
        float hiPassFreq = *parameters.getRawParameterValue(Constants::ID_HI_PASS_FREQ);
        setHiPassFreq(hiPassFreq);
        
        processorChain.prepare(spec);
    }
    
    void process (const dsp::ProcessContextReplacing<float>& context) override
    {
        processorChain.process(context);
    }
    
    void reset () override
    {
        processorChain.reset();
    }
    
    void parameterChanged (const String& parameterID, float newValue) override
    {
        if (parameterID == Constants::ID_LOW_PASS_FREQ)
        {
            setLowPassFreq(newValue);
        }
        else if (parameterID == Constants::ID_HI_PASS_FREQ)
        {
            setHiPassFreq(newValue);
        }
    }
    
private:
    const float Q = 1.0f;
    
    double sampleRate = 44000.0;
    
    enum {
        hiPassIndex,
        lowPassIndex
    };
    
    AudioProcessorValueTreeState& parameters;
    
    using Filter = dsp::StateVariableFilter::Filter<float>;
    using Parameters = dsp::StateVariableFilter::Parameters<float>;
    using DuplicatedFilter = dsp::ProcessorDuplicator<Filter, Parameters>;

    dsp::ProcessorChain<DuplicatedFilter, DuplicatedFilter> processorChain;
    
    void setLowPassFreq(float newFreq){
        auto& lowPassFilter = processorChain.get<lowPassIndex>();
        lowPassFilter.state->setCutOffFrequency(sampleRate, newFreq);
    }
    
    void setHiPassFreq(float newFreq){
        auto& hiPassFilter = processorChain.template get<hiPassIndex>();
        hiPassFilter.state->type = Parameters::Type::highPass;
        hiPassFilter.state->setCutOffFrequency(sampleRate, newFreq);
    }
};
