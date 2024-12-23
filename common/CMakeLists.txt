CPMAddPackage(gh:Neargye/magic_enum@0.9.7)
CPMAddPackage(gh:gabime/spdlog@1.15.0)
CPMAddPackage(gh:victimsnino/ReactivePlusPlus@2.2.1)
CPMAddPackage(gh:marzer/tomlplusplus@3.4.0)
CPMAddPackage(
  NAME reflect-cpp
  GITHUB_REPOSITORY getml/reflect-cpp
  VERSION 0.16.0
  DOWNLOAD_ONLY YES
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
  ${reflect-cpp_SOURCE_DIR}/src/reflectcpp.cpp
  ${reflect-cpp_SOURCE_DIR}/src/reflectcpp_toml.cpp
)

target_include_directories(common
PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${reflect-cpp_SOURCE_DIR}/include
    /opt/homebrew/include
)

target_link_libraries(common
PUBLIC
    magic_enum::magic_enum
    #tomlplusplus
    #reflectcpp
    spdlog::spdlog
    RPP::rpp
)
