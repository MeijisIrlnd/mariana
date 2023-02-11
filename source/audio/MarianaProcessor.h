#pragma once
#include <SDSP/SDSP.h>
#include <audio/MarianaConvolution.h>
#include <audio/Diffuser.h>
namespace Mariana 
{ 
    class MarianaProcessor 
    { 
    public: 
        explicit MarianaProcessor(juce::AudioFormatManager& formatManager);
        ~MarianaProcessor();
        void prepareToPlay(int samplesPerBlockExpected, double sampleRate);
        void getNextAudioBlock(juce::AudioBuffer<float>& buffer);
        void releaseResources();

        SDSP_INLINE void setConvolutionMix(const float newMix) noexcept {
            m_convolutionMix = newMix;
            if(m_hasBeenPrepared) {
                m_smoothedConvolutionMix.setTargetValue(newMix);
            }
        }

        SDSP_INLINE void setFeedback(const float newFeedback) noexcept { 
            m_feedback = newFeedback;
            if(m_hasBeenPrepared) { 
                m_smoothedFeedback.setTargetValue(m_feedback);
            }
        }
        SDSP_INLINE void setTemperature(const float newTemperature) noexcept {
            m_temperature = newTemperature;
            if(m_hasBeenPrepared) {
                setSpeedOfSound(coppensSpeedOfSound(m_temperature, 35, m_depth));
            }
        }

        SDSP_INLINE void setDepth(const float newDepth) noexcept {
            m_depth = newDepth;
            // depth 1 - lowpass 1k
            m_lowpassCutoff = juce::jmap<float>((11000 - m_depth), 0, 11000, 2000.0f, 20000.0f);
            if(m_hasBeenPrepared) {
                setSpeedOfSound(coppensSpeedOfSound(m_temperature, 35, m_depth));
            }
        }

        SDSP_INLINE void setDryWet(const float newDryWet) noexcept { 
            m_dryWet = newDryWet;
            if(m_hasBeenPrepared) { 
                m_smoothedDryWet.setTargetValue(m_dryWet);
            }
        }

    private:
        void interpolate() noexcept;
        SDSP_INLINE float coppensSpeedOfSound(const float temperature, const float salinity, const float depth) {
            auto d = depth / 1000.0f;
            auto t = temperature / 10.0f;
            auto c_0st = 1449.05f + (45.7f * t) - (5.21f * std::powf(t, 2)) + (0.23f * std::powf(t, 3)) + (1.333f - (0.126f * t) + (0.009f * std::powf(t, 2)) * (salinity - 35));
            auto c_dst = c_0st + (16.23f + (0.253f * t)) * d + (0.213f - (0.1f * t)) * std::powf(d, 2) + (0.016f + (0.0002f * salinity - 35)) * (salinity - 35) * (t * d);
            return c_dst;
        }

        SDSP_INLINE void setSpeedOfSound(float newSpeedOfSound) noexcept {
            for(auto channel = 0; channel < m_delayLines.size(); channel++) {
                //float currentTime = (m_roomSize / 343.0f) * std::powf(2, i / 8.0f);
                m_delayLines[channel].setDelay(static_cast<float>(120.0f / newSpeedOfSound) * std::powf(2, static_cast<float>(channel) / 8.0f) * m_sampleRate);
            }
        }

        double m_sampleRate{44100};
        bool m_hasBeenPrepared{ false };
        const int m_updateRate{ 100 };
        int m_samplesUntilUpdate{ 0 };
        const std::array<double, 2> m_butterworthQs = SDSP::getButterworthQs<4>();
        juce::AudioBuffer<float> m_scratchBuffer;
        MarianaConvolution m_convolution;
        std::array<float, 8> m_samples, m_prev;
        std::array<juce::dsp::DelayLine<float, juce::dsp::DelayLineInterpolationTypes::Thiran>, 8> m_delayLines;

        SDSP::SmoothedFilterCoefficients<2> m_lowpassCoeffs;
        std::array<SDSP::BiquadCascade<2>, 8> m_lowpasses;


        float m_temperature{20.0f}, m_depth{0.0f};
        float m_convolutionMix{1.0f};
        juce::SmoothedValue<float> m_smoothedConvolutionMix;
        float m_feedback{0.7f};
        juce::SmoothedValue<float> m_smoothedFeedback;

        float m_dryWet{1.0f};
        juce::SmoothedValue<float> m_smoothedDryWet;

        float m_lowpassCutoff{ 20000.0f };
        
    };
}