cmake_minimum_required(VERSION 3.20)
project(SIMPLE_GL_LIB)

add_library(simple_gl)

#main dependencies
set(DEPENDENCY_SOURCE_DIR "${SIMPLE_GL_SOURCE_DIR}/dep")
set(DEPENDENCY_BINARY_DIR "${SIMPLE_GL_LIB_BINARY_DIR}/dep")

add_subdirectory("${DEPENDENCY_SOURCE_DIR}/glm"
    "${DEPENDENCY_BINARY_DIR}/glm")
target_link_libraries(simple_gl
    PUBLIC
	glm::glm-header-only
)

target_sources(simple_gl
    PRIVATE
	${DEPENDENCY_SOURCE_DIR}/glad/src/glad.c
    PUBLIC
	FILE_SET glad_headers
	TYPE HEADERS
	BASE_DIRS ${DEPENDENCY_SOURCE_DIR}/glad/include
	FILES 
	    ${DEPENDENCY_SOURCE_DIR}/glad/include/glad/glad.h
	    ${DEPENDENCY_SOURCE_DIR}/glad/include/KHR/khrplatform.h
)

if(SIMPLE_GL_USE_STB_IMAGE)
target_sources(simple_gl
    PRIVATE 
	${DEPENDENCY_SOURCE_DIR}/stb/stb_image.c 
    PUBLIC
	FILE_SET HEADERS
	BASE_DIRS ${DEPENDENCY_SOURCE_DIR}/stb
	FILES 
	    ${DEPENDENCY_SOURCE_DIR}/stb/stb_image.h
)
endif()

if(SIMPLE_GL_USE_STB_IMAGE)
target_sources(simple_gl
    PRIVATE
	image.cpp
    PUBLIC
	FILE_SET HEADERS
	BASE_DIRS .
	FILES
	    image.h
)
endif()
#src
target_sources(simple_gl
    PRIVATE
	shader.cpp
	shader_program.cpp
	mesh.cpp
	texture.cpp
    PUBLIC
	FILE_SET HEADERS
	BASE_DIRS .
	FILES
	    shader.h
	    shader_program.h
	    mesh.h
	    texture.h
	    attribute_layout.h
)


if(SIMPLE_GL_BUILD_WITH_DRAW_OBJECTS)
    target_sources(simple_gl
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
