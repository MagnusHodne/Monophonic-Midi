#include <catch2/catch_test_macros.hpp>
#include "../Source/PluginProcessor.h"

TEST_CASE("one is equal to one", "[dummy]")
{
	REQUIRE(1 == 1);
}

// https://github.com/McMartin/FRUT/issues/490#issuecomment-663544272
AudioPluginAudioProcessor testPlugin;
TEST_CASE("Plugin instance name", "[name]") {
	REQUIRE(testPlugin.getName().toStdString() == "Midi Quantizer");
}
