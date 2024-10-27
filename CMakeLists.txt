# CPM requires 3.14
cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(KlangDienst LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(KlangDienst
  main.cpp
  EqualizerNode.h
  EqualizerNode.cpp
  Repository.h
  MediaClass.h
  MediaRepository.h
  MetadataName.h
  MetadataRepository.h
  BuildGraph.h
  KlangDienstSink.h
)

# add dependencies
include(cmake/CPM.cmake)
add_subdirectory(deps)

target_include_directories(KlangDienst
PRIVATE
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
