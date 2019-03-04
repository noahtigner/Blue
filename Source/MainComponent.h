/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "SynthSource.h"
#include "SineWave.h"
//#include "SquareWave.h"
//#include "WaveformViewComponent.h"
//#include "SpectralViewComponent.h"


//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/

//==============================================================================




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
    // Your private member variables go here...
    
    //AnalyserComponent analyzer;
    
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
    
    /*
    Array<Colour> colours {
        Colour (25, 25, 25),    //black
        Colour (33, 33, 33),    //darkGrey
        Colour (255, 255, 255), //white
        Colour (232, 229, 229), //offWhite
        Colour (191, 191, 191), //gray
        Colour (15, 199, 149),  //blue
        Colour (63, 33, 89)     //purple
    }; */
    
    /*
    Synthesiser mySynth;
    SynthVoice* myVoice;
    double lastSampleRate;
     */
    
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
    

    //WaveformViewComponent waveformView;
    //SpectralViewComponent spectralView;
    
    
    //==========================================================================
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};





