#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"

class BeercrusherAudioProcessorEditor final : public juce::AudioProcessorEditor {
public:
    explicit BeercrusherAudioProcessorEditor(BeercrusherAudioProcessor&);
    void paint(juce::Graphics&) override;
    void resized() override;
private:
    using Attachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    BeercrusherAudioProcessor& proc;
    juce::Label title, subtitle;
    std::array<juce::Slider,8> knobs;
    std::array<juce::Label,8> labels;
    std::array<std::unique_ptr<Attachment>,8> attachments;
    void setup(int index, const juce::String& caption, const juce::String& id);
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BeercrusherAudioProcessorEditor)
};
