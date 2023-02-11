#include "MarianaConvolution.h"
#include <BinaryData.h>
#include <SDSP/SDSP.h>
namespace Mariana 
{ 
    MarianaConvolution::MarianaConvolution(juce::AudioFormatManager& formatManager) : m_formatManager(formatManager), m_convolution(juce::dsp::Convolution::NonUniform{1024})
    { 

    }

    MarianaConvolution::~MarianaConvolution() 
    { 

    }

    void MarianaConvolution::prepareToPlay(int samplesPerBlockExpected, double sampleRate) 
    { 
        // 2 channels always??
        // load convolution with resample, pass that buffer off to the convolution..
        juce::dsp::ProcessSpec spec{sampleRate, static_cast<juce::uint32>(samplesPerBlockExpected), 2};
        //juce::AudioFormatManager& formatManager, const void* data, int size, const double sampleRate, juce::AudioBuffer<float>& destBuffer
        juce::AudioBuffer<float> tempConvBuffer;
        SDSP::Utils::loadFromMemoryWithResample(m_formatManager, BinaryData::UnderwaterConv_wav, BinaryData::UnderwaterConv_wavSize, sampleRate / 2, tempConvBuffer);
        m_convolution.prepare(spec);
        m_convolution.loadImpulseResponse(std::move(tempConvBuffer), sampleRate, juce::dsp::Convolution::Stereo::yes, juce::dsp::Convolution::Trim::no, juce::dsp::Convolution::Normalise::yes);
    }

    void MarianaConvolution::getNextAudioBlock(juce::AudioBuffer<float>& buffer) 
    { 
        juce::dsp::AudioBlock<float> block(buffer);
        juce::dsp::ProcessContextReplacing context{block};
        m_convolution.process(context);
    }

    void MarianaConvolution::releaseResources() 
    { 
        m_convolution.reset();
    }
}