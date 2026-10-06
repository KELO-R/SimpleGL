cmake_minimum_required(VERSION 3.20)
project(SIMPLE_GL_EXAMPLES)

if(SIMPLE_GL_USE_STB_IMAGE)
    add_subdirectory(dep/glfw)

    add_executable(image_window)
    target_link_libraries(image_window 
	PRIVATE 
	    simple_gl
	    glfw
    )
    target_include_directories(image_window PRIVATE simple_gl)
    target_sources(image_window
	PRIVATE
	    image_window/main.cpp
    )
endif()

file(COPY ${SIMPLE_GL_EXAMPLES_SOURCE_DIR}/res/
    DESTINATION ${SIMPLE_GL_EXAMPLES_BINARY_DIR}/res)
