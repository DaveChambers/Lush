/*
  ==============================================================================

    TimingPanel.h
    Created: 23 Apr 2020 2:13:48pm
    Author:  Spenser Saling

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "TimeWindow.h"
#include "MovingSlider.h"
#include "Params.h"

//==============================================================================
/*
*/
class TimingPanel    : public Component //, public MovingSlider::Listener //, public AudioProcessorValueTreeState::Listener, AsyncUpdater
{
public:
    TimingPanel(AudioProcessorValueTreeState& s) : state(s)
    {
        addAndMakeVisible(timeWindow);
        auto idDelay = Params::getVoiceParamId(1, Params::DELAY);
        auto delayRange = state.getParameterRange(idDelay);
        timeWindow.setNormalisableRange(delayRange);
        
        for (auto voice=1 ; voice <= Params::NUM_VOICES ; voice++){
            MovingSlider* newMovingSlider = new MovingSlider(*this, state, voice);
            addAndMakeVisible(newMovingSlider);
            newMovingSlider->setEnabled(true);
            newMovingSlider->addMouseListener(this, false);
            movingSliders.add(newMovingSlider);
        }
    }

    ~TimingPanel()
    {
        for (MovingSlider* slider : movingSliders){
            slider->removeMouseListener(this);
        }
    }

    void paint (Graphics& g) override
    {
        
    }

    void resized() override
    {
        auto bounds = getLocalBounds();
        
        timeWindow.setBounds(bounds);
        
        bounds.reduce(0, 40);
        for (auto voice=0 ; voice < Params::NUM_VOICES ; voice++){
            movingSliders[voice]->resized();
        }
    }
    
    void mouseMove(const MouseEvent& event) override {
    }
    
    void mouseEnter(const MouseEvent& event) override {
    }
    
    void mouseExit(const MouseEvent& event) override {
    }
    
    void mouseDown(const MouseEvent& event) override {
    }
    
    void mouseDrag(const MouseEvent& event) override {
    }
    
    void mouseUp(const MouseEvent& event) override {
    }

private:
    AudioProcessorValueTreeState& state;
    
    TimeWindow timeWindow;
    OwnedArray<MovingSlider> movingSliders;
    Array<float> lineDelays;
    Array<bool> linesEnabled;
    Array<bool> linesSelected;
    Array<float> lineGains;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TimingPanel)
};
