/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include "PluginProcessor.h"
#include <SUX/SUX.h>
#include <ui/BackgroundComponent.h>
#include <ui/Readouts.h>
//==============================================================================
/**
*/
class PluginEditor  : public juce::AudioProcessorEditor
{
public:
    PluginEditor (PluginProcessor&, APVTS& tree);
    ~PluginEditor() override = default;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    PluginProcessor& audioProcessor;
    APVTS& m_tree;
    SUX::Quickstart::LayoutQuickstart m_layout;
    SUX::Quickstart::QuickSlider m_submersionSlider, m_depthSlider, m_temperatureSlider, m_dryWetSlider;
    Mariana::BackgroundComponent m_background;
    Mariana::TemperatureReadout m_temperatureReadout;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginEditor)
};
