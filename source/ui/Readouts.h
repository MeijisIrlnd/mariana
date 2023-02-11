//
// Created by Syl on 11/02/2023.
//

#pragma once
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_utils/juce_audio_utils.h>
using APVTS = juce::AudioProcessorValueTreeState;
namespace Mariana
{
    class Readout : public juce::Component, public APVTS::Listener
    {
    public:
        Readout(APVTS& tree) : m_tree(tree) {
        };
        virtual ~Readout() = default;
        virtual void fill(juce::Image& toFill, const float amt) = 0;
        virtual void paint(juce::Graphics& g) override {}
        virtual void resized() override { }
        virtual void parameterChanged(const juce::String& id, float value) override = 0;
    protected:
        APVTS& m_tree;
    };

    class TemperatureReadout : public Readout
    {
    public:
        TemperatureReadout(APVTS& tree);
        ~TemperatureReadout() override;
        void paint(juce::Graphics& g) override;
        void fill(juce::Image& toFill, const float amt) override;
        void parameterChanged(const juce::String& id, float value) override;
    private:
        juce::Image m_toRender;
        float m_fillAmt{0.0f};
    };
}
