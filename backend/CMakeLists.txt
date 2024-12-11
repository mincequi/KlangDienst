CPMAddPackage(gh:gabime/spdlog@1.15.0)
CPMAddPackage(gh:Curve/rohrkabel@8.0)
CPMAddPackage(gh:victimsnino/ReactivePlusPlus@2.2.0)
CPMAddPackage(gh:nlohmann/json@3.11.3)

add_library(backend STATIC
  main.cpp
  AudioChannel.h
  KlangDienstDsp.h
  KlangDienstDsp.cpp
  KlangDienstSink.h
  MediaClass.h
  MediaRole.h
  MetadataName.h
  Util.h
  registries/KlangDienstRegistry.h
  registries/MetadataRegistry.h
  registries/NodeRegistry.h
  registries/PortRegistry.h
  registries/Registry.h
  usecases/BuildGraphUseCase.h
  usecases/LinkNodesUseCase.h
  Filter.h
  Biquad.h
  Biquad.cpp
  FilterParams.h
  Filter.cpp
)

target_include_directories(backend
PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${PIPEWIRE_INCLUDE_DIRS}
  ${SPA_INCLUDE_DIRS}
)

target_link_libraries(backend
PRIVATE
  rohrkabel
  spdlog::spdlog
  RPP::rpp
  nlohmann_json::nlohmann_json
)
