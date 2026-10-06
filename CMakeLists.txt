cmake_minimum_required(VERSION 3.20)
project(SIMPLE_GL)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(SIMPLE_GL_BUILD_WITH_DRAW_OBJECTS
    "Builds simple_engine library with draw_objects 
    functionality and its dependencies" ON)

option(SIMPLE_GL_USE_STB_IMAGE
    "Builds simple_engine library with stb_image dependency,
    adding helper functions to Image class" ON)
add_subdirectory(src)

option(SIMPLE_GL_BUILD_EXAMPLES "Build simple_engine examples" OFF)
if(SIMPLE_GL_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
