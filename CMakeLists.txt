# CPM requires 3.14
cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(KlangDienst LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(KlangDienst
  src/main.cpp
  src/AudioChannel.h
  src/KlangDienstDsp.h
  src/KlangDienstDsp.cpp
  src/KlangDienstSink.h
  src/MediaClass.h
  src/MediaRole.h
  src/MetadataName.h
  src/Util.h
  src/registries/KlangDienstRegistry.h
  src/registries/MetadataRegistry.h
  src/registries/NodeRegistry.h
  src/registries/PortRegistry.h
  src/registries/Registry.h
  src/usecases/BuildGraphUseCase.h
  src/usecases/LinkNodesUseCase.h
  src/Filter.h
  src/Biquad.h
  src/Biquad.cpp
  src/FilterParams.h
  src/Filter.cpp
)

# add dependencies
include(cmake/CPM.cmake)
add_subdirectory(deps)

target_include_directories(KlangDienst
PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/src
  ${PIPEWIRE_INCLUDE_DIRS}
  ${SPA_INCLUDE_DIRS}
)

target_link_libraries(KlangDienst
PRIVATE
  rohrkabel
  magic_enum
  spdlog::spdlog
  RPP::rpp
  nlohmann_json::nlohmann_json
)

include(GNUInstallDirs)
install(TARGETS KlangDienst
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
