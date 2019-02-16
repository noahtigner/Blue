/*
  ==============================================================================

    SineWave.h
    Created: 13 Feb 2019 12:30:10pm
    Author:  Noah Tigner

  ==============================================================================
*/

#pragma once
#include "MainComponent.h"
#include "Tree.h"

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
                        public Slider::Listener
{
public:
    
    Slider *masterSlider;
    
    void sliderValueChanged(Slider *slider) override {
        if(slider == masterSlider) {
            masterLevel = masterSlider->getValue();
            std::cout << "hey";
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
        level = velocity * masterLevel * 0.15;
        tailOff = 0.0;
        
        auto cyclesPerSecond = MidiMessage::getMidiNoteInHertz (midiNoteNumber);
        auto cyclesPerSample = cyclesPerSecond / getSampleRate();
        
        angleDelta = cyclesPerSample * 2.0 * MathConstants<double>::pi;
    }
    
    void stopNote (float /*velocity*/, bool allowTailOff) override
    {
        if (allowTailOff)
        {
            if (tailOff == 0.0)
                tailOff = 1.0;
        }
        else
        {
            clearCurrentNote();
            angleDelta = 0.0;
        }
    }
    
    void pitchWheelMoved (int) override      {}
    void controllerMoved (int, int) override {}
    
    void renderNextBlock (AudioSampleBuffer& outputBuffer, int startSample, int numSamples) override
    {
        if (angleDelta != 0.0)
        {
            if (tailOff > 0.0) //When the key has been released the tailOff value will be greater than zero. You can see the synthesis algorithm is similar
            {
                while (--numSamples >= 0)
                {
                    auto currentSample = (float) (std::sin (currentAngle)  * level * tailOff);
                    
                    for (auto i = outputBuffer.getNumChannels(); --i >= 0;)
                        outputBuffer.addSample (i, startSample, currentSample);
                    
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
                    auto currentSample = (float) (std::sin (currentAngle) * level);
                    
                    for (auto i = outputBuffer.getNumChannels(); --i >= 0;)
                        outputBuffer.addSample (i, startSample, currentSample);
                    
                    currentAngle += angleDelta;
                    ++startSample;
                }
            }
        }
    }
    
private:
    double currentAngle = 0.0, angleDelta = 0.0, level = 0.0, tailOff = 0.0;
    float masterLevel = 0.0;
};
