/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Params.h"

//==============================================================================
LushAudioProcessorEditor::LushAudioProcessorEditor (LushAudioProcessor& p)
    : AudioProcessorEditor (&p),
    processor (p),
    outputPanel(p.parameters),
    timingPanel(p.parameters),
    voiceEditPanel(p.parameters)
{
    setSize (800, 500);
    setResizeLimits(600, 400, 1920, 1080);
    setLookAndFeel(&lushLookAndFeel);
    
    addAndMakeVisible(outputPanel);
    addAndMakeVisible(timingPanel);
    addAndMakeVisible(voiceEditPanel);
    addAndMakeVisible(bypassedVeil);
    addAndMakeVisible(debugDisplay);
    
    addAndMakeVisible(matchedBypassButton);
    matchedBypassButton.setButtonText("MATCHED BYPASS");
    matchedBypassButton.setClickingTogglesState(true);
    matchedBypassButtonAttachment.reset(new ButtonAttachment(p.parameters, Params::idBypass, matchedBypassButton));
    matchedBypassButton.addListener(this);
    
    bypass = p.parameters.getRawParameterValue(Params::idBypass);
    
    updateWindow();
}

LushAudioProcessorEditor::~LushAudioProcessorEditor()
{
    matchedBypassButton.removeListener(this);
    setLookAndFeel(nullptr);
}

//==============================================================================
void LushAudioProcessorEditor::paint (Graphics& g)
{
    
}

void LushAudioProcessorEditor::resized()
{
    auto bounds = getLocalBounds();
    
    int debugWidth = bounds.getWidth() / 5;
    if (debug){
        debugDisplay.setBounds(bounds.removeFromLeft(debugWidth));
    }
    
    bypassedVeil.setBounds(bounds);
    int outputPanelWidth = 100;
    auto outputPanelBounds = bounds.removeFromRight(outputPanelWidth);
    outputPanel.setBounds(outputPanelBounds);
    outputPanelBounds.reduce(5, 5);
    int buttonHeight = 50;
    matchedBypassButton.setBounds(outputPanelBounds.removeFromBottom(buttonHeight));
    int timingPanelHeight = bounds.getHeight() / 2;
    timingPanel.setBounds(bounds.removeFromTop(timingPanelHeight));
    voiceEditPanel.setBounds(bounds);
}

void LushAudioProcessorEditor::buttonClicked (Button *button){
    if (button == &matchedBypassButton){
        updateWindow();
    }
}

void LushAudioProcessorEditor::updateWindow(){
    bool isBypassed = matchedBypassButton.getToggleState();
    //bool isBypassed = bypass->load() > 0.5f ? true : false;
    
    if (isBypassed) {
        bypassedVeil.setVisible(true);
    } else {
        bypassedVeil.setVisible(false);
    }
}
