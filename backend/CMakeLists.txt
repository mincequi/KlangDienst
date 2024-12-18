CPMAddPackage(gh:gabime/spdlog@1.15.0)
CPMAddPackage(gh:Curve/rohrkabel@8.0)
CPMAddPackage(gh:nlohmann/json@3.11.3)

add_library(backend STATIC
  AudioChannel.h
  Biquad.h
  Biquad.cpp
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
  Filter.cpp
)

target_include_directories(backend
PUBLIC
  ${SPA_INCLUDE_DIRS}
  ${PIPEWIRE_INCLUDE_DIRS}
  ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(backend
PUBLIC
  rohrkabel
  spdlog::spdlog

PRIVATE
  common
  nlohmann_json::nlohmann_json
)
