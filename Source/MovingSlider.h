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
class MovingSlider : public Component, public stm::ParameterAttachment::Listener
{
public:
    struct Listener {
        virtual ~Listener(){}
        virtual void xChanged(float xValue) = 0;
        virtual void yChanged(float yValue) = 0;
    };
    
    MovingSlider(Component& p, AudioProcessorValueTreeState& s, int voice)
        : parent(p), state(s),
        gainAttachment(s, Params::getVoiceParamId(voice, Params::GAIN)),
        delayAttachment(s, Params::getVoiceParamId(voice, Params::DELAY)),
        enabledAttachment(s, Params::getVoiceParamId(voice, Params::ENABLED))
    {
        idEnabled = Params::getVoiceParamId(voice, Params::ENABLED);
        idDelay = Params::getVoiceParamId(voice, Params::DELAY);
        idGain = Params::getVoiceParamId(voice, Params::GAIN);
        
        gainAttachment.setListener(this);
        delayAttachment.setListener(this);
        enabledAttachment.setListener(this);
        
        xRange = state.getParameterRange(idDelay);
        yRange = state.getParameterRange(idGain);
        
        gainLabel.setJustificationType(Justification::centredTop);
        delayLabel.setJustificationType(Justification::centredBottom);
    }

    ~MovingSlider(){
        gainAttachment.removeListener();
        delayAttachment.removeListener();
        enabledAttachment.removeListener();
    }

    void paint (Graphics& g) override
    {
        if (isEnabled) {
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

    void resized() override {
        updateBounds();
    }
    
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
            dragOriginY = event.y;
            auto parentEvent = event.getEventRelativeTo(&parent);
            dragOriginX = parentEvent.x;
        }
//        else if (isMouseOverTrack(event)) {
//            dragOriginX = event.x;
//        }
        else {
            auto parentEvent = event.getEventRelativeTo(&parent);
            dragOriginX = parentEvent.x;
        }
    }
    
    void mouseDrag(const MouseEvent& event) override {
        if (dragOriginY >= 0) {
            updateY(event.getDistanceFromDragStartY());
        }
        if (dragOriginX >= 0) {
            updateX(event.getDistanceFromDragStartX());
        }
    }
    
    void mouseUp(const MouseEvent& event) override {
        dragOriginY = -1;
        dragOriginX = -1;
    }
    
    void setListener(Listener* newListener) { listener = newListener; }
    void removeListener(){ listener = nullptr; }
    
    void valueUpdated(stm::ParameterAttachment* attachment, float newValue) override {
        if (attachment == &gainAttachment){
            yRatio = yRange.convertTo0to1(newValue);
        } else if (attachment == &delayAttachment){
            xRatio = xRange.convertTo0to1(newValue);
        } else if (attachment == &enabledAttachment){
            
        }
    }

private:
    //Member variables for state
    Component& parent;
    AudioProcessorValueTreeState& state;
    stm::ParameterAttachment gainAttachment, delayAttachment, enabledAttachment;
    Listener* listener = nullptr;
    String idEnabled, idDelay, idGain;
    //std::unique_ptr<stm::ParameterAttachment> gainAttachment, delayAttachment, enabledAttachment;
    bool isEnabled = false;
    bool isSelected = false;
    bool isForeshadowed = false;
    bool isHovered = false;
    NormalisableRange<float> yRange;
    NormalisableRange<float> xRange;// = NormalisableRange<float>(0.0f, 1.0f);
    Label gainLabel, delayLabel;
    int dragOriginY = -1;
    int dragOriginX = -1;
    float yRatio = 0.3f;
    float xRatio = 0.3f;
    int xPadding = 40;
    
