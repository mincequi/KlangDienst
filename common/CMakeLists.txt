CPMAddPackage(gh:Neargye/magic_enum@0.9.7)

add_library(common STATIC
  FilterParams.h
  FilterParams.cpp
  FilterType.h
  FrequencyTable.h
  FrequencyTable.cpp
)

target_include_directories(common
PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/
)

target_link_libraries(common
PUBLIC
  magic_enum::magic_enum
)
