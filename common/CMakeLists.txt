CPMAddPackage(gh:Neargye/magic_enum@0.9.7)
CPMAddPackage(gh:gabime/spdlog@1.15.0)
CPMAddPackage(gh:victimsnino/ReactivePlusPlus@2.2.0)

add_library(common STATIC
  AudioChannel.h
  Biquad.h
  Biquad.cpp
  Filter.h
  Filter.cpp
  FilterParams.h
  FilterParams.cpp
  FilterType.h
  FrequencyTable.h
  FrequencyTable.cpp
  DspStats.h
)

target_include_directories(common
PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(common
PUBLIC
    magic_enum::magic_enum
    spdlog::spdlog
    RPP::rpp
)
