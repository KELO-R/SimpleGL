cmake_minimum_required(VERSION 3.16)
project(myproject)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(${PROJECT_NAME})

#----dependencies----
add_subdirectory(dep/glfw)
target_sources(${PROJECT_NAME}
    PRIVATE dep/glad/src/glad.c
    PUBLIC
    FILE_SET glad_headers
    TYPE HEADERS
    BASE_DIRS dep/glad/include
    FILES 
	dep/glad/include/glad/glad.h
	dep/glad/include/KHR/khrplatform.h
    PUBLIC
    FILE_SET stb_headers
    TYPE HEADERS
    BASE_DIRS dep/stb
    FILES 
	dep/stb/stb_image.h
)
target_link_libraries(${PROJECT_NAME}
    PUBLIC
	glfw
)
#----end----

target_sources(${PROJECT_NAME}
    PRIVATE
	src/window.cpp
	src/engine.cpp
	src/shader.cpp
	src/shader_program.cpp
	src/mesh.cpp
    PUBLIC
    FILE_SET HEADERS
    TYPE HEADERS
    BASE_DIRS src
    FILES 
	src/window.h
	src/engine.h
	src/shader.h
	src/shader_program.h
	src/mesh.h
	src/attribute_layout.h
)

add_executable(test)

target_sources(test
    PRIVATE
	src/main.cpp
    PUBLIC
    FILE_SET HEADERS
    TYPE HEADERS
    BASE_DIRS src
    FILES 
	src/window.h
	src/engine.h
	src/shader.h
	src/shader_program.h
	src/mesh.h
	src/attribute_layout.h
)	

target_link_libraries(test
    PRIVATE
	${PROJECT_NAME}
)
