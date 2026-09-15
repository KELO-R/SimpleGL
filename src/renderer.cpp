#include "renderer.h"
#include <expected>

/*
void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
}

std::expected<Renderer, std::string> Renderer::create(){
    if (!glfwInit()){
	return std::unexpected("glfw failed to initialize!");
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(500, 500, "GLFW window title", NULL, NULL);
    if (window == NULL){
	glfwTerminate();
	return std::unexpected("glfw window initialization failed");
    }
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
	glfwDestroyWindow(window);
	glfwTerminate();
	return std::unexpected("glad failed to initialize!");
    }
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glViewport(0, 0, 500, 500);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    return Renderer(window);
}

*/
