/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Params.h"


LushAudioProcessor::LushAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       ),
#else
    :
#endif
parameters (*this, &undoManager, Identifier ("Lush"), Params::createParameterLayout()),
lushEngine(parameters)
{
    
}

LushAudioProcessor::~LushAudioProcessor()
{
    
}

//==============================================================================
const String LushAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool LushAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool LushAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool LushAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double LushAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int LushAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int LushAudioProcessor::getCurrentProgram()
{
    return 0;
}

void LushAudioProcessor::setCurrentProgram (int index)
{
}

const String LushAudioProcessor::getProgramName (int index)
{
    return {};
}

void LushAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void LushAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    auto totalNumInputChannels  = getTotalNumInputChannels();
    
    lushEngine.prepare({ sampleRate, (uint32) samplesPerBlock, (uint32) totalNumInputChannels });

    //Reports latency to the DAW
    setLatencySamples(0);
}

void LushAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool LushAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void LushAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

//    //Allows for reading of current playback position/bpm/etc.
//    AudioPlayHead* phead = getPlayHead();
//    if (phead != nullptr)
//    {
//    AudioPlayHead::CurrentPositionInfo playposinfo;
//    phead->getCurrentPosition(playposinfo);
//    }
    
    auto block = juce::dsp::AudioBlock<float> (buffer);
    auto context = juce::dsp::ProcessContextReplacing<float> (block);
    lushEngine.process(context);
}

//==============================================================================
bool LushAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* LushAudioProcessor::createEditor()
{
    return new LushAudioProcessorEditor (*this);
}

//==============================================================================
void LushAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    auto state = parameters.copyState();
    std::unique_ptr<XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);
}

void LushAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
   std::unique_ptr<XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));

   if (xmlState.get() != nullptr)
       if (xmlState->hasTagName (parameters.state.getType()))
           parameters.replaceState (ValueTree::fromXml (*xmlState));
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new LushAudioProcessor();
}
