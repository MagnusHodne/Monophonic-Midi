# 3.18 is latest bundled by VS 2019 as of Jan 2021
# 3.19 is latest bundled CLion as of 2021.1
cmake_minimum_required(VERSION 3.18)

# Change this to your project name
set(TARGET_NAME "MidiLegato")

# Set the plugin formats you'll be building here. Valid formats: AAX Unity VST AU AUv3 Standalone
set(FORMATS VST3 AUv3)

project(${TARGET_NAME} VERSION 0.0.2)

# JUCE is setup as a submodule in the /JUCE folder
# Locally, you need to run `git submodule update --init --recursive` once
# On Github Actions, it's managed by actions/checkout
add_subdirectory(JUCE)

# Check the readme at `docs/CMake API.md` in the JUCE repo for full config...
juce_add_plugin("${TARGET_NAME}"
    COMPANY_NAME "Magnus Hodne"
    DESCRIPTION "Forces incoming MIDI to be monophonic, and remembers last played notes for trills"
    VST3_CATEGORIES Fx
    NEEDS_MIDI_INPUT TRUE               # Does the plugin need midi input?
    NEEDS_MIDI_OUTPUT TRUE              # Does the plugin need midi output?
    IS_MIDI_EFFECT TRUE                 # Is this plugin a MIDI effect?
    PLUGIN_MANUFACTURER_CODE MaHo
    PLUGIN_CODE MiLe                    # A unique four-character plugin id with at least one upper-case character
    FORMATS "${FORMATS}"
    PRODUCT_NAME "MIDI Legato")         # The name of the final executable, which can differ from the target name

# C++20 please 
target_compile_features("${TARGET_NAME}" PRIVATE cxx_std_20)

# Manually list all .h and .cpp files for the plugin, don't use globs:
set(SourceFiles
    Source/PluginProcessor.h
    Source/PluginProcessor.cpp
    )

target_sources("${TARGET_NAME}" PRIVATE ${SourceFiles})

# We'll need to link to these from our plugin as well as our tests
set(JUCE_DEPENDENCIES
    juce::juce_audio_processors
    )

target_compile_definitions("${TARGET_NAME}"
    PUBLIC
    # JUCE_WEB_BROWSER and JUCE_USE_CURL would be on by default, but you might not need them.
    JUCE_WEB_BROWSER=0  # If you remove this, add `NEEDS_WEB_BROWSER TRUE` to the `juce_add_plugin` call
    JUCE_USE_CURL=0     # If you remove this, add `NEEDS_CURL TRUE` to the `juce_add_plugin` call
    JUCE_VST3_CAN_REPLACE_VST2=0)
    
target_link_libraries("${TARGET_NAME}"
    PRIVATE
    # Assets
    ${JUCE_DEPENDENCIES}
    PUBLIC
    juce::juce_recommended_config_flags
    juce::juce_recommended_lto_flags
    juce::juce_recommended_warning_flags)