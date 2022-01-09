#include "PluginProcessor.h"

//==============================================================================
// This creates new instances of the plugin... it has to be defined in a
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MidiPluginProcessor();
}
