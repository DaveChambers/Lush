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
        SELECTED, // = ...
    };
    
    inline static String voiceParamIds[] = {"enabled", "delay", "gain", "selected"};
    inline static String voiceParamNames[] = {"Enabled", "Delay", "Gain", "Selected"};
    
    static String getVoiceParamId(int voiceNumber, VoiceParam param){
        return getVoiceParamIdPrefix(voiceNumber) + voiceParamIds[param];
    }

    static AudioProcessorValueTreeState::ParameterLayout createParameterLayout()
    {
        std::vector<std::unique_ptr<RangedAudioParameter>> params;
        
        params.push_back( std::make_unique<AudioParameterBool>("dry", "Dry Enabled", true) );
        params.push_back( std::make_unique<AudioParameterFloat>("wet", "Wet dB", -100.0f, 12.0f, 0.0f) );
        params.push_back( std::make_unique<AudioParameterBool>("bypass", "Matched Bypass", false) );
        params.push_back( std::make_unique<AudioParameterFloat>("window", "Window", 10.0, 1000.0, 100.0) );
        params.push_back( std::make_unique<AudioParameterBool>("snap", "Snap", false) );
        
        for (int voice = 1; voice <= NUM_VOICES; voice++){
            String paramId = getVoiceParamId(voice, ENABLED);
            String paramName = getVoiceParamName(voice, ENABLED);
            params.push_back( std::make_unique<AudioParameterBool>(paramId, paramName, false) );
            
            paramId = getVoiceParamId(voice, DELAY);
            paramName = getVoiceParamName(voice, DELAY);
            params.push_back( std::make_unique<AudioParameterFloat>(paramId, paramName, 10.0f, 1000.0f, 100.0f) );
            
            paramId = getVoiceParamId(voice, GAIN);
            paramName = getVoiceParamName(voice, GAIN);
            params.push_back( std::make_unique<AudioParameterFloat>(paramId, paramName, -100.0f, 12.0f, 0.0f) );
            
            paramId = getVoiceParamId(voice, SELECTED);
            paramName = getVoiceParamName(voice, SELECTED);
            params.push_back( std::make_unique<AudioParameterBool>(paramId, paramName, false) );
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

