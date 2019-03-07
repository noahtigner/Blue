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
         //for (auto i = 0; i < 4; ++i) {               //add some voices to our synthesiser. This number of voices added determines the polyphony
   
         */
        switch(synthType) {
            case 1:
                for(int i = 0; i < 4; i++) {    //4 polyphony
                    sineVoices[i] = new SynthVoice();
                    //sineVoices[i]->masterSlider = masterSlider;
                    //masterSlider->addListener(sineVoices[i]);
                    
                    
                    synth.addVoice(sineVoices[i]);
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
    
    void comboBoxChanged(ComboBox *box) override{
        for(int i = 0; i < 4; i++) {
            sineVoices[i]->synthChoice = synthChoice;
            synthChoice->addListener(sineVoices[i]);
        }
    }
    
    
    void sliderValueChanged(Slider *slider) override {
        
        for(int i = 0; i < 4; i++) {
#warning Pull Out of Loop
            
            sineVoices[i]->masterSlider = masterSlider;
            masterSlider->addListener(sineVoices[i]);
            
            sineVoices[i]->leftSlider = leftSlider;
            leftSlider->addListener(sineVoices[i]);
            
            sineVoices[i]->rightSlider = rightSlider;
            rightSlider->addListener(sineVoices[i]);
            
            sineVoices[i]->noiseSlider = noiseSlider;
            noiseSlider->addListener(sineVoices[i]);
            
            
            sineVoices[i]->attackSlider = attackSlider;
            attackSlider->addListener(sineVoices[i]);
            
            sineVoices[i]->decaySlider = decaySlider;
            decaySlider->addListener(sineVoices[i]);
            
            sineVoices[i]->sustainSlider = sustainSlider;
            sustainSlider->addListener(sineVoices[i]);
            
            sineVoices[i]->releaseSlider = releaseSlider;
            releaseSlider->addListener(sineVoices[i]);
            
            sineVoices[i]->lpCutoffSlider = lpCutoffSlider;
            lpCutoffSlider->addListener(sineVoices[i]);
            
            sineVoices[i]->lpResSlider = lpResSlider;
            lpResSlider->addListener(sineVoices[i]);
            
            //std::cout<<hpCutoffSlider->getValue();
            sineVoices[i]->hpCutoffSlider = hpCutoffSlider;
            hpCutoffSlider->addListener(sineVoices[i]);
            
            sineVoices[i]->hpResSlider = hpResSlider;
            hpResSlider->addListener(sineVoices[i]);
            
            
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

    SynthVoice* sineVoices[4];
    
    int synthType = 1;
};

