/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

//==============================================================================



//==============================================================================
MainComponent::MainComponent() {
    // specify the number of input and output channels that we want to open
    setAudioChannels (0, 2);    //no in, 2 out
    
    
                                    
                                    
                                    
    //noiseTargetLevel = 0.125;
    noiseTargetLevel = 0.0;
    noiseSlider.setRange(0.0, 0.25);
    noiseSlider.setValue(noiseTargetLevel, dontSendNotification);
    noiseSlider.setTextBoxStyle(Slider::TextBoxRight, false, 100, 20);
    noiseSlider.onValueChange = [this] {
        noiseTargetLevel = noiseSlider.getValue();
        noiseSamplesToTarget = noiseRampLengthSamples;
    };
    noiseSliderLabel.setText("Noise Level", dontSendNotification);
    
    leftTargetLevel = 0.5f;
    leftSlider.setRange(0.0, 1);
    leftSlider.setValue(leftTargetLevel, dontSendNotification);
    leftSlider.setTextBoxStyle(Slider::TextBoxRight, false, 100, 20);
    leftSlider.onValueChange = [this] {
        leftTargetLevel = leftSlider.getValue();
        leftSamplesToTarget = leftRampLengthSamples;
    };
    leftSlider.addListener(this);   //make MC a listener
    leftSliderLabel.setText("Left Level", dontSendNotification);
    //leftSlider.setSkewFactorFromMidPoint(0.5);
    
    rightTargetLevel = 0.5f;
    rightSlider.setRange(0.0, 1);
    rightSlider.setValue(leftSlider.getValue());
    rightSlider.setValue(rightTargetLevel, dontSendNotification);
    rightSlider.setTextBoxStyle(Slider::TextBoxRight, false, 100, 20);
    rightSlider.onValueChange = [this] {
        rightTargetLevel = rightSlider.getValue();
        rightSamplesToTarget = rightRampLengthSamples;
    };
    rightSlider.addListener(this);
    rightSliderLabel.setText("Right Level", dontSendNotification);
    //rightSlider.setSkewFactorFromMidPoint(0.5);
    
    
    masterTargetLevel = 0.0f;
    masterSlider.setRange(0.0, 1.0);
    masterSlider.setValue(masterTargetLevel, dontSendNotification);
    masterSlider.setTextBoxStyle(Slider::TextBoxRight, false, 100, 20);
    masterSlider.onValueChange = [this] {
        masterTargetLevel = masterSlider.getValue();
        masterSamplesToTarget = masterRampLengthSamples;
    };
    masterSlider.addListener(this);
    masterSliderLabel.setText("Master Level", dontSendNotification);
    
    
    
    
    addAndMakeVisible (&noiseSlider);
    addAndMakeVisible (&noiseSliderLabel);
    
    addAndMakeVisible(&leftSlider);
    addAndMakeVisible(&leftSliderLabel);
    
    addAndMakeVisible(&rightSlider);
    addAndMakeVisible(&rightSliderLabel);
    
    
    addAndMakeVisible(&masterSlider);
    addAndMakeVisible(&masterSliderLabel);
    
    
    //createWavetable();  //should now contain 128 samples of a full sine wave cycle
    
    
    
    // Make sure you set the size of the component after
    // you add any child components.
    
  
    
    setSize (600, 150);
}

MainComponent::~MainComponent() {
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}



//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate) {
    // This function will be called when the audio device is started, or when
    // its settings (i.e. sample rate, block size, etc) are changed.

    // You can use this function to initialise any resources you might need,
    // but be careful - it will be called on the audio thread, not the GUI thread.
    
    //not sure if needed
    String message;
    message << "Preparing to play audio...\n";
    message << " samplesPerBlockExpected = " << samplesPerBlockExpected << "\n";
    message << " sampleRate = " << sampleRate;
    Logger::getCurrentLogger()->writeToLog (message);
    
   
    // For more details, see the help for AudioProcessor::prepareToPlay()

    
}

void MainComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) {
    // Your audio-processing code goes here!
    
    //Noise Block
    auto noiseLevel = (float) noiseSlider.getValue();
    auto leftLevel = (float) leftSlider.getValue();
    auto rightLevel = (float) rightSlider.getValue();
    auto masterLevel = (float) masterSlider.getValue();
    
    //auto levelScale = noiseLevel * 2.0f;
    auto sample = 0;
    /*
    //Left Channel
    auto* leftBuffer = bufferToFill.buffer->getWritePointer(0, bufferToFill.startSample);
    for(sample = 0; sample < bufferToFill.numSamples; sample++) {
        leftBuffer[sample] = random.nextFloat() * leftLevel * noiseLevel;
    }
    
    //Right Channel
    auto* rightBuffer = bufferToFill.buffer->getWritePointer(1, bufferToFill.startSample);
    for(sample = 0; sample < bufferToFill.numSamples; sample++) {
        rightBuffer[sample] = random.nextFloat() * rightLevel * levelScale - noiseLevel;
    }*/
    //========================================================================================
    
    #warning TODO
    //Square
    auto f = 1.0f; //f, frequency
    auto A = 0.75f;  //A, peak amplitude
    auto phase = 0.0f;
    auto pi = 3.14159265358979323846f;
    
    auto* leftBuffer = bufferToFill.buffer->getWritePointer(0, bufferToFill.startSample);
    auto* rightBuffer = bufferToFill.buffer->getWritePointer(1, bufferToFill.startSample);
    for(sample = 0; sample < bufferToFill.numSamples; sample++) {
        
    
        if(phase<pi) {
            leftBuffer[sample] = A * leftLevel * noiseLevel * masterLevel;
            rightBuffer[sample] = A * rightLevel * noiseLevel * masterLevel;
        }
        else {
            leftBuffer[sample] = (0.0f-A) * leftLevel * noiseLevel * masterLevel;
            rightBuffer[sample] = (0.0f-A) * rightLevel  * noiseLevel * masterLevel;
        }
        
        phase = phase + ((2*pi*f) / bufferToFill.numSamples);
        
        if(phase>(2*pi)) {
            phase = phase - (2*pi);
        }
    }
    
    
    
    //Wavetable synthesis
    
    
    /*
    bufferToFill.clearActiveBufferRegion();
    mySynth.renderNextBlock(bufferToFill, mySynth.midiMessages, 0, bufferToFill.numSamples);
    */
    
    
}


void MainComponent::releaseResources() {
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    Logger::getCurrentLogger()->writeToLog ("Releasing audio resources");
    
    // For more details, see the help for AudioProcessor::releaseResources()
}

//==============================================================================
void MainComponent::paint (Graphics& g) {
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    //g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
    
    // You can add your drawing code here!
    
    Array<Colour> colours {
        Colour (25, 25, 25),    //black
        Colour (33, 33, 33),    //darkGrey
        Colour (255, 255, 255), //white
        Colour (232, 229, 229), //offWhite
        Colour (191, 191, 191), //gray
        Colour (15, 199, 149),  //blue
        Colour (63, 33, 89)     //purple
    };
    auto darkGrey = colours[1];
    auto blue = colours[5];
    auto purple = colours[6];
    
    noiseSliderLabel.setColour (Label::textColourId, blue);
    noiseSlider.setColour(Slider::thumbColourId, blue);
    noiseSlider.setColour(Slider::trackColourId, purple);
    noiseSlider.setColour(Slider::textBoxTextColourId, blue);
    noiseSlider.setColour(Slider::textBoxOutlineColourId, purple);
    
    leftSliderLabel.setColour(Label::textColourId, blue);
    leftSlider.setColour(Slider::thumbColourId, blue);
    leftSlider.setColour(Slider::trackColourId, purple);
    leftSlider.setColour(Slider::textBoxTextColourId, blue);
    leftSlider.setColour(Slider::textBoxOutlineColourId, purple);
    
    rightSliderLabel.setColour(Label::textColourId, blue);
    rightSlider.setColour(Slider::thumbColourId, blue);
    rightSlider.setColour(Slider::trackColourId, purple);
    rightSlider.setColour(Slider::textBoxTextColourId, blue);
    rightSlider.setColour(Slider::textBoxOutlineColourId, purple);
    
    
    masterSliderLabel.setColour(Label::textColourId, blue);
    masterSlider.setColour(Slider::thumbColourId, purple);
    masterSlider.setColour(Slider::trackColourId, blue);
    masterSlider.setColour(Slider::textBoxTextColourId, blue);
    masterSlider.setColour(Slider::textBoxOutlineColourId, blue);
    
    
 
    
    
    g.fillAll(darkGrey);   //Set Background
}



void MainComponent::resized(){
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
    const int border = 100;
    
    
    noiseSlider.setBounds (border, 10, getWidth() - (border+10), 20);   //x, y, width, height
    noiseSliderLabel.setBounds (10, 10, 90, 20);
    
    leftSlider.setBounds(border, 40,  getWidth() - (border+10), 20);
    leftSliderLabel.setBounds(10, 40, 90, 20);
    
    //rightSlider.setBounds(border, 70, 20, 100);
    rightSlider.setBounds(border, 70, getWidth() - (border+10), 20);
    rightSliderLabel.setBounds(10, 70, 90, 20);
 
    
    masterSlider.setBounds(border, 100, getWidth() - (border+10), 20);
    masterSliderLabel.setBounds(10, 100, 90, 20);
    
    
    /*
    //Vertical
    noiseSlider.setSliderStyle(Slider::LinearVertical);
    noiseSlider.setBounds(10, 10, 20, getHeight() - (border+10));
    noiseSliderLabel.setBounds(10, 10, 90, 20);
    
    leftSlider.setSliderStyle(Slider::LinearVertical);
    leftSlider.setBounds(40, 10,  20, getHeight() - (border+10));
    leftSliderLabel.setBounds(10, 40, 90, 20);
    
    rightSlider.setSliderStyle(Slider::LinearVertical);
    rightSlider.setBounds(70, 10, 20, getHeight() - (border+10));
    rightSliderLabel.setBounds(10, 70, 90, 20);
    */

}
void MainComponent::timerCallback() {
    Logger::getCurrentLogger()->writeToLog ("here");
}
   
