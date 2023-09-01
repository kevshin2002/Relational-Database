add_library(Tests INTERFACE)

target_sources(Tests INTERFACE
    Scripts/ScriptEngine.hpp
    Scripts/ScriptRunner.hpp
    "Test Utilities/Faked.cpp"
    "Test Utilities/Faked.hpp"
)

add_subdirectory(Testing)

target_include_directories(Tests INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)
