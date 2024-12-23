CPMAddPackage(gh:Neargye/magic_enum@0.9.7)
CPMAddPackage(gh:gabime/spdlog@1.15.0)
CPMAddPackage(gh:victimsnino/ReactivePlusPlus@2.2.1)

CPMAddPackage(
  NAME reflect-cpp
  GITHUB_REPOSITORY getml/reflect-cpp
  VERSION 0.16.0
  OPTIONS
    "REFLECTCPP_JSON OFF"
    "REFLECTCPP_TOML ON"
)

add_library(common STATIC
  AudioChannel.h
  Biquad.h
  Biquad.cpp
  Dsp.h
  Filter.h
  Filter.cpp
  FilterParams.h
  FilterParams.cpp
  FilterType.h
  FrequencyTable.h
  FrequencyTable.cpp
  DspStats.h
  Persistence.h
  Persistence.cpp
)

target_include_directories(common
PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    /opt/homebrew/include
)

target_link_libraries(common
PUBLIC
    magic_enum::magic_enum
    reflectcpp
    spdlog::spdlog
    RPP::rpp
)
