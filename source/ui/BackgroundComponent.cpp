//
// Created by Syl on 11/02/2023.
//

#include "BackgroundComponent.h"

namespace Mariana
{
    BackgroundComponent::BackgroundComponent(APVTS& tree) : m_tree(tree)
    {
        m_tree.addParameterListener("Depth", this);
        setInterceptsMouseClicks(false, false);
        juce::ColourGradient gradient;
        gradient.addColour(0.0f, juce::Colour(0xFF000024));
        gradient.addColour(0.5f, juce::Colour(0xFF1C364A));
        gradient.addColour(1.0f, juce::Colour(0xFF5FBFF9));

        gradient.createLookupTable(m_colours.data(), static_cast<int>(m_colours.size()));
    }

    BackgroundComponent::~BackgroundComponent() {
        m_tree.removeParameterListener("Depth", this);
    }

    void BackgroundComponent::paint(juce::Graphics& g)
    {
        g.fillAll(m_colours[m_currentIndex]);
    }

    void BackgroundComponent::resized() {

    }

    void BackgroundComponent::parameterChanged(const juce::String &id, float value) {
        if(id == "Depth"){
            // map value between 0 to n colours..
            m_currentIndex = static_cast<size_t>(juce::jmap<float>(value, 0, 11000, 0, 49));
            repaint();
        }
    }
}
