cmake_minimum_required(VERSION 3.20)
project(SIMPLE_ENGINE_EXAMPLES)

if(${SIMPLE_ENGINE_BUILD_WITH_AUDIO}
    AND ${SIMPLE_ENGINE_BUILD_WITH_DRAW_OBJECTS})
    add_executable(image_with_sound image_with_sound/main.cpp)
    target_link_libraries(image_with_sound simple_engine)

    add_executable(particles)
    target_sources(particles
	PRIVATE
	particles/main.cpp
	PRIVATE
	    FILE_SET HEADERS
	    FILES 
		particles/particle_generator_cpu.h)
    target_link_libraries(particles simple_engine)
endif()

file(COPY ${SIMPLE_ENGINE_EXAMPLES_SOURCE_DIR}/res/
    DESTINATION ${SIMPLE_ENGINE_EXAMPLES_BINARY_DIR}/res)
