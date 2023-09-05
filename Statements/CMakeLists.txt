add_library(Statements INTERFACE)

target_sources(Statements INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/Statement.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/DBStatement.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/DBQuery.hpp
   "${CMAKE_CURRENT_SOURCE_DIR}/SQL Statements/createTableStatement.hpp"
   "${CMAKE_CURRENT_SOURCE_DIR}/SQL Statements/insertTableStatement.hpp"
   "${CMAKE_CURRENT_SOURCE_DIR}/SQL Statements/selectTableStatement.hpp"
   "${CMAKE_CURRENT_SOURCE_DIR}/SQL Statements/SQLStatement.hpp"
   "${CMAKE_CURRENT_SOURCE_DIR}/SQL Statements/updateTableStatement.hpp"
)

target_include_directories(Statements INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
)

