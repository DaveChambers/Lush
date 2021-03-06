/*
  ==============================================================================

    LushLookAndFeel.h
    Created: 23 Apr 2020 1:48:56pm
    Author:  Spenser Saling

  ==============================================================================
*/

#pragma once

class LushLookAndFeel : public LookAndFeel_V4
{
public:
    LushLookAndFeel()
    {
        setColour (Slider::thumbColourId, colourAccent);
        setColour (Slider::trackColourId, colourOff);
        setColour (Slider::backgroundColourId, colourOff);
        setColour (Slider::rotarySliderFillColourId, colourRotary);
        setColour (TextButton::textColourOffId, colourFont);
        setColour (TextButton::textColourOnId, colourFont);
        setColour (Label::textColourId, colourFont);
        
        Typeface::Ptr tface = Typeface::createSystemTypefaceFor(BinaryData::MontserratThin_ttf, BinaryData::MontserratThin_ttfSize);
        setDefaultSansSerifTypeface (tface);
        LookAndFeel::getDefaultLookAndFeel().setDefaultSansSerifTypeface(tface);
        LookAndFeel::getDefaultLookAndFeel().setDefaultSansSerifTypefaceName("Avenir Light");
    }
    
    //==============================================================================
    int getSliderThumbRadius (Slider& slider) override
    {
        return 18;
//        return jmin (12, slider.isHorizontal() ? static_cast<int> (slider.getHeight() * 0.5f)
//                                               : static_cast<int> (slider.getWidth()  * 0.5f));
    }
    
    void drawRotarySlider (Graphics& g, int x, int y, int width, int height, float sliderPos, const float rotaryStartAngle, const float rotaryEndAngle, Slider& slider) override
    {
        auto radius = jmin (width / 2, height / 2) - 4.0f;
        auto centreX = x + width  * 0.5f;
        auto centreY = y + height * 0.5f;
        auto rx = centreX - radius;
        auto ry = centreY - radius;
        auto rw = radius * 2.0f;
        auto angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

        g.setColour( slider.findColour(Slider::rotarySliderFillColourId) );
        g.fillEllipse (rx, ry, rw, rw);
        
        Path p;
        auto pointerLength = radius * 0.4f;
        auto pointerThickness = pointerLength / 4;
        p.addRectangle (-pointerThickness * 0.5f, -radius, pointerThickness, pointerLength);
        p.applyTransform (AffineTransform::rotation (angle).translated (centreX, centreY));
        // pointer
        g.setColour (Colours::white);
        g.fillPath (p);
    }
    
    void drawButtonBackground (Graphics& g, Button& button, const Colour& backgroundColour, bool shouldDrawButtonAsHighlighted,
                                               bool shouldDrawButtonAsDown) override
    {
        auto cornerSize = 6.0f;
        auto bounds = button.getLocalBounds().toFloat().reduced (0.5f, 0.5f);
        
        if (button.getToggleState()){
            g.setColour (colourAccent);
            g.drawRoundedRectangle (bounds, cornerSize, 3.0f);
        }
    }
    
    inline static const Colour
        colourAccent = Colour::fromHSV(109.0f/360.0f, 50.0f/100.0f, 90.0f/100.0f, 0xff),
        colourBackground = Colours::black,
        colourFont = Colour::fromHSV(0.0f, 0.0f, 0.8, 1.0),
        colourPanel = Colour::fromHSV(0.0f, 0.0f, 30.0f/100.0f, 0xff),
        colourRotary = Colour::fromHSV(0.0f, 0.0f, 70.0f/100.0f, 0xff),
        colourOff = Colour::fromHSV(0.0f, 0.0f, 0.45, 0x11)
    ;

private:
    
};
