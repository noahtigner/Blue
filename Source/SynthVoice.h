/*
  ==============================================================================

    SynthVoice.h
    Created: 7 Mar 2019 1:34:22am
    Author:  Noah Tigner

  ==============================================================================
*/

#pragma once
class SynthVoice   : public SynthesiserVoice,
public Slider::Listener,
public ComboBox::Listener
{
public:
    Slider *noiseSlider;
    Slider *leftSlider;
    Slider *rightSlider;
    Slider *masterSlider;
    Slider *attackSlider;
    Slider *decaySlider;
    Slider *sustainSlider;
    Slider *releaseSlider;
    ComboBox *synthChoice;
    Slider *lpCutoffSlider;
    Slider *lpResSlider;
    Slider *hpCutoffSlider;
    Slider *hpResSlider;
    
    void sliderValueChanged(Slider *slider) override {

        if(slider == masterSlider) {
            masterLevel = masterSlider->getValue();
        }
        else if(slider == leftSlider) {
            leftLevel = leftSlider->getValue();
        }
        else if(slider == rightSlider) {
            rightLevel = rightSlider->getValue();
        }
        else if(slider == noiseSlider) {
            noiseLevel = noiseSlider->getValue();
        }
        else if(slider == attackSlider) {
            attackLevel = attackSlider->getValue();
        }
        else if(slider == decaySlider) {
            decayLevel = decaySlider->getValue();
        }
        else if(slider == sustainSlider) {
            sustainLevel = sustainSlider->getValue();
        }
        else if(slider == releaseSlider) {
            releaseLevel = releaseSlider->getValue();
        }
        else if(slider == lpCutoffSlider) {
            lpCutoff = lpCutoffSlider->getValue();
        }
        else if(slider == lpResSlider) {
            lpResonance = lpResSlider->getValue();
        }
        else if(slider == hpCutoffSlider) {
            hpCutoff = hpCutoffSlider->getValue();
        }
        else if(slider == hpResSlider) {
            hpResonance = hpResSlider->getValue();
        }
    }
    
    void comboBoxChanged(ComboBox *box) override {
        if(box == synthChoice) {
            
            switch(synthChoice->getSelectedId()) {
                case 1:
                    //wave = osc1.sinewave(frequency);
                    std::cout<<"Sine";
                    waveType = 1;
                    break;
                case 2:
                    std::cout<<"Square";
                    waveType = 2;
                    break;
                case 3:
                    std::cout<<"Saw";
                    waveType = 3;
                    break;
                case 4:
                    std::cout<<"Triangle";
                    waveType = 4;
                    break;
                case 5:
                    std::cout<<"Noise";
                    waveType = 5;
                    break;
            }
        }
    }
    
    SynthVoice() {}
    
    
    bool canPlaySound (SynthesiserSound* sound) override {
        return dynamic_cast<SynthSound*> (sound) != nullptr;
    }
    
    void startNote (int midiNoteNumber, float velocity,
                    SynthesiserSound*, int /*currentPitchWheelPosition*/) override
    {
        
        level = velocity * masterLevel * noiseLevel * 0.15;
        
        frequency = MidiMessage::getMidiNoteInHertz(midiNoteNumber);
        
        env1.trigger = 1;   //env on
        
    }
    
    void stopNote (float /*velocity*/, bool allowTailOff = true) override {
        env1.trigger = 0;
        clearCurrentNote();
    }
    
    void pitchWheelMoved (int) override      {}
    void controllerMoved (int, int) override {}
    
    void renderNextBlock (AudioSampleBuffer& outputBuffer, int startSample, int numSamples) override {
        
        env1.setAttack(attackLevel);
        env1.setDecay(decayLevel);
        env1.setSustain(sustainLevel);
        env1.setRelease(releaseLevel);
        
        numChannels = outputBuffer.getNumChannels();
        
        for(sample = 0; sample < numSamples; sample++) {
            
            //Choose a generator/oscillator
            wave = osc1.sinewave(frequency);
            switch(waveType) {
                case 1: wave = osc1.sinewave(frequency);
                    break;
                case 2: wave = osc1.square(frequency);
                    break;
                case 3: wave = osc1.sawn(frequency);
                    break;
                case 4: wave = osc1.triangle(frequency);
                    break;
                case 5: wave = osc1.noise();
                    break;
            }
            
            rawSound = env1.adsr(wave, env1.trigger);
            lowpassedSound = filter1.lores(rawSound, lpCutoff, lpResonance);
            highpassedSound = filter2.hires(lowpassedSound, hpCutoff, hpResonance);
            
            leftOut = highpassedSound * level * leftLevel;
            rightOut = highpassedSound * level * rightLevel;
            Out = highpassedSound * level-0.15;
            
            for(channel = 0; channel < numChannels; channel++) {
                if(channel == 0) {
                    outputBuffer.addSample(channel, startSample, leftOut);
                }
                else if(channel == 1) {
                    outputBuffer.addSample(channel, startSample, rightOut);
                }
                else {
                    outputBuffer.addSample(channel, startSample, Out);
                }
            }
            ++startSample;
        }
    }
    
private:
    double level = 0.0, leftOut = 0.0, rightOut = 0.0, Out = 0.0;
    double masterLevel = 0.5f, leftLevel = 0.5f, rightLevel = 0.5f, noiseLevel = 0.0f;
    double frequency;
    
    int attackLevel = 500;
    int decayLevel = 500;
    float sustainLevel = 0.8;
    int releaseLevel = 500;
    
    int waveType = 1;
    double wave;
    
    double rawSound = 0.0, lowpassedSound = 0.0, highpassedSound = 0.0;
    
    int lpCutoff = 8000;
    double lpResonance = 1.0;
    
    double hpCutoff = 100;
    double hpResonance = 1.0;
               
    int channel = 0, sample = 0;
    int numChannels = 0;
    
    maxiOsc osc1;
    maxiEnv env1;
    maxiFilter filter1;
    maxiFilter filter2;
};
