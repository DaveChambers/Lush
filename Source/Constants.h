/*
  ==============================================================================

    Constants.h
    Created: 23 Apr 2020 3:26:06pm
    Author:  Spenser Saling

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

struct Constants
{
public:
    static const int numVoices = 4;
    static const int maxTaps = 10;
    
    constexpr static float minDelay = 10.0; //ms
    constexpr static float maxDelay = 1000.0; //ms
    constexpr static float minSpacing = 0.1; //ms
    constexpr static float maxSpacing = 10.0; //ms
    constexpr static float maxModAmount = 1.0; //ms
    
    inline static String idDry = "dry";
    inline static String idWet = "wet";
    inline static String idBypass = "bypass";
    inline static String idWindow = "window";
    inline static String idSnap = "snap";
    
    inline static String voiceIdPrefix = "voice";
    inline static String voiceNamePrefix = "Voice ";
    
    inline static String idVoiceEnabled = "enabled";
    inline static String idVoiceDelay = "delay";
    inline static String idVoiceTaps = "taps";
    inline static String idVoiceSpacig = "spacing";
    inline static String idVoiceFeedback = "feedback";
    inline static String idVoiceAllPass = "allPass";
    inline static String idVoiceHighPass = "highPass";
    inline static String idVoiceLowPass = "lowPass";
    inline static String idVoiceModAmount = "modAmount";
    inline static String idVoiceModFrequency = "modFrequency";
    inline static String idVoiceIsSelected = "isSelected";
    
};
