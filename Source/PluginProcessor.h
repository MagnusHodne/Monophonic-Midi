#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
using namespace juce;

class MidiPluginProcessor : public AudioProcessor
{
public:
    MidiPluginProcessor()
            : AudioProcessor (BusesProperties())
    {
        addParameter(retriggerVelocity = new AudioParameterInt("velocity", "Retrigger velocity", 0, 127, 64));
    }
    ~MidiPluginProcessor() override = default;

    void prepareToPlay (double sampleRate, int samplesPerBlock) override {
        // Use this method as the place to do any pre-playback
        // initialisation that you need...
        ignoreUnused (samplesPerBlock);

        channelNumber = 1;
    }

    void releaseResources() override {}

    void processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages) override
    {
        //The audio buffer in a midi effect should have zero channels...
        jassert(buffer.getNumChannels() == 0);
        buffer.clear();

        MidiBuffer processedMidi; //This is the "output" buffer

        //Iterate over each midi message in buffer
        for (const auto metadata : midiMessages)
        {
            auto message = metadata.getMessage();
            const auto samplePos = metadata.samplePosition;

            if(message.isNoteOn()){
                processedMidi.addEvent(message, samplePos);
                if(currentNote != -1){
                    processedMidi.addEvent(MidiMessage::noteOff(channelNumber, currentNote), samplePos);
                }
                lastNote = currentNote;
                currentNote = message.getNoteNumber();

            } else if(message.isNoteOff()){
                if(message.getNoteNumber() == currentNote){
                    if(lastNote != -1){
                        processedMidi.addEvent(MidiMessage::noteOn(channelNumber, lastNote, (uint8) *retriggerVelocity), samplePos);
                        currentNote = lastNote;
                        lastNote = -1;
                    } else {
                        currentNote = -1;
                    }
                    processedMidi.addEvent(message, samplePos);

                } else if(message.getNoteNumber() == lastNote){
                    lastNote = -1;
                }
            } else {
                processedMidi.addEvent(message, samplePos);
            }
        }
        midiMessages.swapWith (processedMidi);
    }

    using AudioProcessor::processBlock;

    bool isBusesLayoutSupported (const BusesLayout& layouts) const override {
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

    AudioProcessorEditor* createEditor() override { return new GenericAudioProcessorEditor (*this); }
    bool hasEditor() const override { return true; }

    const String getName() const override { return JucePlugin_Name; }

    bool acceptsMidi() const override {
        #if JucePlugin_WantsMidiInput
            return true;
        #else
            return false;
        #endif
    }
    bool producesMidi() const override {
        #if JucePlugin_ProducesMidiOutput
            return true;
        #else
            return false;
        #endif
    }
    bool isMidiEffect() const override {
        #if JucePlugin_IsMidiEffect
            return true;
        #else
            return false;
        #endif
    }
    double getTailLengthSeconds() const override { return 0.0; }

    // NB: some hosts don't cope very well if you tell them there are 0 programs,
    // so this should be at least 1, even if you're not really implementing programs.
    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram (int index) override { ignoreUnused(index); }
    const String getProgramName (int index) override {
        ignoreUnused (index);
        return { "None" };
    }
    void changeProgramName (int index, const String& newName) override { ignoreUnused(index, newName); }

    void getStateInformation (MemoryBlock& destData) override {
        // You should use this method to store your parameters in the memory block.
        // You could do that either as raw data, or use the XML or ValueTree classes
        // as intermediaries to make it easy to save and load complex data.
        ignoreUnused(destData); }
    void setStateInformation (const void* data, int sizeInBytes) override {
        // You should use this method to restore your parameters from this memory block,
        // whose contents will have been created by the getStateInformation() call.
        ignoreUnused(data, sizeInBytes);
    }

private:
    int channelNumber;
    int currentNote {-1};
    int lastNote {-1};

    AudioParameterInt* retriggerVelocity;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MidiPluginProcessor)
};


