#include "MarianaProcessor.h"

namespace Mariana 
{ 
    MarianaProcessor::MarianaProcessor(juce::AudioFormatManager& formatManager) : m_convolution(formatManager) 
    { 

    }

    MarianaProcessor::~MarianaProcessor() 
    { 

    }

    void MarianaProcessor::prepareToPlay(int samplesPerBlockExpected, double sampleRate) 
    {
        m_sampleRate = sampleRate;
        juce::dsp::ProcessSpec spec{sampleRate, static_cast<juce::uint32>(samplesPerBlockExpected), 1};
        std::memset(m_samples.data(), 0.0f, sizeof(float) * m_samples.size());
        std::memset(m_prev.data(), 0.0f, sizeof(float) * m_prev.size());
        for(auto stage = 0; stage < 2; ++stage) {
            SDSP::RBJ::lowpass(m_lowpassCoeffs.target(stage), sampleRate, m_lowpassCutoff, m_butterworthQs[static_cast<size_t>(stage)]);
            std::memcpy(m_lowpassCoeffs.current(stage), m_lowpassCoeffs.target(stage), sizeof(double) * 6);
        }
        for(auto& lp : m_lowpasses) {
            for(auto stage = 0; stage < 2; ++stage) {
                m_lowpasses[static_cast<size_t>(stage)].setCoefficients(m_lowpassCoeffs.target(stage), stage);
            }
        }
        m_convolution.prepareToPlay(samplesPerBlockExpected, sampleRate);

        for(auto & m_delayLine : m_delayLines) {
            m_delayLine.prepare(spec);
            m_delayLine.setMaximumDelayInSamples(static_cast<int>(sampleRate));
        }
        setSpeedOfSound(coppensSpeedOfSound(m_temperature, 35.0f, m_depth));

        m_smoothedConvolutionMix.reset(sampleRate, 0.1);
        m_smoothedConvolutionMix.setCurrentAndTargetValue(m_convolutionMix);

        m_smoothedFeedback.reset(sampleRate, 0.1);
        m_smoothedFeedback.setCurrentAndTargetValue(m_feedback);

        m_smoothedDryWet.reset(sampleRate, 0.1);
        m_smoothedDryWet.setCurrentAndTargetValue(m_dryWet);
        m_hasBeenPrepared = true;
    }

    void MarianaProcessor::getNextAudioBlock(juce::AudioBuffer<float>& buffer) 
    { 
        if(m_scratchBuffer.getNumSamples() != buffer.getNumSamples() || m_scratchBuffer.getNumChannels() != buffer.getNumChannels()) { 
            m_scratchBuffer.setSize(buffer.getNumChannels(), buffer.getNumSamples(), false);
        }
        for(auto channel = 0; channel < buffer.getNumChannels(); ++channel) { 
            m_scratchBuffer.copyFrom(channel, 0, buffer, channel, 0, buffer.getNumSamples());
        }

        m_convolution.getNextAudioBlock(m_scratchBuffer);
        auto* dryRead = buffer.getArrayOfReadPointers();
        auto* read = m_scratchBuffer.getArrayOfReadPointers();
        auto* write = buffer.getArrayOfWritePointers();

        auto channelsPerChannel = m_samples.size() / static_cast<size_t>(m_scratchBuffer.getNumChannels()); 
        for(auto sample = 0; sample < m_scratchBuffer.getNumSamples(); ++sample) 
        {
            if(m_samplesUntilUpdate == 0) {
                for(auto stage = 0; stage < 2; ++stage) {
                    SDSP::RBJ::lowpass(m_lowpassCoeffs.target(stage), m_sampleRate, m_lowpassCutoff, m_butterworthQs[static_cast<size_t>(stage)]);
                }
                m_samplesUntilUpdate = m_updateRate;
            }
            interpolate();
            --m_samplesUntilUpdate;
            float feedback = m_smoothedFeedback.getNextValue();
            float dryWet = m_smoothedDryWet.getNextValue();
            float convMix = m_smoothedConvolutionMix.getNextValue();
            for(auto channel = 0; channel < m_scratchBuffer.getNumChannels(); ++channel) {
                auto currentConv = read[static_cast<size_t>(channel)][static_cast<size_t>(sample)];
                auto currentDry = dryRead[static_cast<size_t>(channel)][static_cast<size_t>(sample)];
                auto value = (currentConv * convMix) + (currentDry * (1 - convMix));
                juce::FloatVectorOperations::fill(m_samples.data() + (channelsPerChannel * channel), value, channelsPerChannel);
            }

            for(size_t channel = 0; channel < m_delayLines.size(); ++channel) { 
                float current = m_lowpasses[channel].processSample(m_delayLines[channel].popSample(0));
                m_delayLines[channel].pushSample(0, m_samples[channel] + (m_prev[channel] * feedback));
                m_samples[channel] = m_prev[channel] = current;
            }
            SDSP::Householder<float, 8>::inPlace(m_prev.data());
            for(auto channel = 0; channel < buffer.getNumChannels(); ++channel) { 
                write[channel][sample] = (m_samples[static_cast<size_t>(channel)] * dryWet) + (dryRead[channel][sample] * (1 - dryWet));
            }
        }
    }

    void MarianaProcessor::releaseResources() 
    { 
        m_convolution.releaseResources();
    }

    void MarianaProcessor::interpolate() noexcept {
        m_lowpassCoeffs.interpolate();
        for(auto& f : m_lowpasses) {
            for(auto stage = 0; stage < 2; ++stage) {
                f.setCoefficients(m_lowpassCoeffs.current(stage), stage);
            }
        }
    }
}