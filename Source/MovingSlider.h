/*
  ==============================================================================

    LineComponent.h
    Created: 25 Apr 2020 5:44:08pm
    Author:  Spenser Saling

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "LushLookAndFeel.h"

//==============================================================================
/*
*/
class MovingSlider : public Component
{
public:
    struct Listener {
        virtual ~Listener(){}
        virtual void dragged(float xDrag, float yDrag) = 0;
        virtual void valueChanged(float newValue) = 0;
    };
    
    MovingSlider(Component& p) : parent(p){
        gainLabel.setJustificationType(Justification::centred);
        delayLabel.setJustificationType(Justification::centred);
    }

    ~MovingSlider(){}

    void paint (Graphics& g) override
    {
        updateLabels();
        
        if (isEnabled) {
            calculate();
            drawTrack(g);
            if (isSelected) {
                drawOutline(g);
            } else if (isHovered) {
                drawOutline(g, hoveredAlpha);
            }
            drawThumb(g);
        } else if (isForeshadowed) {
            drawTrack(g, foreshadowedAlpha);
            drawThumb(g, foreshadowedAlpha);
        }
    }

    void resized() override {}
    
    void setEnabled(bool e) {isEnabled = e;}
    void setSelected(bool s) {isSelected = s;}
    void setForeshadowed(bool f) {isForeshadowed = f;}
    
    void mouseEnter(const MouseEvent& event) override {
        isHovered = true;
        showLabels();
        repaint();
    }
    
    void mouseExit(const MouseEvent& event) override {
        isHovered = false;
        hideLabels();
        repaint();
    }
    
    void mouseDown(const MouseEvent& event) override {
        if (isMouseOverThumb(event)) {
            dragOriginX = event.x;
            dragOriginY = event.y;
        } else if (isMouseOverTrack(event)) {
            dragOriginY = event.y;
        }
    }
    
    void mouseDrag(const MouseEvent& event) override {
        if (dragOriginY > -1) {
            updateY(event.getDistanceFromDragStartY());
        }
        if (dragOriginX > -1) {
            updateX(event.getDistanceFromDragStartX());
        }
    }
    
    void mouseUp(const MouseEvent& event) override {
        dragOriginY = -1;
        dragOriginX = -1;
    }

private:
    //Member variables for state
    Listener* listener;
    Component& parent;
    bool isEnabled = false;
    bool isSelected = false;
    bool isForeshadowed = false;
    bool isHovered = false;
    NormalisableRange<float> xRange = NormalisableRange<float>(-100.0f, 0.0f);
    NormalisableRange<float> yRange = NormalisableRange<float>(1.0f, 1000.0f);
    Label gainLabel, delayLabel;
    int dragOriginY = -1;
    int dragOriginX = -1;
    float yRatio = 0.0f;
    float xRatio = 0.0f;
    
    //Member variables that affect the look and feel
    float trackWidth = 6.0f;
    float outlineThickness = 2.0f;
    float outlineOffset = 4.0f;
    //Colours
    Colour trackColour = Colours::white;
    Colour thumbColour = LushLookAndFeel::colourAccent;
    Colour outlineColour = Colours::white;
    float foreshadowedAlpha = 0.5f;
    float hoveredAlpha = 0.5f;
    
    //Shared member variables to make "draw" functions faster
    float center = 0.0f;
    float thumbRadius = 0.0f;
    float thumbRangeY = 0.0f;
    float thumbCenterY = 0.0f;
    float thumbX = 0.0f;
    float thumbY = 0.0f;
    float trackX = 0.0f;
    float trackY = 0.0f;
    float trackHeight = 0.0f;

    float getXValue(){
        return xRange.convertFrom0to1(xRatio);
    }
    float getYValue(){
        return yRange.convertFrom0to1(yRatio);
    }
    
    void updateX(int dragX){
        int newX = dragOriginX + dragX;
        xRatio = (float)newX / getWidth();
        xRatio = jlimit(0.0f, 1.0f, xRatio);
        updateLabels();
        repaint();
    }
    
    void updateY(int dragY){
        int newY = dragOriginY + dragY;
        yRatio = (float)newY / thumbRangeY;
        yRatio = jlimit(0.0f, 1.0f, yRatio);
        updateLabels();
        repaint();
        
        stm::DebugDisplay::set(6, "yRatio: " + String(yRatio));
        stm::DebugDisplay::set(7, "newY: " + String(newY));
        stm::DebugDisplay::set(8, "dragOriginY: " + String(dragOriginY));
    }
    
    
    void calculate(){
        center = getWidth() / 2;
        
        thumbRadius = getWidth() / 2 - outlineOffset;
        thumbRangeY = getHeight() - thumbRadius * 2;
        thumbCenterY = yRatio * thumbRangeY + thumbRadius;
        thumbX = outlineOffset;
        thumbY = thumbCenterY - thumbRadius - outlineOffset;
        
        trackX = center - trackWidth/2;
        trackY = thumbRadius + outlineOffset - trackWidth/2;
        trackHeight = getHeight() - trackY * 2;
    }
    
    void drawOutline(Graphics& g, float alpha = 1.0f ){
        float trackWO = trackWidth + outlineOffset * 2;
        float trackHO= trackHeight + outlineOffset * 2;
        float thumbRO = getWidth()/2;
        float thumbRI = thumbRO - outlineThickness;
        float gap = outlineOffset - outlineThickness;
        
        g.setColour(outlineColour.withAlpha(alpha));
        g.drawRoundedRectangle(trackX - gap - outlineThickness/2, trackY - gap - outlineThickness/2, trackWO - outlineThickness, trackHO - outlineThickness, (trackWO - outlineThickness) / 2, outlineThickness);
        
        g.fillRoundedRectangle(0.0f, thumbY - outlineOffset, thumbRO*2, thumbRO*2, thumbRO);
        
        g.setColour(Colours::black);
        g.fillRoundedRectangle(0.0f + outlineThickness, thumbY - gap, thumbRI * 2, thumbRI * 2, thumbRI);
    }
    
    void drawTrack(Graphics& g, float alpha = 1.0f){
        g.setColour(trackColour.withAlpha(alpha));
        g.fillRoundedRectangle(trackX, trackY, trackWidth, trackHeight, trackWidth / 2);
    }
    
    void drawThumb(Graphics& g, float alpha = 1.0f ){
        g.setColour(thumbColour.withAlpha(alpha));
        g.fillRoundedRectangle(thumbX, thumbY, thumbRadius * 2, thumbRadius * 2, thumbRadius);
    }
    
    bool isMouseOverThumb(const MouseEvent& event) {
        float dx = event.x - center;
        float dy = event.y - thumbCenterY;
        float distance = sqrt(dx * dx + dy * dy);
        if (distance < thumbRadius) {
            return true;
        }
        return false;
    }
                   
    bool isMouseOverTrack(const MouseEvent& event) {
        if (isWithin<int>(event.x, center, trackWidth/2)
            && isWithin<int>(event.y, getHeight()/2, trackHeight))
        {
            return true;
        }
        return false;
    }
    
    void updateLabels() {
        String gainString = String(getYValue()) + "dB";
        gainLabel.setText(gainString, dontSendNotification);
        
        String delayString = String(getXValue()) + "ms";
        delayLabel.setText(delayString, dontSendNotification);
    }
    
    void showLabels() {
        getParentComponent()->addAndMakeVisible(gainLabel);
        getParentComponent()->addAndMakeVisible(delayLabel);
    }
    
    void hideLabels() {
        getParentComponent()->removeChildComponent(&gainLabel);
        getParentComponent()->removeChildComponent(&delayLabel);
    }
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MovingSlider)
};
