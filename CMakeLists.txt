cmake_minimum_required(VERSION 3.7.2)
project(Mariana VERSION 0.0.1)
find_package(JUCE CONFIG REQUIRED)
set(CMAKE_CXX_STANDARD 20)

juce_add_plugin(
    Mariana 
    PLUGIN_MANUFACTURER_CODE SMPI
    PLUGIN_CODE SM02
    FORMATS AU VST3 Standalone 
    PRODUCT_NAME "Mariana"
)
juce_add_module(${CMAKE_CURRENT_SOURCE_DIR}/Modules/SDSP)
juce_add_module(${CMAKE_CURRENT_SOURCE_DIR}/Modules/SUX)
juce_generate_juce_header(Mariana)

add_subdirectory(source)
target_sources(Mariana PRIVATE 
    ${PROJECT_SOURCE}
)

target_include_directories(Mariana PUBLIC 
${CMAKE_CURRENT_SOURCE_DIR}/include
${CMAKE_CURRENT_SOURCE_DIR}/include/Asio
${CMAKE_CURRENT_SOURCE_DIR}/source)


target_compile_definitions(Mariana PUBLIC 
JUCE_ASIO=1
JUCE_WEB_BROWSER=0
JUCE_USE_CURL=0
JUCE_VST3_CAN_REPLACE_VST2=0)

juce_add_binary_data(BinaryData SOURCES 
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/UnderwaterConv.wav
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/Thermometer.png
)
#juce_add_binary_data(SprinklerData SOURCES
# Resources/NotoSansJP-Regular.otf
# Resources/SprinklerSliderDark.png
#)
target_link_libraries(Mariana PRIVATE 
BinaryData
juce::juce_audio_utils 
juce::juce_dsp
SDSP
PUBLIC 
juce::juce_recommended_config_flags
juce::juce_recommended_lto_flags
juce::juce_recommended_warning_flags)