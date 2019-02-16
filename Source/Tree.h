/*
  ==============================================================================

    Tree.h
    Created: 15 Feb 2019 12:03:19pm
    Author:  Noah Tigner

  ==============================================================================
*/

#pragma once

/*
struct ExampleListener  : public ValueTree::Listener
{
    ExampleListener (ValueTree v)
    : tree (v)
    {
        tree.addListener (this);
    }
    ValueTree tree;
    
    
    void valueTreePropertyChanged() {
        std::cout << "hi";
    }
    
};
 */

class Tree: public Slider::Listener {
    
public:
    
    Slider testSlider;
    
    Tree();
    ~Tree();
    
};
