cmake_minimum_required(VERSION 3.17)
project(MyApp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(MyApp main.cpp)

add_subdirectory(View)
add_subdirectory(Misc)
add_subdirectory(Utilities)
add_subdirectory(Statements)
add_subdirectory(Database)
add_subdirectory(Tests)
add_subdirectory(Controller)


target_link_libraries(MyApp PRIVATE
    Controller
    Database
    Misc
    Statements
    Tests
    Utilities
    View
)
