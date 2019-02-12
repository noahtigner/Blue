/*
  ==============================================================================

    SynthSound.h
    Created: 6 Feb 2019 11:13:05pm
    Author:  Noah Tigner

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class SynthSound: public SynthesiserSound {
public:
    bool appliesToNote(int /*midiNoteNumber*/) {
        return true;
    }
    bool appliesToChannel(int /*midiChannel*/) {
        return true;
    }
    
};
