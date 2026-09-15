#include <iostream>
#include <string>
#include "lib.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
}

void process_input(GLFWwindow* window){
    if(glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    else if(glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
	glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
    else if(glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
    else if(glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
}

int test(){
    if (!glfwInit()){
	std::cerr << "glfw failed to initialize!" << std::endl;
	return 1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(500, 500, "GLFW window title", NULL, NULL);
    if (window == NULL){
	std::cerr << "glfw window initialization failed" << std::endl;
	glfwTerminate();
	return 2;
    }
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
	std::cout << "glad failed to initialize!" << std::endl;
	return 3;
    }
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glViewport(0, 0, 500, 500);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

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
    unsigned int vao;
    glCreateVertexArrays(1, &vao);
    glBindVertexArray(vao);

    make_vbo(vertices, sizeof(vertices));
    make_ebo(indices, sizeof(indices));

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
    glEnableVertexAttribArray(0);
    auto program_result = create_program();
    if (program_result.has_value()){
	glUseProgram(program_result.value());
    }
    else {
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
    }

    while (!glfwWindowShouldClose(window)){
	process_input(window);
	glClear(GL_COLOR_BUFFER_BIT);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glfwSwapBuffers(window);
	glfwPollEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

unsigned int make_vbo(float* vertices, unsigned int vertices_byte_size){
    unsigned int vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices_byte_size, vertices, GL_STATIC_DRAW);
    return vbo;
}
unsigned int make_ebo(unsigned* indices, unsigned int indices_byte_size){
    unsigned int ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_byte_size, indices, GL_STATIC_DRAW);
    return ebo;
}

std::optional<unsigned int> create_program(){
    const char* vertex_shader_source = R"(
#version 330 core
layout (location = 0) in vec3 aPos;

void main()
{
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
    )";
    const char* fragment_shader_source = R"(
#version 330 core
out vec4 FragColor;

void main()
{
    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
} 
    )";
    auto vertex_shader_result = compile_shader(vertex_shader_source, GL_VERTEX_SHADER);
    unsigned int vertex_shader_id;
    if (vertex_shader_result.has_value()){
	vertex_shader_id = vertex_shader_result.value();
    }else{
	return std::optional<unsigned int>();
    }

    auto fragment_shader_result = compile_shader(fragment_shader_source, GL_FRAGMENT_SHADER);
    unsigned int fragment_shader_id;
    if (fragment_shader_result.has_value()){
	fragment_shader_id = fragment_shader_result.value();
    }else{
	return std::optional<unsigned int>();
    }
    unsigned int program_id;
    program_id = glCreateProgram();
    glAttachShader(program_id, vertex_shader_id);
    glAttachShader(program_id, fragment_shader_id);
    glLinkProgram(program_id);
    glDeleteShader(vertex_shader_id);
    glDeleteShader(fragment_shader_id);
    int program_link_status;
    glGetProgramiv(program_id, GL_LINK_STATUS, &program_link_status);
    if(!program_link_status){
	int log_length;
	glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &log_length);
	std::string compile_log;
	compile_log.resize(log_length);
	glGetProgramInfoLog(program_id, log_length, &log_length, compile_log.data());
	std::cout << "Shader Program linking failed!\n";
	std::cout << compile_log << '\n';
	return std::optional<unsigned int>();
    }
    return std::optional<unsigned int>(program_id);
}

std::optional<unsigned int> compile_shader(const char* const source, int gl_shader_type){
    unsigned int shader_id;
    shader_id = glCreateShader(gl_shader_type);
    glShaderSource(shader_id, 1, &source, NULL);
    glCompileShader(shader_id);
    
    int compiled;
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &compiled);
    if(!compiled){
	int log_length;
	glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &log_length);
	std::string compile_log;
	compile_log.resize(log_length);
	glGetShaderInfoLog(shader_id, log_length, &log_length, compile_log.data());
	std::cout << "Shader compilation failed!\n";
	std::cout << compile_log << '\n';
	return std::optional<unsigned int>();
    }
    return std::optional<unsigned int>(shader_id);
}
