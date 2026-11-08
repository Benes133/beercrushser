#include "PluginProcessor.h"
#include "PluginEditor.h"

namespace {
float coeff(float hz, double sr) {
    const float f = juce::jlimit(10.0f, static_cast<float>(sr * 0.45), hz);
    return 1.0f - std::exp(-juce::MathConstants<float>::twoPi * f / static_cast<float>(sr));
}
}

BeercrusherAudioProcessor::BeercrusherAudioProcessor()
    : AudioProcessor(BusesProperties()
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)),
      parameters(*this, nullptr, "BEERCRUSHER_STATE", createParameterLayout()) {}

juce::AudioProcessorValueTreeState::ParameterLayout BeercrusherAudioProcessor::createParameterLayout() {
    using APF = juce::AudioParameterFloat;
    using API = juce::AudioParameterInt;
    juce::AudioProcessorValueTreeState::ParameterLayout p;
    p.add(std::make_unique<API>("BITS", "Alcohol / Bit Depth", 1, 24, 8));
    p.add(std::make_unique<APF>("CRUSH", "Temperature / Rate Reduction", juce::NormalisableRange<float>(1,64,1), 12));
    p.add(std::make_unique<APF>("DRIVE", "Pressure / Drive", juce::NormalisableRange<float>(0,24,0.01f), 6, " dB"));
    p.add(std::make_unique<APF>("HPF", "Hops HPF", juce::NormalisableRange<float>(20,2000,1,0.35f), 20, " Hz"));
    p.add(std::make_unique<APF>("LPF", "Hops LPF", juce::NormalisableRange<float>(1000,20000,1,0.35f), 18000, " Hz"));
    p.add(std::make_unique<APF>("SAT", "Fermentation / Saturation", juce::NormalisableRange<float>(0,100,0.1f), 15, " %"));
    p.add(std::make_unique<APF>("MIX", "Beer Mix", juce::NormalisableRange<float>(0,100,0.1f), 100, " %"));
    p.add(std::make_unique<APF>("OUTPUT", "Final Pour / Output", juce::NormalisableRange<float>(-24,12,0.01f), 0, " dB"));
    return p;
}

void BeercrusherAudioProcessor::prepareToPlay(double sampleRate, int) {
    sr = sampleRate > 0 ? sampleRate : 44100.0;
    held.fill(0); lpState.fill(0); hpX.fill(0); hpY.fill(0); holdCounter = 0;
}

bool BeercrusherAudioProcessor::isBusesLayoutSupported(const BusesLayout& l) const {
    const auto in = l.getMainInputChannelSet(), out = l.getMainOutputChannelSet();
    return in == out && (out == juce::AudioChannelSet::mono() || out == juce::AudioChannelSet::stereo());
}

void BeercrusherAudioProcessor::processBlock(juce::AudioBuffer<float>& b, juce::MidiBuffer&) {
    juce::ScopedNoDenormals noDenormals;
    const int chs = juce::jmin(2, getTotalNumInputChannels(), b.getNumChannels());
    const int n = b.getNumSamples();
    for (int ch = getTotalNumInputChannels(); ch < getTotalNumOutputChannels(); ++ch) b.clear(ch,0,n);

    const int bits = juce::jlimit(1,24,(int)parameters.getRawParameterValue("BITS")->load());
    const int factor = juce::jlimit(1,64,(int)std::round(parameters.getRawParameterValue("CRUSH")->load()));
    const float drive = juce::Decibels::decibelsToGain(parameters.getRawParameterValue("DRIVE")->load());
    const float hpA = coeff(parameters.getRawParameterValue("HPF")->load(),sr);
    const float lpA = coeff(parameters.getRawParameterValue("LPF")->load(),sr);
    const float sat = parameters.getRawParameterValue("SAT")->load() * 0.01f;
    const float mix = parameters.getRawParameterValue("MIX")->load() * 0.01f;
    const float outGain = juce::Decibels::decibelsToGain(parameters.getRawParameterValue("OUTPUT")->load());
    const float levels = (float)juce::jmax(1, (1 << (bits - 1)) - 1);

    for (int i=0;i<n;++i) {
        if (holdCounter == 0) {
            for (int ch=0;ch<chs;++ch) {
                float x = b.getSample(ch,i) * drive;
                if (sat > 0.0001f) {
                    const float amount = 1.0f + sat * 12.0f;
                    x = std::tanh(x * amount) / std::tanh(amount);
                }
                x = std::round(juce::jlimit(-1.0f,1.0f,x) * levels) / levels;
                const auto c = (size_t)ch;
                const float high = x - hpX[c] + (1.0f-hpA)*hpY[c];
                hpX[c] = x; hpY[c] = high;
                lpState[c] += lpA * (high-lpState[c]);
                held[c] = lpState[c];
            }
        }
        for (int ch=0;ch<chs;++ch) {
            const float dry = b.getSample(ch,i);
            const float wet = held[(size_t)ch];
            b.setSample(ch,i,juce::jlimit(-8.0f,8.0f,(dry*(1.0f-mix)+wet*mix)*outGain));
        }
        holdCounter = (holdCounter+1)%factor;
    }
}

juce::AudioProcessorEditor* BeercrusherAudioProcessor::createEditor() { return new BeercrusherAudioProcessorEditor(*this); }

void BeercrusherAudioProcessor::getStateInformation(juce::MemoryBlock& dest) {
    auto state = parameters.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml,dest);
}
void BeercrusherAudioProcessor::setStateInformation(const void* data,int size) {
    std::unique_ptr<juce::XmlElement> xml(getXmlFromBinary(data,size));
    if (xml && xml->hasTagName(parameters.state.getType())) parameters.replaceState(juce::ValueTree::fromXml(*xml));
}
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() { return new BeercrusherAudioProcessor(); }
