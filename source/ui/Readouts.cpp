//
// Created by Syl on 11/02/2023.
//

#include "Readouts.h"
#include <BinaryData.h>
namespace Mariana
{
    TemperatureReadout::TemperatureReadout(APVTS &tree) : Readout(tree) {
        m_tree.addParameterListener("Temperature", this);
        m_toRender = juce::ImageCache::getFromMemory(BinaryData::Thermometer_png, BinaryData::Thermometer_pngSize);
    }

    TemperatureReadout::~TemperatureReadout() {
        m_tree.removeParameterListener("Temperature", this);
    }

    void TemperatureReadout::paint(juce::Graphics &g) {
        auto rescaled = m_toRender.rescaled(getWidth(), getHeight());
        fill(rescaled, m_fillAmt);
        g.drawImage(rescaled, getLocalBounds().toFloat(), juce::RectanglePlacement::centred, false);
    }

    void TemperatureReadout::fill(juce::Image& toFill, const float amt) {
        auto pixelData = juce::Image::BitmapData{toFill, 0, 0, toFill.getWidth(), toFill.getHeight(), juce::Image::BitmapData::ReadWriteMode::readWrite};
        // Can we deal with the bounds as if it's 16x16?
        auto blockW = toFill.getWidth() / 16;
        auto blockH = toFill.getHeight() / 16;
        // so we know the starting block on x is 8 pixels in, pixels 8 and 9
        auto centre = toFill.getWidth() / 2.0f;
        auto startingPixelX = blockW * 7;
        auto endingPixelX = blockW * 10;
        auto startingPixelY = getHeight() - blockH;
        // right, now the height.
        auto nBlocksHigh = static_cast<int>(amt * 14);
        //cool, SO..
        for(auto x = startingPixelX; x < endingPixelX; ++x) {
            // okay so how high?
            for(auto y = startingPixelY; y < startingPixelY + (nBlocksHigh * blockH); ++y) {
                pixelData.setPixelColour(x, y, juce::Colour(0xFFFF0000));
            }
        }

    }

    void TemperatureReadout::parameterChanged(const juce::String &id, float value) {
        float normalised = juce::jmap<float>(value, 0.0f, 40.0f, 0.0f, 1.0f);
        m_fillAmt = normalised;
        repaint();
    }
}