/*
  ==============================================================================

    timeWindow.h
    Created: 25 Apr 2020 9:52:50am
    Author:  Spenser Saling

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "LushLookAndFeel.h"

//==============================================================================
/*
*/
class TimeWindow    : public Component
{
public:
    TimeWindow()
    {
        normalisableRange.setSkewForCentre(100.0f);
        setInterceptsMouseClicks(false, true);
    }
    

    ~TimeWindow()
    {
    }

    void paint (Graphics& g) override
    {
        //g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background
        
        float timeStart = normalisableRange.getRange().getStart();
        float timeEnd = normalisableRange.getRange().getEnd();
        float time = timeStart;
        
        //Draw millisecond ticks
        g.setFont (14.0f);
        g.setColour(Colours::white.withAlpha(0.5f));
        while(time <= timeEnd)
        {
            float x = normalisableRange.convertTo0to1(time) * getWidth();
            
            if ( isMultiple(time, 10.0f, 4) )
            {
                drawTick(g, x, 1.0f, 1.0f);
            } else {
                //drawTick(g, x, 0.3f, 1.0f);
            }
            
            incrementTime(time);
        }
        
        //Draw note ticks (1/4 1/2 etc.)
        g.setColour(LushLookAndFeel::colourAccent.withAlpha(0.5f));
        int notes[] = {1, 2, 4, 8, 16, 32, 64};
        for (const int& note : notes){
            float noteDuration = stm::Notes::ms(note, timeSignatureDenominator, bpm);
            if (noteDuration < timeEnd && noteDuration > timeStart){
                float x = normalisableRange.convertTo0to1(noteDuration) * getWidth();
                drawTick(g, x, 1.0f, 1.0f);
            }
            
//            //dotted
//            noteDuration = stm::Notes::msDotted(note, timeSignatureDenominator, bpm);
//            if (noteDuration < timeEnd && noteDuration > timeStart){
//                float x = normalisableRange.convertTo0to1(noteDuration) * getWidth();
//                drawTick(g, x, 0.5f, 0.5f);
//            }
//
//            //triplets
//            noteDuration = stm::Notes::msTriplet(note, timeSignatureDenominator, bpm);
//            if (noteDuration < timeEnd && noteDuration > timeStart){
//                float x = normalisableRange.convertTo0to1(noteDuration) * getWidth();
//                drawTick(g, x, 0.5f, 0.5f);
//            }
        }
        
        drawGradients(g);
        
        //Draw ms tick labels
        time = timeStart;
        g.setColour(Colours::white.withAlpha(0.8f));
        while(time <= timeEnd){
            float x = normalisableRange.convertTo0to1(time) * getWidth();
            if ( isMultiple(time, 10.0f, 4) )
            {
                String text = String((int)time) + "ms";
                labelTick(g, x, text, Justification::centredTop);
            }
            incrementTime(time);
        }
        
        //Draw note tick labels
        g.setColour(LushLookAndFeel::colourAccent.withAlpha(0.9f));
        for (const int& note : notes){
            float noteDuration = stm::Notes::ms(note, timeSignatureDenominator, bpm);
            if (noteDuration < timeEnd && noteDuration > timeStart){
                String text = "1/" + String(note);
                float x = normalisableRange.convertTo0to1(noteDuration) * getWidth();
                labelTick(g, x, text, Justification::centredBottom);
            }
        }
        
        
    }

    void resized() override {}
    
    void setShowNotes(bool val) {showNotes = val;}
    void setTiming(float _bpm, int timeSigDenom) {
        bpm = _bpm;
        timeSignatureDenominator = timeSigDenom;
    }
    void setNormalisableRange(NormalisableRange<float> newRange){
        normalisableRange = newRange;
    }

private:
    bool showNotes = true;
    NormalisableRange<float> normalisableRange = NormalisableRange<float>(5.0f, 1500.0f);
    float bpm = 120.0f;
    int timeSignatureDenominator = 4;
    
    bool isMultiple(float value, float multiple, int maxMultiplications)
    {
        int i=0;
        float compare = multiple;
        
        while (i < maxMultiplications)
        {
            if (isWithin(value, compare, 0.05f))
            {
                return true;
            }
            compare *= multiple;
            i++;
        }
        
        return false;
    }
    
    /**Percent on a range from 0 to 1*/
    bool isWithin(float val1, float val2, float percent){
        float amount;
        float delta;
        
        if (val1 > val2) {
            amount = val2 * percent;
            delta = val1 - val2;
        } else {
            amount = val1 * percent;
            delta = val2 - val1;
        }
        
        return delta < amount ? true : false;
    }
    
    void incrementTime(float& time){
        if (time < 10.0f) {
            time += 1.0f;
        } else if (time < 100.0f) {
            time += 10.0f;
        } else if (time < 1000.0f) {
            time += 100.0f;
        } else {
            time += 1000.0f;
        }
    }
    
    void drawTick(Graphics& g, float x, float thickness, float length){
        float height = getHeight();
        float lineLength = height * length;
        
        g.drawLine(x, 0.0f, x, lineLength, thickness);
        //g.drawLine(x, height - lineLength, x, height, thickness);
    }
    
    void labelTick(Graphics& g, float x, String text, Justification justification){
        float width = 100;
        
        g.drawFittedText(text, x - width/2, 0.0f, width, getHeight(), justification, 2, 1.0f);
    }
    
    void drawGradients(Graphics& g){
        float thickness = 40.0f;
        Colour colour1 = Colours::black;
        Colour colour2 = colour1.withAlpha(0.0f);
        auto topGradient = ColourGradient(colour1, 0.0, 0.0, colour2, 0.0, thickness, false);
        g.setGradientFill(topGradient);
        auto bounds = getLocalBounds();
        g.fillRect(bounds.removeFromTop(thickness));
        
        auto bottomGradient = ColourGradient(colour1, 0.0, getHeight(), colour2, 0.0, getHeight() - thickness, false);
        g.setGradientFill(bottomGradient);
        g.fillRect(bounds.removeFromBottom(thickness));
    }
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TimeWindow)
};
