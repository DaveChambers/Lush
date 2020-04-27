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
    TimingPanel(AudioProcessorValueTreeState& p) : params(p)
    {
        addAndMakeVisible(timeWindow);
        
        for (auto voice=1 ; voice <= Params::NUM_VOICES ; voice++){
            MovingSlider* newMovingSlider = new MovingSlider(*this);
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
        
        bounds.reduce(10, 0);
        timeWindow.setBounds(bounds);
        
        bounds.reduce(0, 30);
        for (auto voice=0 ; voice < Params::NUM_VOICES ; voice++){
            movingSliders[voice]->setBounds(bounds.removeFromLeft(28));
            bounds.removeFromLeft(20);
        }
        
    }
    
    void mouseMove(const MouseEvent& event) override {
        stm::DebugDisplay::set(7, "Timing Panel X: " + String(event.x));
        stm::DebugDisplay::set(8, "Timing Panel Y: " + String(event.y));
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
    AudioProcessorValueTreeState& params;
    
    TimeWindow timeWindow;
    OwnedArray<MovingSlider> movingSliders;
    Array<float> lineDelays;
    Array<bool> linesEnabled;
    Array<bool> linesSelected;
    Array<float> lineGains;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TimingPanel)
};





//struct AttachedControlBase  : public AudioProcessorValueTreeState::Listener,
//                              public AsyncUpdater
//{
//    AttachedControlBase (AudioProcessorValueTreeState& s, const String& p)
//        : state (s), paramID (p), lastValue (0)
//    {
//        state.addParameterListener (paramID, this);
//    }
//
//    void removeListener()
//    {
//        state.removeParameterListener (paramID, this);
//    }
//
//    void setNewDenormalisedValue (float newDenormalisedValue)
//    {
//        if (auto* p = state.getParameter (paramID))
//        {
//            const float newValue = state.getParameterRange (paramID)
//                                        .convertTo0to1 (newDenormalisedValue);
//
//            if (p->getValue() != newValue)
//                p->setValueNotifyingHost (newValue);
//        }
//    }
//
//    void sendInitialUpdate()
//    {
//        if (auto* v = state.getRawParameterValue (paramID))
//            parameterChanged (paramID, *v);
//    }
//
//    void parameterChanged (const String&, float newValue) override
//    {
//        lastValue = newValue;
//
//        if (MessageManager::getInstance()->isThisTheMessageThread())
//        {
//            cancelPendingUpdate();
//            setValue (newValue);
//        }
//        else
//        {
//            triggerAsyncUpdate();
//        }
//    }
//
//    void beginParameterChange()
//    {
//
//        if (auto* p = state.getParameter (paramID))
//        {
//            if (state.undoManager != nullptr)
//                state.undoManager->beginNewTransaction();
//
//            p->beginChangeGesture();
//        }
//    }
//
//    void endParameterChange()
//    {
//        if (AudioProcessorParameter* p = state.getParameter (paramID))
//            p->endChangeGesture();
//    }
//
//    void handleAsyncUpdate() override
//    {
//        setValue (lastValue);
//    }
//
//    virtual void setValue (float) = 0;
//
//    AudioProcessorValueTreeState& state;
//    String paramID;
//    float lastValue;
//
//    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AttachedControlBase)
//};
//
//struct AudioProcessorValueTreeState::ButtonAttachment::Pimpl  : private AttachedControlBase,
//                                                                private Button::Listener
//{
//    Pimpl (AudioProcessorValueTreeState& s, const String& p, Button& b)
//        : AttachedControlBase (s, p), button (b), ignoreCallbacks (false)
//    {
//        sendInitialUpdate();
//        button.addListener (this);
//    }
//
//    ~Pimpl() override
//    {
//        button.removeListener (this);
//        removeListener();
//    }
//
//    void setValue (float newValue) override
//    {
//        const ScopedLock selfCallbackLock (selfCallbackMutex);
//
//        {
//            ScopedValueSetter<bool> svs (ignoreCallbacks, true);
//            button.setToggleState (newValue >= 0.5f, sendNotificationSync);
//        }
//    }
//
//    void buttonClicked (Button* b) override
//    {
//        const ScopedLock selfCallbackLock (selfCallbackMutex);
//
//        if (! ignoreCallbacks)
//        {
//            beginParameterChange();
//            setNewDenormalisedValue (b->getToggleState() ? 1.0f : 0.0f);
//            endParameterChange();
//        }
//    }
//
//    Button& button;
//    bool ignoreCallbacks;
//    CriticalSection selfCallbackMutex;
//
//    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Pimpl)
//};
