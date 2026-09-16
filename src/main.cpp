#include "attribute_layout.h"
#include "engine.h"
#include "window.h"
#include "shader_program.h"
#include "mesh.h"
#include <iostream>
#include <vector>

int main(){
    auto engine_r = EngineBuilder().build();
    if(!engine_r.has_value()){
	std::cout << engine_r.error() << std::endl;
	return -1;
    }
    Engine engine {std::move(engine_r.value())};

    auto window_r = Window::opengl_window();
    if(!window_r.has_value()){
	std::cout << window_r.error() << std::endl;
	return -1;
    }
    Window window {std::move(window_r.value())};
    window.make_context_current();

    const char* vertex_shader_source = R"(#version 330 core
	layout (location = 0) in vec3 aPos;

	void main()
	{
	    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
	}
    )";
    const char* fragment_shader_source = R"(#version 330 core
	out vec4 FragColor;

	void main()
	{
	    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
	} 
    )";

    auto frag_shader_r = Shader::try_from(fragment_shader_source, GL_FRAGMENT_SHADER);
    if(!frag_shader_r.has_value()){
	std::cout << frag_shader_r.error() << std::endl;
	return -1;
    }
    Shader frag_shader {std::move(frag_shader_r.value())};

    auto vert_shader_r = Shader::try_from(vertex_shader_source, GL_VERTEX_SHADER);
    if(!vert_shader_r.has_value()){
	std::cout << vert_shader_r.error() << std::endl;
	return -1;
    }
    Shader vert_shader {std::move(vert_shader_r.value())};

    std::vector<Shader> shaders;
    shaders.emplace_back(std::move(vert_shader));
    shaders.emplace_back(std::move(frag_shader));

    auto program_r = ShaderProgram::try_from(shaders);
    if(!program_r.has_value()){
	std::cout << program_r.error() << std::endl;
	return -1;
    }
    ShaderProgram program {std::move(program_r.value())};

    float vertices[] = {
	 0.5f,  0.5f, 0.0f,  // top right
	 0.5f, -0.5f, 0.0f,  // bottom right
	-0.5f, -0.5f, 0.0f,  // bottom left
	-0.5f,  0.5f, 0.0f   // top left 
    };
    unsigned int indices[] = {  // note that we start from 0!
	0, 1, 3,   // first triangle
	1, 2, 3    // second triangle
    }; 

    std::vector<AttributeLayout> layout {
	AttributeLayout(3, GLenumWrapper::FLOAT, GL_FALSE)
    }; 

    auto mesh_r = Mesh::try_from(layout, 
	    vertices, sizeof(vertices), 
	    indices, sizeof(indices), sizeof(indices) / sizeof(unsigned int));
    if(!mesh_r.has_value()){
	std::cout << mesh_r.error() << std::endl;
	return -1;
    }
    Mesh mesh {std::move(mesh_r.value())};

    glClearColor(0.3f, 0.4f, 0.1f, 1.0f);
    program.use_program();
    while(!window.should_close()){
	glfwPollEvents();
	glClear(GL_COLOR_BUFFER_BIT);
	mesh.draw_elements();
	window.swap_buffers();
    }
}
