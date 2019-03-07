/*
 ==============================================================================
 
 SynthSource.h
 Created: 13 Feb 2019 12:20:24pm
 Author:  Noah Tigner
 
 ==============================================================================
 */

#pragma once
#include "SynthSound.h"
#include "SynthVoice.h"

//==============================================================================

class SynthAudioSource: public AudioSource,
public Slider::Listener,
public ComboBox::Listener

{
public:
    
    Slider *noiseSlider;
    Slider *leftSlider;
    Slider *rightSlider;
    Slider *masterSlider;
    ComboBox *synthChoice;
    Slider *attackSlider;
    Slider *decaySlider;
    Slider *sustainSlider;
    Slider *releaseSlider;
    Slider *lpCutoffSlider;
    Slider *lpResSlider;
    Slider *hpCutoffSlider;
    Slider *hpResSlider;
    
    SynthAudioSource(MidiKeyboardState& keyState):
    keyboardState (keyState)
    {
        /*
         //for (auto i = 0; i < 4; ++i) {       //number of voices added determines the polyphony
   
         */
        switch(synthType) {
            case 1:
                for(int i = 0; i < 4; i++) {    //4 polyphony
                    Voices[i] = new SynthVoice();
                    //sineVoices[i]->masterSlider = masterSlider;
                    //masterSlider->addListener(sineVoices[i]);
                    
                    
                    synth.addVoice(Voices[i]);
                }
                break;
        }
        synth.addSound (new SynthSound());
    }
    
    ~SynthAudioSource(){}
    
    void setUsingSineWaveSound()
    {
        synth.clearSounds();
    }
    
    void comboBoxChanged(ComboBox *box) override {
        for(int i = 0; i < 4; i++) {
            Voices[i]->synthChoice = synthChoice;
            synthChoice->addListener(Voices[i]);
        }
    }
    
    
    void sliderValueChanged(Slider *slider) override {
        
        for(int i = 0; i < 4; i++) {
#warning Pull Out of Loop
            
            Voices[i]->masterSlider = masterSlider;
            masterSlider->addListener(Voices[i]);
            
            Voices[i]->leftSlider = leftSlider;
            leftSlider->addListener(Voices[i]);
            
            Voices[i]->rightSlider = rightSlider;
            rightSlider->addListener(Voices[i]);
            
            Voices[i]->noiseSlider = noiseSlider;
            noiseSlider->addListener(Voices[i]);
            
            
            Voices[i]->attackSlider = attackSlider;
            attackSlider->addListener(Voices[i]);
            
            Voices[i]->decaySlider = decaySlider;
            decaySlider->addListener(Voices[i]);
            
            Voices[i]->sustainSlider = sustainSlider;
            sustainSlider->addListener(Voices[i]);
            
            Voices[i]->releaseSlider = releaseSlider;
            releaseSlider->addListener(Voices[i]);
            
            Voices[i]->lpCutoffSlider = lpCutoffSlider;
            lpCutoffSlider->addListener(Voices[i]);
            
            Voices[i]->lpResSlider = lpResSlider;
            lpResSlider->addListener(Voices[i]);
            
            Voices[i]->hpCutoffSlider = hpCutoffSlider;
            hpCutoffSlider->addListener(Voices[i]);
            
            Voices[i]->hpResSlider = hpResSlider;
            hpResSlider->addListener(Voices[i]);
        }
    }
    
    void getLevel(){}
    
    void prepareToPlay (int /*samplesPerBlockExpected*/, double sampleRate) override {
        synth.setCurrentPlaybackSampleRate (sampleRate); //synthesiser needs to know the sample rate of the audio output.
        midiCollector.reset(sampleRate);    //order to process the timestamps of the MIDI data the MidiMessageCollector class needs to know the audio sample rate
    }
    
    void releaseResources() override {}
    
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override {
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
    SynthVoice* Voices[4];
    
    int synthType = 1;
};

