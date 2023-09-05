add_library(View INTERFACE)

target_sources(View INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/Views/StringView.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Views/View.hpp
)

target_include_directories(View INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Views>
    $<INSTALL_INTERFACE:include>
)
