add_library(Storage
    Storage.cpp
    Storage.hpp
)

target_include_directories(Storage PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(Table)
