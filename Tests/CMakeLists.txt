#=======================TEST INTEGRATION========================
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
# Using pre-installed version of Catch2 doesn't seem to work (probably issue with version, as they are just about to bump to v3. Check in later!)
Include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.0.0-preview3
)

FetchContent_MakeAvailable(Catch2)

set_target_properties(Catch2 PROPERTIES FOLDER Catch2)
set_target_properties(Catch2WithMain PROPERTIES FOLDER Catch2)

set(UnitTests
    PluginBasics.cpp)

# A list of all the files that we need for our unit tests. Unless you write tests for every single class
# used in the application, it is probably better to just include the files we actually need
set(TestedFiles
    ../Source/PluginEditor.cpp
    ../Source/PluginEditor.h
    ../Source/PluginProcessor.h
    ../Source/PluginProcessor.cpp)

juce_add_console_app(MyPluginTests
    PRODUCT_NAME "MyPlugin Unit Tests")

target_compile_features(MyPluginTests PRIVATE cxx_std_20)

target_sources(MyPluginTests 
    PRIVATE 
        ${UnitTests} 
        # ${SourceFiles} # Switching to this instead of TestedFiles will include all but Main.cpp
        ${TestedFiles})

target_compile_definitions(MyPluginTests
    PRIVATE
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0)

target_link_libraries(MyPluginTests
    PRIVATE
        ${JUCE_DEPENDENCIES}
        Catch2::Catch2WithMain
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_warning_flags)

#Since we are obtaining Catch2 via fetch(), we need this list() command
# https://github.com/catchorg/Catch2/blob/devel/docs/cmake-integration.md#usage
list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(CTest)
include(Catch)
catch_discover_tests(MyPluginTests)