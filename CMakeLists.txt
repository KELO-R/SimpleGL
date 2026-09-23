cmake_minimum_required(VERSION 3.16)
project(myproject)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(${PROJECT_NAME})

#----dependencies----
add_subdirectory(dep/glfw)
add_subdirectory(dep/glm)
target_link_libraries(${PROJECT_NAME}
    PUBLIC
	glfw
	glm::glm-header-only
)
target_sources(${PROJECT_NAME}
    PRIVATE 
	dep/glad/src/glad.c
	dep/stb/stb_image.c 
	dep/miniaudio/miniaudio.c
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

    PUBLIC
    FILE_SET miniaudio_headers
    TYPE HEADERS
    BASE_DIRS dep/miniaudio
    FILES
	dep/miniaudio/miniaudio.h
)
#----end----

target_sources(${PROJECT_NAME}
    PRIVATE
	src/window.cpp
	src/engine.cpp
	src/shader.cpp
	src/shader_program.cpp
	src/mesh.cpp
	src/image.cpp
	src/texture.cpp
	src/audio/audio.cpp
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
	src/image.h
	src/texture.h
	src/audio/audio.h
	src/draw_objects/draw_object.h
)

#examples
add_executable(image_with_sound)
target_sources(image_with_sound
    PRIVATE
	examples/image_with_sound/main.cpp
)	
target_link_libraries(image_with_sound
    PRIVATE
	${PROJECT_NAME}
)
