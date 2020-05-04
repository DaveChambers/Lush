/*
  ==============================================================================

    Parameters.h
    Created: 23 Apr 2020 5:40:02pm
    Author:  Spenser Saling

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class Params
{
public:
    static const int NUM_VOICES = 4;
    constexpr static const float MAX_DELAY_MS = 1000.0f;
    constexpr static const float MAX_MOD_MS = 1.0f;
    constexpr static const float DELAY_CENTRE = 100.0f;
    
    
    inline static const String idDry = "dry";
    inline static const String idWet = "wet";
    inline static const String idBypass = "bypass";
    inline static const String idWindow = "window";
    inline static const String idSnap = "snap";
    
    //These must line up with the order in voiceParamIds and voiceParamNames below.
    //Their integer representation will be used to index for the given String
    enum VoiceParam {
        ENABLED = 0,
        DELAY, // = 1
        GAIN, // = 2
    };
    
    inline static String voiceParamIds[] = {"enabled", "delay", "gain", "selected"};
    inline static String voiceParamNames[] = {"Enabled", "Delay", "Gain", "Selected"};
    
    static String getVoiceParamId(int voiceNumber, VoiceParam param){
        return getVoiceParamIdPrefix(voiceNumber) + voiceParamIds[param];
    }

    static AudioProcessorValueTreeState::ParameterLayout createParameterLayout()
    {
        std::vector<std::unique_ptr<RangedAudioParameter>> params;
        
        auto delayRange = NormalisableRange<float>(MAX_MOD_MS, MAX_DELAY_MS);
        delayRange.setSkewForCentre(DELAY_CENTRE);
        
        auto gainRange = NormalisableRange<float>(-100.0f, 0.0f);
        gainRange.setSkewForCentre(-18.0f);
        
        params.push_back( std::make_unique<AudioParameterBool>("dry", "Dry Enabled", true) );
        params.push_back( std::make_unique<AudioParameterFloat>("wet", "Wet dB", gainRange, 0.0f) );
        params.push_back( std::make_unique<AudioParameterBool>("bypass", "Matched Bypass", false) );
        params.push_back( std::make_unique<AudioParameterBool>("snap", "Snap", false) );
        
        for (int voice = 1; voice <= NUM_VOICES; voice++){
            String paramId = getVoiceParamId(voice, ENABLED);
            String paramName = getVoiceParamName(voice, ENABLED);
            params.push_back( std::make_unique<AudioParameterBool>(paramId, paramName, true) );
            
            paramId = getVoiceParamId(voice, DELAY);
            paramName = getVoiceParamName(voice, DELAY);
            params.push_back( std::make_unique<AudioParameterFloat>(paramId, paramName, delayRange, DELAY_CENTRE) );
            
            paramId = getVoiceParamId(voice, GAIN);
            paramName = getVoiceParamName(voice, GAIN);
            params.push_back( std::make_unique<AudioParameterFloat>(paramId, paramName, gainRange, 0.0f) );
        }
        
        return { params.begin(), params.end() };
    }
        
private:
    static String getVoiceParamName(int voiceNumber, VoiceParam param){
        return "Voice " + String(voiceNumber) + voiceParamNames[param];
    }
    
    static String getVoiceParamIdPrefix(int voiceNumber){
        return "voice" + String(voiceNumber);
    }
};

