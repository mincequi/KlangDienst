CPMAddPackage(gh:Curve/rohrkabel@8.0)
CPMAddPackage(gh:nlohmann/json@3.11.3)

add_library(backend STATIC
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

PRIVATE
  common
  nlohmann_json::nlohmann_json
)
