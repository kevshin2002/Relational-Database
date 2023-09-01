add_library(Database
    Database.cpp
    Database.hpp
)

target_include_directories(Database PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(Storage)
