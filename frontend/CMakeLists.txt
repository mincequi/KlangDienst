CPMAddPackage("gh:vector-of-bool/cmrc#952ffdd")
CPMAddPackage(
  NAME Drogon
  GITHUB_REPOSITORY drogonframework/drogon
  VERSION 1.9.8
  OPTIONS
    "BUILD_CTL OFF"
    "BUILD_EXAMPLES OFF"
    "BUILD_ORM OFF"
    "BUILD_BROTLI OFF"
    "BUILD_YAML_CONFIG OFF"
    "BUILD_TESTS OFF"
    "BUILD_TESTING OFF"
    "TRANTOR_USE_TLS none"
)

add_subdirectory(webapp)

add_library(frontend STATIC
  WebServer.h
  WebServer.cpp
  WebSocketHandler.h
  WebSocketHandler.cpp
)

target_compile_definitions(frontend
PRIVATE
  -DCMRC_NO_EXCEPTIONS
)

target_include_directories(frontend
PUBLIC
  ${cmakerc_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(frontend
PUBLIC
  common
  drogon
  webapp
)
