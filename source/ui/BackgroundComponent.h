//
// Created by Syl on 11/02/2023.
//
#pragma once
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_utils/juce_audio_utils.h>
using APVTS = juce::AudioProcessorValueTreeState;
namespace Mariana
{
class BackgroundComponent : public juce::Component, public APVTS::Listener
    {
    public:
        BackgroundComponent(APVTS& tree);
        ~BackgroundComponent() override;
        void paint(juce::Graphics& g) override;
        void resized() override;
        void parameterChanged(const juce::String& id, float value) override;
    private:
        APVTS& m_tree;
        std::array<juce::PixelARGB, 50> m_colours;
        size_t m_currentIndex = 0.0f;
    };
}
