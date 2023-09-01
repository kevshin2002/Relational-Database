add_library(Testing INTERFACE)

target_include_directories(Testing INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_sources(Testing INTERFACE
    TestAutomatic.hpp
    TestManually.hpp
    TestSequencer.hpp
)
