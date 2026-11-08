cmake_minimum_required(VERSION 3.22)
project(BEERCRUSHER VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

include(FetchContent)
FetchContent_Declare(JUCE
    GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
    GIT_TAG 8.0.6
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(JUCE)

juce_add_plugin(BEERCRUSHER
    COMPANY_NAME "Beercrusher Audio"
    BUNDLE_ID "audio.beercrusher.plugin"
    PLUGIN_MANUFACTURER_CODE Bcrs
    PLUGIN_CODE Bcr1
    FORMATS VST3 Standalone
    PRODUCT_NAME "BEERCRUSHER"
    DESCRIPTION "Extreme beer-themed bitcrusher and digital destruction effect"
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    COPY_PLUGIN_AFTER_BUILD FALSE
)

target_sources(BEERCRUSHER PRIVATE
    Source/PluginProcessor.cpp
    Source/PluginEditor.cpp
)
target_compile_definitions(BEERCRUSHER PUBLIC
    JUCE_WEB_BROWSER=0 JUCE_USE_CURL=0 JUCE_VST3_CAN_REPLACE_VST2=0
)
target_link_libraries(BEERCRUSHER PRIVATE
    juce::juce_audio_utils juce::juce_dsp
    PUBLIC juce::juce_recommended_config_flags
           juce::juce_recommended_lto_flags
           juce::juce_recommended_warning_flags
)
