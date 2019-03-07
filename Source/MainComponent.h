/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "SynthSource.h"

class MainComponent   : public AudioAppComponent,
                        public Slider::Listener,
                        private Timer
{
   
    
public:
    //==============================================================================
    MainComponent();
    ~MainComponent();

    //==============================================================================
    
   

    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;
    void synthChoiceChanged();
    
    

    //==============================================================================
    void paint (Graphics& g) override;
    void resized() override;
    
    
    //Makes L,R Sliders co-dependent
    void sliderValueChanged(Slider *slider) override {
        if(slider == &leftSlider) {
            rightSlider.setValue(1.0 - leftSlider.getValue());
        }
        else if(slider == &rightSlider) {
            leftSlider.setValue(1.0 - rightSlider.getValue());
        }
        
        else if(slider == &masterSlider) {
            master = masterSlider.getValue();
        }
    }

private:
    //==============================================================================
    
    Random random;  //for noise
    
    Slider noiseSlider;
    Label noiseSliderLabel;
    float noiseTargetLevel;
    float noiseRampLengthSamples;
    int noiseSamplesToTarget;
    
    Slider leftSlider;
    Label leftSliderLabel;
    float leftTargetLevel;
    float leftRampLengthSamples;
    int leftSamplesToTarget;
    
    Slider rightSlider;
    Label rightSliderLabel;
    float rightTargetLevel;
    float rightRampLengthSamples;
    int rightSamplesToTarget;
    
    Slider masterSlider;
    Label masterSliderLabel;
    float masterTargetLevel;
    float masterRampLengthSamples;
    int masterSamplesToTarget;
    
    Label synthChoiceLabel{{}, "Synth: "};
    ComboBox synthChoice;
    
    SynthAudioSource synthAudioSource;
    MidiKeyboardState keyboardState;
    MidiKeyboardComponent keyboardComponent;
    
    ComboBox midiInputList;
    Label midiInputListLabel;
    int lastInputIndex = 0;
    
    void timerCallback() override;
    void setMidiInput(int index);
    
    int master;
    
    Slider attackSlider;
    Label attackSliderLabel;
    
    Slider decaySlider;
    Label decaySliderLabel;
    
    Slider sustainSlider;
    Label sustainSliderLabel;
    
    Slider releaseSlider;
    Label releaseSliderLabel;
    
    Slider lpCutoffSlider;
    Label lpCutoffLabel;
    
    Slider lpResSlider;
    Label lpResLabel;
    
    Slider hpCutoffSlider;
    Label hpCutoffLabel;
    
    
    Slider hpResSlider;
    Label hpResLabel;
    
    Label envelope;
    Label lp;
    Label hp;
    //==========================================================================
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};





