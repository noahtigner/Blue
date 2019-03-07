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


class SynthSound   : public SynthesiserSound {

    //the sound class might be where you would store data that is needed to create the sound (such as a wavetable)
public:
    SynthSound() {}
    
    bool appliesToNote    (int) override        { return true; }
    bool appliesToChannel (int) override        { return true; }
};
