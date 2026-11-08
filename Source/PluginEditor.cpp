#include "PluginEditor.h"

namespace {
const juce::Colour bg(0xff111315), panel(0xff1b1e20), amber(0xffd99a32), cream(0xffffedcf), muted(0xff92999a);
const std::array<const char*,8> ids {"BITS","CRUSH","DRIVE","HPF","LPF","SAT","MIX","OUTPUT"};
const std::array<const char*,8> names {"ALCOHOL / BIT DEPTH","TEMPERATURE / RATE","PRESSURE / DRIVE","HOPS / HIGH PASS","HOPS / LOW PASS","FERMENTATION","BEER MIX","FINAL POUR"};
}

BeercrusherAudioProcessorEditor::BeercrusherAudioProcessorEditor(BeercrusherAudioProcessor& p)
    : AudioProcessorEditor(&p), proc(p) {
    setSize(900,560);
    title.setText("BEERCRUSHER",juce::dontSendNotification);
    title.setFont(juce::Font(juce::FontOptions(30.0f,juce::Font::bold)));
    title.setColour(juce::Label::textColourId,cream);
    addAndMakeVisible(title);
    subtitle.setText("EXTREME DIGITAL DESTRUCTION  /  AUDIO EFFECT",juce::dontSendNotification);
    subtitle.setFont(juce::Font(juce::FontOptions(11.0f,juce::Font::bold)));
    subtitle.setColour(juce::Label::textColourId,amber);
    addAndMakeVisible(subtitle);
    for (int i=0;i<8;++i) setup(i,names[(size_t)i],ids[(size_t)i]);
}
void BeercrusherAudioProcessorEditor::setup(int i,const juce::String& caption,const juce::String& id) {
    auto& s=knobs[(size_t)i]; auto& l=labels[(size_t)i];
    l.setText(caption,juce::dontSendNotification);
    l.setFont(juce::Font(juce::FontOptions(9.5f,juce::Font::bold)));
    l.setColour(juce::Label::textColourId,muted);
    l.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(l);
    s.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    s.setTextBoxStyle(juce::Slider::TextBoxBelow,false,76,20);
    s.setColour(juce::Slider::rotarySliderFillColourId,amber);
    s.setColour(juce::Slider::rotarySliderOutlineColourId,juce::Colour(0xff414649));
    s.setColour(juce::Slider::thumbColourId,cream);
    s.setColour(juce::Slider::textBoxTextColourId,cream);
    s.setColour(juce::Slider::textBoxBackgroundColourId,panel);
    addAndMakeVisible(s);
    attachments[(size_t)i]=std::make_unique<Attachment>(proc.parameters,id,s);
}
void BeercrusherAudioProcessorEditor::paint(juce::Graphics& g) {
    g.fillAll(bg);
    g.setColour(panel); g.fillRoundedRectangle(getLocalBounds().toFloat().reduced(12),18);
    g.setColour(amber); g.fillRect(28,78,getWidth()-56,1);
    auto glass=juce::Rectangle<float>(365,142,170,235);
    juce::Path outline;
    outline.startNewSubPath(glass.getX()+30,glass.getY());
    outline.lineTo(glass.getX()+43,glass.getBottom()-12);
    outline.quadraticTo(glass.getCentreX(),glass.getBottom()+2,glass.getRight()-43,glass.getBottom()-12);
    outline.lineTo(glass.getRight()-30,glass.getY());
    juce::Path beer;
    beer.startNewSubPath(glass.getX()+32,glass.getY()+92);
    beer.lineTo(glass.getX()+45,glass.getBottom()-14);
    beer.quadraticTo(glass.getCentreX(),glass.getBottom()-4,glass.getRight()-45,glass.getBottom()-14);
    beer.lineTo(glass.getRight()-32,glass.getY()+92); beer.closeSubPath();
    g.setColour(juce::Colour(0xffc77a18)); g.fillPath(beer);
    g.setColour(cream); g.fillRoundedRectangle(glass.getX()+29,glass.getY()+80,glass.getWidth()-58,23,10);
    g.setColour(juce::Colour(0xbbeaf5f5)); g.strokePath(outline,juce::PathStrokeType(3));
    g.setColour(cream); g.setFont(juce::Font(juce::FontOptions(14,juce::Font::bold)));
    g.drawFittedText("THE CRUSHER",350,390,200,24,juce::Justification::centred,1);
    g.setColour(muted); g.setFont(juce::Font(juce::FontOptions(10)));
    g.drawFittedText("DIGITAL BREWING SYSTEM",350,414,200,18,juce::Justification::centred,1);
    g.setColour(amber); g.drawFittedText("CRUSH YOUR SOUND.",28,getHeight()-28,getWidth()-56,14,juce::Justification::centred,1);
}
void BeercrusherAudioProcessorEditor::resized() {
    title.setBounds(30,20,360,42); subtitle.setBounds(32,55,500,18);
    const int xs[4]={34,179,620,765}; const int ys[2]={112,278};
    for (int i=0;i<8;++i) {
        const int col=i%4, row=i/4, x=xs[col], y=ys[row];
        labels[(size_t)i].setBounds(x-8,y,128,22);
        knobs[(size_t)i].setBounds(x,y+20,112,122);
    }
}
