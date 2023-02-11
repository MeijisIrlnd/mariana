#pragma once 
#include <juce_dsp/juce_dsp.h>

namespace Mariana 
{ 
    class MarianaConvolution 
    { 
    public: 
        explicit MarianaConvolution(juce::AudioFormatManager& formatManager);
        ~MarianaConvolution();
        void prepareToPlay(int samplesPerBlockExpected, double sampleRate);
        void getNextAudioBlock(juce::AudioBuffer<float>& buffer);
        void releaseResources();
    private: 
        
        juce::dsp::Convolution m_convolution;
        juce::AudioFormatManager& m_formatManager;
    };
}