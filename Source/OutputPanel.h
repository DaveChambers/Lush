/*
  ==============================================================================

    OutputPanel.h
    Created: 23 Apr 2020 2:13:33pm
    Author:  Spenser Saling

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class OutputPanel    : public Component
{
public:
    OutputPanel(AudioProcessorValueTreeState& p) : params(p)
    {
        addAndMakeVisible(dryButton);
        dryButton.setButtonText("Dry");
        dryButton.setClickingTogglesState(true);
        dryButtonAttachment.reset(new ButtonAttachment(params, Params::idDry, dryButton));
        
        addAndMakeVisible(wetSlider);
        wetSliderAttachment.reset(new SliderAttachment(params, Params::idWet, wetSlider));
        wetSlider.setSliderStyle(Slider::LinearVertical);
        wetSlider.setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
        wetSlider.setPopupDisplayEnabled(true, true, this);
        
    }

    ~OutputPanel()
    {
    }

    void paint (Graphics& g) override
    {
        
    }

    void resized() override
    {
        auto bounds = getLocalBounds();
        int padding = 5;
        bounds.reduce(padding, padding);
        int buttonHeight = bounds.getHeight() / 6;
        dryButton.setBounds(bounds.removeFromTop(buttonHeight));
        bounds.removeFromBottom(buttonHeight);
        wetSlider.setBounds(bounds.reduced(0, padding));
    }
    
    

private:
    AudioProcessorValueTreeState& params;
    
    Slider wetSlider;
    typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;
    std::unique_ptr<SliderAttachment> wetSliderAttachment;
    
    TextButton dryButton;
    typedef AudioProcessorValueTreeState::ButtonAttachment ButtonAttachment;
    std::unique_ptr<ButtonAttachment> dryButtonAttachment;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OutputPanel)
};