    //Member variables that affect the look and feel
    const int labelWidth = 100;
    const int labelHeight = 50;
    const int sliderWidth = 28;
    const float trackWidth = 4.0f;
    const float outlineThickness = 2.0f;
    const float outlineOffset = 4.0f;
    const float outerRadius = sliderWidth / 2;
    const float thumbRadius = outerRadius - outlineOffset;
    const Colour trackColour = Colours::white;
    const Colour thumbColour = LushLookAndFeel::colourAccent;
    const Colour outlineColour = Colours::white;
    const float foreshadowedAlpha = 0.5f;
    const float hoveredAlpha = 0.5f;
    
    
    float getXValue(){ return xRange.convertFrom0to1(xRatio); }
    float getYValue(){ return yRange.convertFrom0to1(yRatio); }
    float getXRange(){ return parent.getWidth() - xPadding * 2; }
    float getYRange(){ return getHeight() - outerRadius * 2; }
    float getXPixels(){ return xPadding + getXRange() * xRatio; }
    float getYPixels(){ return outerRadius + getYRange() * (yRatio); }
    Rectangle<float> getBoundsAtSliderCenter(float width, float height){
        auto bounds = Rectangle<float>(width, height);
        bounds.setCentre(getWidth()/2, getHeight()/2);
        return bounds;
    }
    Rectangle<float> getBoundsAtThumbCenter(float width, float height){
        auto bounds = Rectangle<float>(width, height);
        bounds.setCentre(getWidth()/2, getYPixels());
        return bounds;
    }
    
    void updateBounds() {
        auto bounds = parent.getLocalBounds();
        bounds = bounds.withX(int(getXPixels()) - sliderWidth / 2);
        bounds = bounds.reduced(0, labelHeight).withWidth(sliderWidth);
        setBounds(bounds);
        
        bounds = parent.getLocalBounds();
        bounds = bounds.withX(int(getXPixels()) - labelWidth / 2);
        bounds = bounds.withWidth(labelWidth);
        delayLabel.setBounds(bounds.removeFromTop(labelHeight));
        gainLabel.setBounds(bounds.removeFromBottom(labelHeight));
    }
    
    void updateX(int dragX){
        int newX = dragOriginX + dragX - xPadding;
        xRatio = (float)newX / (parent.getWidth() - 2*xPadding);
        xRatio = jlimit(0.0f, 1.0f, xRatio);
        updateLabelText();
        updateBounds();
        
        if (listener != nullptr) {
            listener->xChanged(getXValue());
        }
        
        delayAttachment.setValue(xRange.convertFrom0to1(xRatio));
        
        repaint();
    }
    
    void updateY(int dragY){
        int newY = dragOriginY + dragY - thumbRadius - outlineOffset;
        yRatio = (float)newY / getYRange();
        yRatio = jlimit(0.0f, 1.0f, yRatio);
        updateLabelText();
        updateBounds();
        
        if (listener != nullptr) {
            listener->yChanged(getYValue());
        }
        
        gainAttachment.setValue(yRange.convertFrom0to1(yRatio));
        
        repaint();
    }
    
    void drawOutline(Graphics& g, float alpha = 1.0f ){
        g.setColour(outlineColour.withAlpha(alpha));
        auto offsetTrackBounds = getBoundsAtSliderCenter(trackWidth + outlineOffset * 2 - outlineThickness,
                                                         getYRange() + trackWidth + outlineOffset*2 - outlineThickness);
        g.drawRoundedRectangle(offsetTrackBounds, trackWidth/2 + outlineOffset - outlineThickness/2, outlineThickness);
        
        g.setColour(Colours::black);
        auto offsetThumbBounds = getBoundsAtThumbCenter(getWidth(), getWidth());
        g.fillRoundedRectangle(offsetThumbBounds, getWidth()/2);
        
        offsetThumbBounds = getBoundsAtThumbCenter(getWidth() - outlineThickness, getWidth() - outlineThickness);
        g.setColour(outlineColour.withAlpha(alpha));
        g.drawRoundedRectangle(offsetThumbBounds, getWidth()/2 - outlineThickness, outlineThickness);
    }
    
