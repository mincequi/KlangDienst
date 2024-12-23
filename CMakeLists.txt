# CPM requires 3.14
cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(KlangDienst VERSION 0.1.0 LANGUAGES CXX)

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

# Create the systemd service file
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/debian/KlangDienst.service.in
    ${CMAKE_CURRENT_BINARY_DIR}/KlangDienst.service
    @ONLY
)

# Install the systemd service file
install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/KlangDienst.service
    DESTINATION /etc/systemd/system
)


# Add CPack rules for debian. It shall run as systemd service as user KlangDienst.
set(CPACK_GENERATOR "DEB")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Manuel Weichselbaumer <mincequi@web.de>")
#set(CPACK_DEBIAN_PACKAGE_NAME "klangdienst")
#set(CPACK_DEBIAN_PACKAGE_VERSION "0.1")
#set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64")
# Update CPack configuration for Debian package
set(CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA
    "${CMAKE_CURRENT_SOURCE_DIR}/debian/postinst;${PROJECT_SOURCE_DIR}/debian/postrm")

include(CPack)
