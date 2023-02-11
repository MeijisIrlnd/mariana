/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

#pragma clang diagnostic push
#pragma ide diagnostic ignored "VirtualCallInCtorOrDtor"
//==============================================================================
PluginEditor::PluginEditor (PluginProcessor& p, APVTS& tree)
    : AudioProcessorEditor (&p), audioProcessor (p), m_tree(tree), m_background(m_tree), m_temperatureReadout(m_tree)
{
    setSize (400, 300);
    addAndMakeVisible(&m_background);
    m_layout.instantiateSlider(this, m_submersionSlider, m_tree, "Submersion");
    m_layout.instantiateSlider(this, m_depthSlider, m_tree, "Depth");
    m_layout.instantiateSlider(this, m_temperatureSlider, m_tree, "Temperature");
    addAndMakeVisible(&m_temperatureReadout);
    resized();
}
#pragma clang diagnostic pop


//==============================================================================
void PluginEditor::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void PluginEditor::resized()
{
    m_background.setBounds(getLocalBounds());
    auto h = getHeight() / 12;
    for(size_t i = 0; i < m_layout.uiElements.size(); i++) {
        m_layout.uiElements[i]->label.setBounds(0, h * static_cast<int>(i), getWidth() / 8, h);
        m_layout.uiElements[i]->slider.setBounds(getWidth() / 8, h * static_cast<int>(i), getWidth() - getWidth() / 8, h);
    }
    m_temperatureReadout.setBounds(getWidth() / 2 - getWidth() / 4, getHeight() / 2, getWidth() / 2, getHeight() / 2);
}