    void drawTrack(Graphics& g, float alpha = 1.0f){
        g.setColour(trackColour.withAlpha(alpha));
        g.fillRoundedRectangle(getBoundsAtSliderCenter(trackWidth, getYRange() + trackWidth), trackWidth / 2);
    }
    
    void drawThumb(Graphics& g, float alpha = 1.0f ){
        g.setColour(thumbColour.withAlpha(alpha));
        g.fillRoundedRectangle(getBoundsAtThumbCenter(thumbRadius*2, thumbRadius*2), thumbRadius);
    }
    
    bool isMouseOverThumb(const MouseEvent& event) {
        float dx = event.x - outerRadius;
        float dy = event.y - getYPixels();
        float distance = sqrt(dx * dx + dy * dy);
        if (distance < outerRadius) {
            return true;
        }
        return false;
    }
                   
    bool isMouseOverTrack(const MouseEvent& event) {
        if (isWithin<int>(event.x, outerRadius, trackWidth/2)
            && isWithin<int>(event.y, getHeight()/2, getYRange() + trackWidth))
        {
            return true;
        }
        return false;
    }
    
    void updateLabelText() {
        String gainString = String::toDecimalStringWithSignificantFigures(getYValue(), 3) + "dB";
        gainLabel.setText(gainString, dontSendNotification);
        
        String delayString = String::toDecimalStringWithSignificantFigures(getXValue(), 3) + "ms";
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






















//class MovingSlider : public Component
//{
//public:
//    struct Listener {
//        virtual ~Listener(){}
//        virtual void xChanged(float xValue) = 0;
//        virtual void yChanged(float yValue) = 0;
//    };
//
//    MovingSlider(Component& p, AudioProcessorValueTreeState& s, int voice) : parent(p), state(s){
//        idEnabled = Params::getVoiceParamId(voice, Params::ENABLED);
//        idDelay = Params::getVoiceParamId(voice, Params::DELAY);
//        idGain = Params::getVoiceParamId(voice, Params::GAIN);
//
//        gainLabel.setJustificationType(Justification::centredTop);
//        delayLabel.setJustificationType(Justification::centredBottom);
//    }
//
//    ~MovingSlider(){}
//
//    void paint (Graphics& g) override
//    {
//        if (isEnabled) {
//            calculate();
//            drawTrack(g);
//            if (isSelected) {
//                drawOutline(g);
//            } else if (isHovered) {
//                drawOutline(g, hoveredAlpha);
//            }
//            drawThumb(g);
//
//        } else if (isForeshadowed) {
//            drawTrack(g, foreshadowedAlpha);
//            drawThumb(g, foreshadowedAlpha);
//        }
//    }
//
//    void resized() override {
//        updateBounds();
//    }
//
//    void setEnabled(bool e) {isEnabled = e;}
//    void setSelected(bool s) {isSelected = s;}
//    void setForeshadowed(bool f) {isForeshadowed = f;}
//
//    void mouseEnter(const MouseEvent& event) override {
//        isHovered = true;
//        showLabels();
//        repaint();
//    }
//
//    void mouseExit(const MouseEvent& event) override {
//        isHovered = false;
//        hideLabels();
//        repaint();
//    }
//
//    void mouseDown(const MouseEvent& event) override {
//        if (isMouseOverThumb(event)) {
//            dragOriginY = event.y;
//            auto parentEvent = event.getEventRelativeTo(&parent);
//            dragOriginX = parentEvent.x;
//        }
////        else if (isMouseOverTrack(event)) {
////            dragOriginX = event.x;
////        }
//        else {
//            auto parentEvent = event.getEventRelativeTo(&parent);
//            dragOriginX = parentEvent.x;
//        }
//    }
//
//    void mouseDrag(const MouseEvent& event) override {
//        if (dragOriginY >= 0) {
//            updateY(event.getDistanceFromDragStartY());
//        }
//        if (dragOriginX >= 0) {
//            updateX(event.getDistanceFromDragStartX());
//        }
//    }
//
//    void mouseUp(const MouseEvent& event) override {
//        dragOriginY = -1;
//        dragOriginX = -1;
//    }
//
//    void setListener(Listener* newListener) { listener = newListener; }
//    void removeListener(){ listener = nullptr; }
//
//private:
//    //Member variables for state
//    Listener* listener = nullptr;
//    Component& parent;
//    AudioProcessorValueTreeState& state;
//    String idEnabled, idDelay, idGain;
//    bool isEnabled = false;
//    bool isSelected = false;
//    bool isForeshadowed = false;
//    bool isHovered = false;
//    NormalisableRange<float> yRange;
//    NormalisableRange<float> xRange;// = NormalisableRange<float>(0.0f, 1.0f);
//    Label gainLabel, delayLabel;
//    int dragOriginY = -1;
//    int dragOriginX = -1;
//    float yRatio = 0.3f;
//    float xRatio = 0.3f;
//    int xPadding = 40;
//
//    //Member variables that affect the look and feel
//    int labelWidth = 100;
//    int labelHeight = 45;
//    int sliderWidth = 28;
//    float trackWidth = 4.0f;
//    float outlineThickness = 2.0f;
//    float outlineOffset = 4.0f;
//    //Colours
//    Colour trackColour = Colours::white;
//    Colour thumbColour = LushLookAndFeel::colourAccent;
//    Colour outlineColour = Colours::white;
//    float foreshadowedAlpha = 0.5f;
//    float hoveredAlpha = 0.5f;
//
//    //Shared member variables to make "draw" functions faster
//    float center = 0.0f;
//    float thumbRadius = 0.0f;
//    float thumbRangeY = 0.0f;
//    float thumbCenterY = 0.0f;
//    float thumbX = 0.0f;
//    float thumbY = 0.0f;
//    float trackX = 0.0f;
//    float trackY = 0.0f;
//    float trackHeight = 0.0f;
//
//    float getXValue(){ return xRange.convertFrom0to1(xRatio); }
//    float getYValue(){ return yRange.convertFrom0to1(1-yRatio); }
//
//    void updateX(int dragX){
//        int newX = dragOriginX + dragX - xPadding;
//        xRatio = (float)newX / (parent.getWidth() - 2*xPadding);
//        xRatio = jlimit(0.0f, 1.0f, xRatio);
//        updateLabelText();
//        updateBounds();
//
//        if (listener != nullptr) {
//            listener->xChanged(getXValue());
//        }
//
//        stm::DebugDisplay::set(6, "newX: " + String(newX));
//        stm::DebugDisplay::set(7, "dragOriginX: " + String(dragOriginX));
//        stm::DebugDisplay::set(8, "xRatio: " + String(xRatio));
//
//        repaint();
//    }
//
//    void updateY(int dragY){
//        int newY = dragOriginY + dragY - thumbRadius - outlineOffset;
//        yRatio = (float)newY / thumbRangeY;
//        yRatio = jlimit(0.0f, 1.0f, yRatio);
//        updateLabelText();
//        updateBounds();
//
//        stm::DebugDisplay::set(6, "newY: " + String(newY));
//        stm::DebugDisplay::set(7, "dragOriginY: " + String(dragOriginY));
//        stm::DebugDisplay::set(8, "yRatio: " + String(yRatio));
//
//        if (listener != nullptr) {
//            listener->yChanged(getYValue());
//        }
//
//        repaint();
//    }
//
//
//    void calculate(){
//        center = getWidth() / 2;
//
//        thumbRadius = getWidth() / 2 - outlineOffset;
//        thumbRangeY = getHeight() - (thumbRadius + outlineOffset) * 2;
//        thumbCenterY = yRatio * thumbRangeY + thumbRadius + outlineOffset;
//        thumbX = outlineOffset;
//        thumbY = thumbCenterY - thumbRadius;
//
//        trackX = center - trackWidth/2;
//        trackY = thumbRadius + outlineOffset - trackWidth/2;
//        trackHeight = getHeight() - trackY * 2;
//    }
//
//    void drawOutline(Graphics& g, float alpha = 1.0f ){
//        float trackWO = trackWidth + outlineOffset * 2;
//        float trackHO= trackHeight + outlineOffset * 2;
//        float thumbRO = getWidth()/2;
//        float thumbRI = thumbRO - outlineThickness;
//        float gap = outlineOffset - outlineThickness;
//
//        g.setColour(outlineColour.withAlpha(alpha));
//        g.drawRoundedRectangle(trackX - gap - outlineThickness/2, trackY - gap - outlineThickness/2, trackWO - outlineThickness, trackHO - outlineThickness, (trackWO - outlineThickness) / 2, outlineThickness);
//
//        g.fillRoundedRectangle(0.0f, thumbY - outlineOffset, thumbRO*2, thumbRO*2, thumbRO);
//
//        g.setColour(Colours::black);
//        g.fillRoundedRectangle(0.0f + outlineThickness, thumbY - gap, thumbRI * 2, thumbRI * 2, thumbRI);
//    }
//
//    void drawTrack(Graphics& g, float alpha = 1.0f){
//        g.setColour(trackColour.withAlpha(alpha));
//        g.fillRoundedRectangle(trackX, trackY, trackWidth, trackHeight, trackWidth / 2);
//    }
//
//    void drawThumb(Graphics& g, float alpha = 1.0f ){
//        g.setColour(thumbColour.withAlpha(alpha));
//        g.fillRoundedRectangle(thumbX, thumbY, thumbRadius * 2, thumbRadius * 2, thumbRadius);
//    }
//
//    bool isMouseOverThumb(const MouseEvent& event) {
//        float dx = event.x - center;
//        float dy = event.y - thumbCenterY;
//        float distance = sqrt(dx * dx + dy * dy);
//        if (distance < thumbRadius) {
//            return true;
//        }
//        return false;
//    }
//
//    bool isMouseOverTrack(const MouseEvent& event) {
//        if (isWithin<int>(event.x, center, trackWidth/2)
//            && isWithin<int>(event.y, getHeight()/2, trackHeight))
//        {
//            return true;
//        }
//        return false;
//    }
//
//    float getXPixels(){
//        int range = parent.getWidth() - 2 * xPadding;
//        return xPadding + float(range) * xRatio;
//    }
//
//    void updateBounds() {
//        auto bounds = parent.getLocalBounds();
//        bounds = bounds.withX(int(getXPixels()) - sliderWidth / 2);
//        bounds = bounds.reduced(0, labelHeight).withWidth(sliderWidth);
//        setBounds(bounds);
//
//        bounds = parent.getLocalBounds();
//        bounds = bounds.withX(int(getXPixels()) - labelWidth / 2);
//        bounds = bounds.withWidth(labelWidth);
//        delayLabel.setBounds(bounds.removeFromTop(labelHeight));
//        gainLabel.setBounds(bounds.removeFromBottom(labelHeight));
//    }
//
//    void updateLabelText() {
//        String gainString = String::toDecimalStringWithSignificantFigures(getYValue(), 3) + "dB";
//        gainLabel.setText(gainString, dontSendNotification);
//
//        String delayString = String::toDecimalStringWithSignificantFigures(getXValue(), 3) + "ms";
//        delayLabel.setText(delayString, dontSendNotification);
//    }
//
//    void showLabels() {
//        getParentComponent()->addAndMakeVisible(gainLabel);
//        getParentComponent()->addAndMakeVisible(delayLabel);
//    }
//
//    void hideLabels() {
//        getParentComponent()->removeChildComponent(&gainLabel);
//        getParentComponent()->removeChildComponent(&delayLabel);
//    }
//
//    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MovingSlider)
//};
















