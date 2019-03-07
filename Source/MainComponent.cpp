/*
 ==============================================================================
 
 This file was auto-generated!
 
 ==============================================================================
 */

#include "MainComponent.h"
#include "SynthSource.h"
#include "SineWave.h"

//==============================================================================



//==============================================================================
MainComponent::MainComponent(): synthAudioSource  (keyboardState),
keyboardComponent (keyboardState, MidiKeyboardComponent::horizontalKeyboard)
{
    // specify the number of input and output channels that we want to open
    setAudioChannels (0, 2);    //no in, 2 out
    
    /*
     static Identifier myNodeType ("MyNode"); // pre-create an Identifier
     ValueTree myNode (myNodeType);           // This is a valid node, of type "MyNode"
     */
    
    
    
    
    //noiseTargetLevel = 0.125;
    noiseTargetLevel = 0.0;
    noiseSlider.setRange(0.0, 1.0, .001);
    noiseSlider.setValue(noiseTargetLevel, dontSendNotification);
    noiseSlider.setTextBoxStyle(Slider::TextBoxRight, false, 80, 20);
    noiseSlider.onValueChange = [this] {
        noiseTargetLevel = noiseSlider.getValue();
        noiseSamplesToTarget = noiseRampLengthSamples;
    };
    //noiseSliderLabel.setText("Level", dontSendNotification);
    synthAudioSource.noiseSlider = &noiseSlider;
    noiseSlider.addListener(&synthAudioSource);
    
    leftTargetLevel = 0.5f;
    leftSlider.setRange(0.0, 1, .001);
    leftSlider.setValue(leftTargetLevel, dontSendNotification);
    leftSlider.setTextBoxStyle(Slider::TextBoxRight, false, 100, 20);
    leftSlider.onValueChange = [this] {
        leftTargetLevel = leftSlider.getValue();
        leftSamplesToTarget = leftRampLengthSamples;
    };
    leftSlider.addListener(this);   //make MC a listener
    leftSliderLabel.setText("Left", dontSendNotification);
    //leftSlider.setSkewFactorFromMidPoint(0.5);
    synthAudioSource.leftSlider = &leftSlider;
    leftSlider.addListener(&synthAudioSource);
    
    rightTargetLevel = 0.5f;
    rightSlider.setRange(0.0, 1, .001);
    rightSlider.setValue(leftSlider.getValue());
    rightSlider.setValue(rightTargetLevel, dontSendNotification);
    rightSlider.setTextBoxStyle(Slider::TextBoxRight, false, 100, 20);
    rightSlider.onValueChange = [this] {
        rightTargetLevel = rightSlider.getValue();
        rightSamplesToTarget = rightRampLengthSamples;
    };
    rightSlider.addListener(this);
    rightSliderLabel.setText("Right", dontSendNotification);
    synthAudioSource.rightSlider = &rightSlider;
    rightSlider.addListener(&synthAudioSource);
    
    
    masterTargetLevel = 0.5f;
    masterSlider.setRange(0.0, 1.0, .001);
    masterSlider.setValue(masterTargetLevel, dontSendNotification);
    masterSlider.setTextBoxStyle(Slider::TextBoxRight, false, 80, 20);
    masterSlider.onValueChange = [this] {
        masterTargetLevel = masterSlider.getValue();
        masterSamplesToTarget = masterRampLengthSamples;
    };
    masterSlider.addListener(this);
    masterSliderLabel.setText("Master Level", dontSendNotification);
    synthAudioSource.masterSlider = &masterSlider;
    masterSlider.addListener(&synthAudioSource);
    
    
    //&tree::testSlider.setRange(0.0, 1.0);
    //tree->testSlider.addListener(this);
    //addAndMakeVisible(&tree->testSlider);
    
    attackSlider.setRange(0, 5000, 1);
    attackSlider.setValue(500, dontSendNotification);
    attackSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 70, 30);
    attackSlider.addListener(this);
    attackSliderLabel.setText("Attack", dontSendNotification);
    attackSlider.setTextValueSuffix("ms");
    synthAudioSource.attackSlider = &attackSlider;
    attackSlider.addListener(&synthAudioSource);
    addAndMakeVisible(&attackSlider);
    addAndMakeVisible(&attackSliderLabel);
    
    decaySlider.setRange(0, 5000, 1);
    decaySlider.setValue(500, dontSendNotification);
    decaySlider.setTextBoxStyle(Slider::TextBoxBelow, false, 70, 30);
    decaySlider.addListener(this);
    decaySliderLabel.setText("Decay", dontSendNotification);
    decaySlider.setTextValueSuffix("ms");
    synthAudioSource.decaySlider = &decaySlider;
    decaySlider.addListener(&synthAudioSource);
    addAndMakeVisible(&decaySlider);
    addAndMakeVisible(&decaySliderLabel);
    
    sustainSlider.setRange(0.0, 1.0, .01);
    sustainSlider.setValue(.8, dontSendNotification);
    sustainSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 70, 30);
    sustainSlider.addListener(this);
    sustainSliderLabel.setText("Sustain", dontSendNotification);
    //sustainSlider.setTextValueSuffix("ms");
    synthAudioSource.sustainSlider = &sustainSlider;
    sustainSlider.addListener(&synthAudioSource);
    addAndMakeVisible(&sustainSlider);
    addAndMakeVisible(&sustainSliderLabel);
    
    releaseSlider.setRange(0, 5000, 1);
    releaseSlider.setValue(500, dontSendNotification);
    releaseSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 70, 30);
    releaseSlider.addListener(this);
    releaseSliderLabel.setText("Release", dontSendNotification);
    releaseSlider.setTextValueSuffix("ms");
    synthAudioSource.releaseSlider = &releaseSlider;
    releaseSlider.addListener(&synthAudioSource);
    addAndMakeVisible(&releaseSlider);
    addAndMakeVisible(&releaseSliderLabel);
    
    lpCutoffSlider.setRange(100, 8000, 1);
    lpCutoffSlider.setValue(8000, dontSendNotification);
    lpCutoffSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 70, 30);
    lpCutoffSlider.addListener(this);
    lpCutoffLabel.setText("Cutoff", dontSendNotification);
    lpCutoffSlider.setTextValueSuffix("Hz");
    synthAudioSource.lpCutoffSlider = &lpCutoffSlider;
    lpCutoffSlider.addListener(&synthAudioSource);
    addAndMakeVisible(&lpCutoffSlider);
    addAndMakeVisible(&lpCutoffLabel);
    
    lpResSlider.setRange(1.0, 10.0, .01);
    lpResSlider.setValue(1.0, dontSendNotification);
    lpResSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 70, 30);
    lpResSlider.addListener(this);
    lpResLabel.setText("Resonance", dontSendNotification);
    synthAudioSource.lpResSlider = &lpResSlider;
    lpResSlider.addListener(&synthAudioSource);
    addAndMakeVisible(&lpResSlider);
    addAndMakeVisible(&lpResLabel);
    
    hpCutoffSlider.setRange(100, 2000, 1);
    hpCutoffSlider.setValue(100, dontSendNotification);
    hpCutoffSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 70, 30);
    hpCutoffSlider.addListener(this);
    hpCutoffLabel.setText("Cutoff", dontSendNotification);
    hpCutoffSlider.setTextValueSuffix("Hz");
    synthAudioSource.hpCutoffSlider = &hpCutoffSlider;
    hpCutoffSlider.addListener(&synthAudioSource);
    addAndMakeVisible(&hpCutoffSlider);
    addAndMakeVisible(&hpCutoffLabel);
    
    hpResSlider.setRange(1.0, 10.0, .01);
    hpResSlider.setValue(1.0, dontSendNotification);
    hpResSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 70, 30);
    hpResSlider.addListener(this);
    hpResLabel.setText("Resonance", dontSendNotification);
    synthAudioSource.hpResSlider = &hpResSlider;
    hpResSlider.addListener(&synthAudioSource);
    addAndMakeVisible(&hpResSlider);
    addAndMakeVisible(&hpResLabel);
    
    
    
    
    
    addAndMakeVisible (&noiseSlider);
    addAndMakeVisible (&noiseSliderLabel);
    
    addAndMakeVisible(&leftSlider);
    addAndMakeVisible(&leftSliderLabel);
    
    addAndMakeVisible(&rightSlider);
    addAndMakeVisible(&rightSliderLabel);
    
    
    addAndMakeVisible(&masterSlider);
    addAndMakeVisible(&masterSliderLabel);
    
    //addAndMakeVisible(synthChoiceLabel);
    synthChoice.setText("Synth");
    
    synthChoice.addItem("Sine", 1);
    synthChoice.setItemEnabled(1, true);
    synthChoice.addItem("Square", 2);
    synthChoice.setItemEnabled(2, true);
    synthChoice.addItem("Saw", 3);
    synthChoice.setItemEnabled(3, true);
    synthChoice.addItem("Triangle", 4);
    synthChoice.setItemEnabled(4, true);
    addAndMakeVisible(synthChoice);
    synthChoice.addSeparator();
    synthChoice.addItem("Noise", 5);
    synthChoice.setItemEnabled(5, true);
    
    synthChoice.onChange = [this] {synthChoiceChanged(); };
    synthChoice.setSelectedId(1);
    synthAudioSource.synthChoice = &synthChoice;
    synthChoice.addListener(&synthAudioSource);
    
    
    
    //createWavetable();  //should now contain 128 samples of a full sine wave cycle
    
    
    
    // Make sure you set the size of the component after
    // you add any child components.
    keyboardComponent.setKeyPressBaseOctave(5);
    //keyboardComponent.setOctaveForMiddleC(2);
    addAndMakeVisible (keyboardComponent);
    //addAndMakeVisible (midiInputListLabel);
    //midiInputListLabel.setText ("MIDI Input:", dontSendNotification);
    //midiInputListLabel.attachToComponent (&midiInputList, true);
    //addAndMakeVisible (midiInputList);
    //midiInputList.setTextWhenNoChoicesAvailable ("No MIDI Inputs Enabled");
    auto midiInputs = MidiInput::getDevices();
    //midiInputList.addItemList (midiInputs, 1);
    midiInputList.onChange = [this] { setMidiInput (midiInputList.getSelectedItemIndex()); };
    for (auto midiInput : midiInputs)
    {
        if (deviceManager.isMidiInputEnabled (midiInput))
        {
            setMidiInput (midiInputs.indexOf (midiInput));
            break;
        }
    }
    if (midiInputList.getSelectedId() == 0)
        setMidiInput (0);
    
    //keyboardComponent.setAvailableRange(24, 103);
    
    //setWantsKeyboardFocus(true);
    
    setSize (600, 550);
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
    
    synthAudioSource.prepareToPlay (samplesPerBlockExpected, sampleRate);
}

void MainComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) {
    // Your audio-processing code goes here!
    synthAudioSource.getNextAudioBlock(bufferToFill);
}


void MainComponent::releaseResources() {
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.
    
    Logger::getCurrentLogger()->writeToLog ("Releasing audio resources");
    
    // For more details, see the help for AudioProcessor::releaseResources()
    
    synthAudioSource.releaseResources();
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
        Colour (63, 33, 89),    //purple
        Colour (0x1000b00),     //gray
        Colour (24, 35, 52)      //trackGrey
    };
    auto black = colours[0];
    auto darkGrey = colours[1];
    //auto white = colours[2];
    auto offWhite = colours[3];
    auto blue = colours[5];
    auto purple = colours[6];
    auto def = colours[7];
    auto trackGrey = colours[8];
    
    g.fillAll(darkGrey);   //Set Background
    
    noiseSliderLabel.setColour (Label::textColourId, blue);
    noiseSlider.setColour(Slider::thumbColourId, blue);
    noiseSlider.setColour(Slider::trackColourId, purple);
    noiseSlider.setColour(Slider::textBoxTextColourId, blue);
    noiseSlider.setColour(Slider::textBoxOutlineColourId, purple);
    noiseSlider.setTextBoxStyle(Slider::TextBoxRight, false, 80, 20);
    
    leftSliderLabel.setColour(Label::textColourId, blue);
    leftSliderLabel.setJustificationType(36);
    leftSlider.setColour(Slider::thumbColourId, blue);
    leftSlider.setColour(Slider::trackColourId, purple);
    leftSlider.setColour(Slider::textBoxTextColourId, blue);
    leftSlider.setColour(Slider::textBoxOutlineColourId, purple);
    leftSlider.setTextBoxStyle(Slider::TextBoxAbove, false, 80, 20);
    leftSlider.setSliderStyle(Slider::LinearVertical);
    
    rightSliderLabel.setColour(Label::textColourId, blue);
    rightSliderLabel.setJustificationType(36);
    rightSlider.setColour(Slider::thumbColourId, blue);
    rightSlider.setColour(Slider::trackColourId, purple);
    rightSlider.setColour(Slider::textBoxTextColourId, blue);
    rightSlider.setColour(Slider::textBoxOutlineColourId, purple);
    rightSlider.setTextBoxStyle(Slider::TextBoxAbove, false, 80, 20);
    rightSlider.setSliderStyle(Slider::LinearVertical);
    
    
    masterSliderLabel.setColour(Label::textColourId, blue);
    //masterSliderLabel.setJustificationType(1);
    masterSlider.setColour(Slider::thumbColourId, purple);
    masterSlider.setColour(Slider::trackColourId, blue);
    masterSlider.setColour(Slider::textBoxTextColourId, blue);
    masterSlider.setColour(Slider::textBoxOutlineColourId, purple);
    
    synthChoiceLabel.setColour(Label::textColourId, blue);
    synthChoiceLabel.setColour(Label::outlineColourId, purple);
    synthChoice.setColour(ComboBox::backgroundColourId, def);
    synthChoice.setColour(ComboBox::textColourId, blue);
    synthChoice.setColour(ComboBox::outlineColourId, purple);
    synthChoice.setColour(ComboBox::buttonColourId, blue);
    synthChoice.setColour(ComboBox::arrowColourId, blue);
    synthChoice.setColour(ComboBox::focusedOutlineColourId, purple);
    
    keyboardComponent.setColour(MidiKeyboardComponent::whiteNoteColourId, darkGrey);
    keyboardComponent.setColour(MidiKeyboardComponent::blackNoteColourId, black);
    keyboardComponent.setColour(MidiKeyboardComponent::upDownButtonBackgroundColourId, black);
    keyboardComponent.setColour(MidiKeyboardComponent::keyDownOverlayColourId, offWhite);
    
    attackSliderLabel.setColour(Label::textColourId, blue);
    attackSliderLabel.setJustificationType(36);
    attackSlider.setColour(Slider::thumbColourId, blue);
    attackSlider.setColour(Slider::trackColourId, purple);
    attackSlider.setColour(Slider::rotarySliderFillColourId, purple);
    //attackSlider.setColour(Slider::rotarySliderOutlineColourId, trackGrey);
    attackSlider.setColour(Slider::textBoxTextColourId, blue);
    attackSlider.setColour(Slider::textBoxOutlineColourId, purple);
    attackSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 60, 20);
    attackSlider.setSliderStyle(Slider::Rotary);

    decaySliderLabel.setColour(Label::textColourId, blue);
    decaySliderLabel.setJustificationType(36);
    decaySlider.setColour(Slider::thumbColourId, blue);
    decaySlider.setColour(Slider::trackColourId, purple);
    decaySlider.setColour(Slider::rotarySliderFillColourId, purple);
    //attackSlider.setColour(Slider::rotarySliderOutlineColourId, trackGrey);
    decaySlider.setColour(Slider::textBoxTextColourId, blue);
    decaySlider.setColour(Slider::textBoxOutlineColourId, purple);
    decaySlider.setTextBoxStyle(Slider::TextBoxBelow, false, 60, 20);
    decaySlider.setSliderStyle(Slider::Rotary);
    
    sustainSliderLabel.setColour(Label::textColourId, blue);
    sustainSliderLabel.setJustificationType(36);
    sustainSlider.setColour(Slider::thumbColourId, blue);
    sustainSlider.setColour(Slider::trackColourId, purple);
    sustainSlider.setColour(Slider::rotarySliderFillColourId, purple);
    //attackSlider.setColour(Slider::rotarySliderOutlineColourId, trackGrey);
    sustainSlider.setColour(Slider::textBoxTextColourId, blue);
    sustainSlider.setColour(Slider::textBoxOutlineColourId, purple);
    sustainSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 60, 20);
    sustainSlider.setSliderStyle(Slider::Rotary);

    releaseSliderLabel.setColour(Label::textColourId, blue);
    releaseSliderLabel.setJustificationType(36);
    releaseSlider.setColour(Slider::thumbColourId, blue);
    releaseSlider.setColour(Slider::trackColourId, purple);
    releaseSlider.setColour(Slider::rotarySliderFillColourId, purple);
    //attackSlider.setColour(Slider::rotarySliderOutlineColourId, trackGrey);
    releaseSlider.setColour(Slider::textBoxTextColourId, blue);
    releaseSlider.setColour(Slider::textBoxOutlineColourId, purple);
    releaseSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 60, 20);
    releaseSlider.setSliderStyle(Slider::Rotary);
    
    lpCutoffLabel.setColour(Label::textColourId, blue);
    lpCutoffLabel.setJustificationType(36);
    lpCutoffSlider.setColour(Slider::thumbColourId, blue);
    lpCutoffSlider.setColour(Slider::trackColourId, purple);
    lpCutoffSlider.setColour(Slider::rotarySliderFillColourId, purple);
    //attackSlider.setColour(Slider::rotarySliderOutlineColourId, trackGrey);
    lpCutoffSlider.setColour(Slider::textBoxTextColourId, blue);
    lpCutoffSlider.setColour(Slider::textBoxOutlineColourId, purple);
    lpCutoffSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 60, 20);
    lpCutoffSlider.setSliderStyle(Slider::Rotary);
    
    lpResLabel.setColour(Label::textColourId, blue);
    lpResLabel.setJustificationType(36);
    lpResSlider.setColour(Slider::thumbColourId, blue);
    lpResSlider.setColour(Slider::trackColourId, purple);
    lpResSlider.setColour(Slider::rotarySliderFillColourId, purple);
    //attackSlider.setColour(Slider::rotarySliderOutlineColourId, trackGrey);
    lpResSlider.setColour(Slider::textBoxTextColourId, blue);
    lpResSlider.setColour(Slider::textBoxOutlineColourId, purple);
    lpResSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 60, 20);
    lpResSlider.setSliderStyle(Slider::Rotary);
    
    hpCutoffLabel.setColour(Label::textColourId, blue);
    hpCutoffLabel.setJustificationType(36);
    hpCutoffSlider.setColour(Slider::thumbColourId, blue);
    hpCutoffSlider.setColour(Slider::trackColourId, purple);
    hpCutoffSlider.setColour(Slider::rotarySliderFillColourId, purple);
    //attackSlider.setColour(Slider::rotarySliderOutlineColourId, trackGrey);
    hpCutoffSlider.setColour(Slider::textBoxTextColourId, blue);
    hpCutoffSlider.setColour(Slider::textBoxOutlineColourId, purple);
    hpCutoffSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 60, 20);
    hpCutoffSlider.setSliderStyle(Slider::Rotary);
    
    hpResLabel.setColour(Label::textColourId, blue);
    hpResLabel.setJustificationType(36);
    hpResSlider.setColour(Slider::thumbColourId, blue);
    hpResSlider.setColour(Slider::trackColourId, purple);
    hpResSlider.setColour(Slider::rotarySliderFillColourId, purple);
    //attackSlider.setColour(Slider::rotarySliderOutlineColourId, trackGrey);
    hpResSlider.setColour(Slider::textBoxTextColourId, blue);
    hpResSlider.setColour(Slider::textBoxOutlineColourId, purple);
    hpResSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 60, 20);
    hpResSlider.setSliderStyle(Slider::Rotary);
    
    envelope.setText("Envelope", dontSendNotification);
    envelope.setColour(Label::textColourId, darkGrey);
    envelope.setJustificationType(36);
    addAndMakeVisible(&envelope);
    
    lp.setText("Lowpass", dontSendNotification);
    lp.setColour(Label::textColourId, darkGrey);
    lp.setJustificationType(36);
    addAndMakeVisible(&lp);
    
    hp.setText("Highpass", dontSendNotification);
    hp.setColour(Label::textColourId, darkGrey);
    hp.setJustificationType(36);
    addAndMakeVisible(&hp);
    
    g.setColour(blue);  //Partitions
    g.drawLine(10, 40, getWidth()-10, 40);
    g.drawLine(10, 410, getWidth()-10, 410);
    
    g.fillRoundedRectangle(100, 50, 150, 60, 20);
    g.fillRoundedRectangle(100, 70, 400, 150, 20);

    g.fillRoundedRectangle(100, 230, 150, 60, 20);
    g.fillRoundedRectangle(100, 250, 195, 150, 20);
    
    g.fillRoundedRectangle(305, 230, 150, 60, 20);
    g.fillRoundedRectangle(305, 250, 195, 150, 20);

    
    //g.fillRoundedRectangle(100, 180, 195, 120, 20);
    //g.fillRoundedRectangle(305, 180, 195, 120, 20);
    
    /*
    g.setColour(purple);
    g.drawLine(10, 310, getWidth()-10, 40);
    g.drawLine(10, 40, getWidth()-10, 310);
    */
    
    
    g.setColour(darkGrey);
    g.fillRoundedRectangle(101, 71, 398, 148, 20);
    g.fillRoundedRectangle(101, 251, 193, 148, 20);
    g.fillRoundedRectangle(306, 251, 193, 148, 20);
    
    /*
    g.fillRoundedRectangle(101, 181, 193, 118, 20);
    g.fillRoundedRectangle(306, 181, 193, 118, 20);
     */
    
    /*
    g.setColour(offWhite);
    g.fillRect(100, 50, 100, 170);
    
    g.setColour(offWhite);
    g.fillRect(300, 50, 100, 170);
    */
}



