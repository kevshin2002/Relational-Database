add_library(Utilities INTERFACE)

target_include_directories(Utilities INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(Utilities INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/Tokenizer/Scanner.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Tokenizer/Tokenizer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Tokenizer/Tokenizer.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Chunker.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/compare.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Config.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Filters.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Filters.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FolderReader.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Helpers.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Timer.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Validator.hpp
)

target_link_libraries(Utilities INTERFACE Testing)
