add_library(Table
    Attribute.cpp
    Attribute.hpp
    Row.cpp
    Row.hpp
    Schema.hpp
    Schema.cpp
    Table.hpp
)

add_subdirectory(BlockIO)

target_include_directories(Table PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})