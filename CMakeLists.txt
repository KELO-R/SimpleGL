cmake_minimum_required(VERSION 3.20)
project(SIMPLE_ENGINE)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(SIMPLE_ENGINE_BUILD_WITH_AUDIO 
    "Builds simple_engine library with audio 
    functionality and its dependencies" ON)

option(SIMPLE_ENGINE_BUILD_WITH_DRAW_OBJECTS 
    "Builds simple_engine library with draw_objects 
    functionality and its dependencies" ON)

add_subdirectory(src)

option(SIMPLE_ENGINE_BUILD_EXAMPLES "Build simple_engine examples" ON)
if(SIMPLE_ENGINE_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
