/*
  ==============================================================================

    SynthSource.h
    Created: 13 Feb 2019 12:20:24pm
    Author:  Noah Tigner

  ==============================================================================
*/

#pragma once
#include "SineWave.h"



//==============================================================================

class SynthAudioSource: public AudioSource,
                        public Slider::Listener

{
public:
    
    Slider *noiseSlider;
    Slider *leftSlider;
    Slider *rightSlider;
    Slider *masterSlider;
    
    
    SynthAudioSource(MidiKeyboardState& keyState):
            keyboardState (keyState)
    {
        /*
        //for (auto i = 0; i < 4; ++i) {               //add some voices to our synthesiser. This number of voices added determines the polyphony
            sine = new SineWaveVoice();
            sine2 = new SineWaveVoice();
            sine3 = new SineWaveVoice();
            sine4 = new SineWaveVoice();
         */
        
        //sineVoices[0] = new NoiseVoice();
        
        for(int i = 0; i < 4; i++) {    //4 polyphony
            sineVoices[i] = new SineWaveVoice();
            //sineVoices[i]->masterSlider = masterSlider;
            //masterSlider->addListener(sineVoices[i]);
            synth.addVoice(sineVoices[i]);
        }
        
        
        synth.addVoice(noiseV);

        synth.addSound (new SineWaveSound());       //add the sound so that the synthesiser knows which sounds it can play.
    }
    
    ~SynthAudioSource(){}
    
    void setUsingSineWaveSound()
    {
        synth.clearSounds();
    }
    
    
    void sliderValueChanged(Slider *slider) override {

        for(int i = 0; i < 4; i++) {
            sineVoices[i]->masterSlider = masterSlider;
            masterSlider->addListener(sineVoices[i]);
            
            sineVoices[i]->leftSlider = leftSlider;
            leftSlider->addListener(sineVoices[i]);
            
            sineVoices[i]->rightSlider = rightSlider;
            rightSlider->addListener(sineVoices[i]);
            
            sineVoices[i]->noiseSlider = noiseSlider;
            noiseSlider->addListener(sineVoices[i]);
        }
        noiseV->masterSlider = masterSlider;
        masterSlider->addListener(noiseV);
        
        noiseV->leftSlider = leftSlider;
        leftSlider->addListener(noiseV);
        
        noiseV->rightSlider = rightSlider;
        rightSlider->addListener(noiseV);
        
        noiseV->noiseSlider = noiseSlider;
        noiseSlider->addListener(noiseV);
        
         
      
        
        if(slider == masterSlider) {
            //master = masterSlider->getValue();
            std::cout << "hey";
        }
    }
    
    
    
    void getLevel(){}
    
    void prepareToPlay (int /*samplesPerBlockExpected*/, double sampleRate) override
    {
        synth.setCurrentPlaybackSampleRate (sampleRate); //synthesiser needs to know the sample rate of the audio output.
        midiCollector.reset(sampleRate);    //order to process the timestamps of the MIDI data the MidiMessageCollector class needs to know the audio sample rate
    }
    
    void releaseResources() override {}
    
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override
    {
        bufferToFill.clearActiveBufferRegion();
        
        MidiBuffer incomingMidi;
        midiCollector.removeNextBlockOfMessages (incomingMidi, bufferToFill.numSamples); //pull any MIDI messages for each block of audio
        keyboardState.processNextMidiBuffer (incomingMidi, bufferToFill.startSample,
                                             bufferToFill.numSamples, true);       //pull buffers of MIDI data from the MidiKeyboardState objec
        
        synth.renderNextBlock (*bufferToFill.buffer, incomingMidi,
                               bufferToFill.startSample, bufferToFill.numSamples); //buffers of MIDI are passed to the synthesiser which will be used to render the voices using the timestamps of the note-on and note-off messages
    }
    
    MidiMessageCollector* getMidiCollector()
    {
        //We'll need access to this MidiMessageCollector object from outside the SynthAudioSource class, so add an accessor
        return &midiCollector;
    }
    
private:
    MidiKeyboardState& keyboardState;
    Synthesiser synth;
    MidiMessageCollector midiCollector;
    

    
    SineWaveVoice* sineVoices[4];
    
    NoiseVoice* noiseV = new NoiseVoice();
    
    
};
