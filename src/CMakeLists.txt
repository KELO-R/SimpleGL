cmake_minimum_required(VERSION 3.20)
project(SIMPLE_ENGINE_LIB)

add_library(simple_engine)

#main dependencies
set(DEPENDENCY_SOURCE_DIR "${SIMPLE_ENGINE_SOURCE_DIR}/dep")
set(DEPENDENCY_BINARY_DIR "${SIMPLE_ENGINE_LIB_BINARY_DIR}/dep")

add_subdirectory("${DEPENDENCY_SOURCE_DIR}/glfw"
    "${DEPENDENCY_BINARY_DIR}/glfw")
add_subdirectory("${DEPENDENCY_SOURCE_DIR}/glm"
    "${DEPENDENCY_BINARY_DIR}/glm")
target_link_libraries(simple_engine
    PUBLIC
	glfw
	glm::glm-header-only
)
target_sources(simple_engine
    PRIVATE 
	${DEPENDENCY_SOURCE_DIR}/glad/src/glad.c
	${DEPENDENCY_SOURCE_DIR}/stb/stb_image.c 
    PUBLIC
	FILE_SET glad_headers
	TYPE HEADERS
	BASE_DIRS ${DEPENDENCY_SOURCE_DIR}/glad/include
	FILES 
	    ${DEPENDENCY_SOURCE_DIR}/glad/include/glad/glad.h
	    ${DEPENDENCY_SOURCE_DIR}/glad/include/KHR/khrplatform.h
    PUBLIC
	FILE_SET stb_headers
	TYPE HEADERS
	BASE_DIRS ${DEPENDENCY_SOURCE_DIR}/stb
	FILES 
	    ${DEPENDENCY_SOURCE_DIR}/stb/stb_image.h
)

#src
target_sources(simple_engine
    PRIVATE
	engine.cpp
	window.cpp
	shader.cpp
	shader_program.cpp
	mesh.cpp
	image.cpp
	texture.cpp
    PUBLIC
	FILE_SET HEADERS
	FILES
	    engine.h
	    window.h
	    shader.h
	    shader_program.h
	    mesh.h
	    image.h
	    texture.h
	    attribute_layout.h
	    gl_helper.h
)

#optional audio
if(SIMPLE_ENGINE_BUILD_WITH_AUDIO)
    target_sources(simple_engine
	PRIVATE
	    ${DEPENDENCY_SOURCE_DIR}/miniaudio/miniaudio.c
	PUBLIC
	    FILE_SET HEADERS
	    BASE_DIRS 
		${DEPENDENCY_SOURCE_DIR}/miniaudio
	    FILES ${DEPENDENCY_SOURCE_DIR}/miniaudio/miniaudio.h
    )
    target_sources(simple_engine
	PRIVATE
	    audio/audio.cpp
	PUBLIC
	    FILE_SET HEADERS
	    FILES audio/audio.h
    )
endif()

if(SIMPLE_ENGINE_BUILD_WITH_DRAW_OBJECTS)
    target_sources(simple_engine
	PUBLIC
	    FILE_SET HEADERS
	    FILES 
		draw_objects/draw_object.h
		draw_objects/draw_mesh.h
		draw_objects/draw_textured_mesh.h
		draw_objects/uniform_value.h
		draw_objects/transform_2d.h
		draw_objects/transform_3d.h
		draw_objects/draw_mesh_2d_t.h
		draw_objects/draw_mesh_3d_t.h
		draw_objects/draw_textured_mesh_2d_t.h
		draw_objects/draw_textured_mesh_3d_t.h
    )
endif()
