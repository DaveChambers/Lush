/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "LushLookAndFeel.h"
#include "OutputPanel.h"
#include "TimingPanel.h"
#include "VoiceEditPanel.h"

//==============================================================================
/**
*/
class LushAudioProcessorEditor  : public AudioProcessorEditor, private Button::Listener
{
public:
    LushAudioProcessorEditor (LushAudioProcessor&);
    ~LushAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    
    void buttonClicked (Button *button) override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    LushAudioProcessor& processor;
    LushLookAndFeel lushLookAndFeel;
    
    const bool debug = true;
    
    std::atomic<float>* bypass;
    
    OutputPanel outputPanel;
    TimingPanel timingPanel;
    VoiceEditPanel voiceEditPanel;
    
    stm::DisabledVeil bypassedVeil;
    stm::DebugDisplay debugDisplay;
    
    TextButton matchedBypassButton;
    typedef AudioProcessorValueTreeState::ButtonAttachment ButtonAttachment;
    std::unique_ptr<ButtonAttachment> matchedBypassButtonAttachment;
    
    void updateWindow();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LushAudioProcessorEditor)
};
