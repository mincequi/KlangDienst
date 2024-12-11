# CPM requires 3.14
cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(KlangDienst LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(KlangDienst
  main.cpp
)

# add dependencies
include(cmake/CPM.cmake)
#add_subdirectory(backend)
add_subdirectory(common)
add_subdirectory(frontend)

target_link_libraries(KlangDienst
PRIVATE
  common
  #backend
  frontend
)

include(GNUInstallDirs)
install(TARGETS KlangDienst
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
