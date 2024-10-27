# CPM requires 3.14
cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(KlangDienst LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(KlangDienst main.cpp)

# add dependencies
include(cmake/CPM.cmake)

CPMFindPackage(
  NAME           rohrkabel
  VERSION        4.0
  GIT_REPOSITORY "https://github.com/Curve/rohrkabel"
)

include(GNUInstallDirs)
install(TARGETS KlangDienst
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
