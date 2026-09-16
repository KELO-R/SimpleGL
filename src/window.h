#pragma once
#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <expected>
#include <string>
#include <memory>

class Window{
private:
    static void free_window (GLFWwindow* window){
	glfwDestroyWindow(window);
    }
    std::unique_ptr<GLFWwindow, decltype(&free_window)> window;

    Window(std::unique_ptr<GLFWwindow, decltype(&free_window)>&& window)
	:window(std::move(window)){}

public:
    const GLFWwindow* get_window(){
	return window.get();
    }

    static std::expected<Window, std::string> opengl_window(
	int width = 500, 
	int height = 500, 
	std::string_view title = "", 
	GLFWmonitor *monitor = NULL, 
	GLFWwindow *share = NULL,
	int gl_version_minor = 3,
	int gl_version_major = 3,
	int gl_profile = GLFW_OPENGL_CORE_PROFILE,
	bool init_glad = true);

    inline bool should_close() const{
	return glfwWindowShouldClose(window.get()); 
    }
    inline void make_context_current(){ 
	return glfwMakeContextCurrent(window.get()); 
    }
    inline void swap_buffers(){
	return glfwSwapBuffers(window.get()); 
    }
};