void MainComponent::resized(){
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
    
    const int sliderJustification = 110;
    const int sliderX = sliderJustification + 40;
    const int sliderWidth = getWidth() - (sliderJustification + 10);
    const int sliderHeight = 20;
    
    const int labelJustification = 10;
    const int labelWidth = 90;
    
    int y = 10;
    
    

    synthChoice.setBounds(labelJustification, y, 80, sliderHeight);
    
    noiseSlider.setBounds(95, y, 495, sliderHeight);   //x, y, width, height
    //noiseSliderLabel.setBounds(100, y, 40, sliderHeight);
    
    //leftSlider.setBounds(sliderJustification, y+=30, sliderWidth, sliderHeight);
    //leftSliderLabel.setBounds(labelJustification, y, labelWidth, sliderHeight);
    
    leftSlider.setBounds(20, 50, 60, 340);
    leftSliderLabel.setBounds(20, 385, 60, 20);
    
    rightSlider.setBounds(getWidth()-80, 50, 60, 340);
    rightSliderLabel.setBounds(getWidth()-80, 385, 60, 20);
    
    
    //rightSlider.setBounds(sliderJustification, y+=30, sliderWidth, sliderHeight);
    //rightSliderLabel.setBounds(labelJustification, y, labelWidth, sliderHeight);

    envelope.setBounds(135, 50, 80, 20);
    
    attackSliderLabel.setBounds(100, 80, 100, sliderHeight);
    attackSlider.setBounds(100, 100, 100, 100);
    
    decaySliderLabel.setBounds(200, 80, 100, sliderHeight);
    decaySlider.setBounds(200, 100, 100, 100);
    
    lp.setBounds(135, 230, 80, 20);
    sustainSliderLabel.setBounds(300 , 80, 100, sliderHeight);
    sustainSlider.setBounds(300, 100, 100, 100);
    
    hp.setBounds(340, 230, 80, 20);
    releaseSliderLabel.setBounds(400, 80, 100, sliderHeight);
    releaseSlider.setBounds(400, 100, 100, 100);
 
    
    //===========================================
    
    
    lpCutoffLabel.setBounds(100, 260, 100, sliderHeight);
    lpCutoffSlider.setBounds(100, 280, 100, 100);
    
    lpResLabel.setBounds(200, 260, 100, sliderHeight);
    lpResSlider.setBounds(200, 280, 100, 100);
    
    hpCutoffLabel.setBounds(300, 260, 100, sliderHeight);
    hpCutoffSlider.setBounds(300, 280, 100, 100);
    
    hpResLabel.setBounds(400, 260, 100, sliderHeight);
    hpResSlider.setBounds(400, 280, 100, 100);
    /*
    lpResLabel.setBounds(sliderJustification+100, y, 100, sliderHeight);
    lpResSlider.setBounds(sliderJustification+100, y+20, 100, 100);
    
    hpCutoffLabel.setBounds(sliderJustification+200, y, 100, sliderHeight);
    hpCutoffSlider.setBounds(sliderJustification+200, y+20, 100, 100);
    
    hpResLabel.setBounds(sliderJustification+300, y, 100, sliderHeight);
    hpResSlider.setBounds(sliderJustification+300, y+20, 100, 100);
    */
    
    masterSlider.setBounds(95, 420, 495, sliderHeight);
    masterSliderLabel.setBounds(labelJustification, 419, 100, sliderHeight);
    
    keyboardComponent.setBounds(10, 450, getWidth() - 20, 90);
    
    
    
}
void MainComponent::timerCallback() {
    Logger::getCurrentLogger()->writeToLog ("here");
}

void MainComponent::synthChoiceChanged() {
    switch(synthChoice.getSelectedId()) {
        case 1:
            break;
        case 2:
            break;
        case 3:
            break;
    }
}

void MainComponent::setMidiInput (int index)
{
    auto list = MidiInput::getDevices();
    deviceManager.removeMidiInputCallback (list[lastInputIndex], synthAudioSource.getMidiCollector()); //remove the previous MidiInputCallback object for the previously selected MIDI input device if the user changes the selected device using the combo-box
    auto newInput = list[index];
    if (! deviceManager.isMidiInputEnabled (newInput))
        deviceManager.setMidiInputEnabled (newInput, true);
    deviceManager.addMidiInputCallback (newInput, synthAudioSource.getMidiCollector()); //add the MidiMessageCollector object from our SynthAudioSource object as a MidiInputCallback object for the specified MIDI input device
    midiInputList.setSelectedId (index + 1, dontSendNotification);
    lastInputIndex = index;
}


