/*
 ==============================================================================
 
 SineWave.h
 Created: 13 Feb 2019 12:30:10pm
 Author:  Noah Tigner
 
 ==============================================================================
 */

#pragma once
#include "MainComponent.h"
#include "maximilian.h"


class SineWaveSound   : public SynthesiserSound
//public MainComponent
{
    //the sound class might be where you would store data that is needed to create the sound (such as a wavetable)
public:
    SineWaveSound() {}
    
    bool appliesToNote    (int) override        { return true; }
    bool appliesToChannel (int) override        { return true; }
};

//==============================================================================

class SineWaveVoice   : public SynthesiserVoice,
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
            //std::cout << "hey";
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
    }
    
    void comboBoxChanged(ComboBox *box) override{
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
                    break;
            }
        }
    }
    
    SineWaveVoice() {}
    
    
    bool canPlaySound (SynthesiserSound* sound) override
    {
        return dynamic_cast<SineWaveSound*> (sound) != nullptr;
    }
    
    void startNote (int midiNoteNumber, float velocity,
                    SynthesiserSound*, int /*currentPitchWheelPosition*/) override
    {
#warning !wavetable
        currentAngle = 0.0;
        level = velocity * masterLevel * noiseLevel * 0.15;
        tailOff = 0.0;
        
        auto cyclesPerSecond = MidiMessage::getMidiNoteInHertz (midiNoteNumber);
        auto cyclesPerSample = cyclesPerSecond / getSampleRate();
        
        angleDelta = cyclesPerSample * 2.0 * MathConstants<double>::pi;
        
        frequency = cyclesPerSecond;
        
        env1.trigger = 1;   //env on
        
    }
    
    void stopNote (float /*velocity*/, bool allowTailOff = true) override
    {
        
        env1.trigger = 0;
        
        if (tailOff == 0.0)
            tailOff = 1.0;
    
        clearCurrentNote();
        
    }
    
    void pitchWheelMoved (int) override      {}
    void controllerMoved (int, int) override {}
    
    void renderNextBlock (AudioSampleBuffer& outputBuffer, int startSample, int numSamples) override
    {
        
        //env1.setAttack(2000);   //2 seconds
        env1.setAttack(attackLevel);
        env1.setDecay(decayLevel);
        env1.setSustain(sustainLevel);
        env1.setRelease(releaseLevel);
        
        /*
        env1.setDecay(500);
        env1.setSustain(0.8);
        env1.setRelease(2000);
         */
        
        //sinewave, square, pulse(freq, duty), saw, sawn, triangle
        
        
        for(int sample = 0; sample< numSamples; sample++) {
            
            wave = osc1.sinewave(frequency);
            /*
            switch(waveType) {
                case 1: std::cout<<"Here";
                    wave = osc1.sinewave(frequency);
                    break;
                case 2: std::cout<<"There";
                    wave = osc1.square(frequency);
                    break;
            }*/
            
            //double theWave = osc1.sinewave(frequency);
            double theSound = env1.adsr(wave, env1.trigger) * level;
            double lowpass = filter1.lores(theSound, lpCutoff, lpResonance);
            double highpass = filter1.hires(lowpass, hpCutoff, hpResonance);
            
            for(int channel = 0; channel < outputBuffer.getNumChannels(); channel++) {
                outputBuffer.addSample(channel, startSample, highpass);
            }
            ++startSample;
        }
        
        /*
        if (angleDelta != 0.0)
        {
            if (tailOff > 0.0) //When the key has been released the tailOff value will be greater than zero. You can see the synthesis algorithm is similar
            {
                while (--numSamples >= 0)
                {
                    
         
                    
                    double lrSample = osc1.sinewave(440) * level * tailOff;
                    
                    outputBuffer.addSample (0, startSample, lrSample);
                    outputBuffer.addSample (1, startSample, lrSample);
                    
         
                    //auto leftSample = (float) (std::sin (currentAngle)  * level * leftLevel * tailOff);
                    //auto rightSample = (float) (std::sin (currentAngle)  * level * rightLevel * tailOff);
                    //outputBuffer.addSample (0, startSample, leftSample);
                    //outputBuffer.addSample (1, startSample, rightSample);
         
                    
                    
                    currentAngle += angleDelta;
                    ++startSample;
                    
                    tailOff *= 0.99; //simple exponential decay envelope shape
                    
                    if (tailOff <= 0.005)
                    {
                        clearCurrentNote(); //When the tailOff value is small we determine that the voice has ended. We must call the SynthesiserVoice::clearCurrentNote() function at this point so that the voice is reset and available to be reused
                        
                        angleDelta = 0.0;
                        break;
                    }
                }
            }
            else
            {
                while (--numSamples >= 0) //This loop is used for the normal state of the voice, while the key is being held down. Notice that we use the AudioSampleBuffer::addSample() function, which mixes the currentSample value with the value alread at index startSample. This is because the synthesiser will be iterating over all of the voices. It is the responsibility of each voice to mix its output with the current contents of the buffer
                {
         
                    
               
        
                   
                    currentAngle += angleDelta;
                    ++startSample;
                    
                }
            }
        
        }
        */
    }
    
private:
    double currentAngle = 0.0, angleDelta = 0.0, level = 0.0, tailOff = 0.0;
    double masterLevel = 0.5f, leftLevel = 0.5f, rightLevel = 0.5f, noiseLevel = 0.0f;
    double frequency;
    int attackLevel = 500;
    int decayLevel = 500;
    float sustainLevel = 0.8;
    int releaseLevel = 500;
    
    int waveType = 1;
    double wave;
    
    double lpCutoff = 0.0;
    double lpResonance = 0.0;
    
    double hpCutoff = 0.0;
    double hpResonance = 0.0;
    
    maxiOsc osc1;
    maxiEnv env1;
    maxiFilter filter1;
};

