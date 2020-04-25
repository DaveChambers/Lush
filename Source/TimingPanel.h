/*
  ==============================================================================

    TimingPanel.h
    Created: 23 Apr 2020 2:13:48pm
    Author:  Spenser Saling

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class TimingPanel    : public Component
{
public:
    TimingPanel(AudioProcessorValueTreeState& p) : params(p)
    {
        // In your constructor, you should add any child components, and
        // initialise any special settings that your component needs.
        
    }

    ~TimingPanel()
    {
    }

    void paint (Graphics& g) override
    {
        /* This demo code just fills the component's background and
           draws some placeholder text to get you started.

           You should replace everything in this method with your own
           drawing code..
        */

        g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background

        g.setColour (Colours::grey);
        g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

        g.setColour (Colours::white);
        g.setFont (14.0f);
        g.drawText ("TimingPanel", getLocalBounds(),
                    Justification::centred, true);   // draw some placeholder text
    }

    void resized() override
    {
        // This method is where you should set the bounds of any child
        // components that your component contains..

    }

private:
    AudioProcessorValueTreeState& params;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TimingPanel)
};
