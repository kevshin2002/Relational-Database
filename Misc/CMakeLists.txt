add_library(Misc INTERFACE)

target_sources(Misc INTERFACE
    Types/BasicTypes.hpp
    Types/Errors.hpp
    Types/keywords.hpp
)

target_include_directories(Misc INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
)
