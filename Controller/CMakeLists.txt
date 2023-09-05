add_library(Controller
    AppController.cpp
    AppController.hpp
)

target_include_directories(Controller PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_library(AppProcessor INTERFACE
    Processors/AppProcessor.hpp
)

add_library(DBProcessor
    Processors/DBProcessor.cpp
    Processors/DBProcessor.hpp
)

add_library(SQLProcessor
    Processors/SQLProcessor.cpp
    Processors/SQLProcessor.hpp
)

target_link_libraries(Controller PRIVATE AppProcessor)
