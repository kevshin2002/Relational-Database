add_library(Tokenizer INTERFACE)
target_include_directories(Tokenizer INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

target_sources(Tokenizer INTERFACE
    Scanner.hpp
    Tokenizer.cpp
    Tokenizer.hpp
)
