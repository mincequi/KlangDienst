# CPM requires 3.14
cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(KlangDienst LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES)
set(LIBRARIES
  common
  frontend
)

# Add platform-specific source files
if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    list(APPEND SOURCES main.cpp)
    list(APPEND LIBRARIES backend)
else()
    list(APPEND SOURCES main_osx.cpp)
endif()

add_executable(KlangDienst
  ${SOURCES}
)

# add dependencies
include(cmake/CPM.cmake)

add_subdirectory(common)
add_subdirectory(frontend)
if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
  add_subdirectory(backend)
endif()

target_link_libraries(KlangDienst
PRIVATE
  ${LIBRARIES}
)

include(GNUInstallDirs)
install(TARGETS KlangDienst
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
