#include "window.h"
#include "glfw/glfw3.h"

std::expected<Window, std::string> Window::opengl_window(
    int width,
    int height,
    std::string_view title,
    GLFWmonitor *monitor,
    GLFWwindow *share,
    int gl_version_minor,
    int gl_version_major,
    int gl_profile,
    bool init_glad
    ){

    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, gl_version_minor);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, gl_version_major);
    glfwWindowHint(GLFW_OPENGL_PROFILE, gl_profile);
    GLFWwindow* window = glfwCreateWindow(width, height, title.cbegin(), monitor, share);
    if(!window){
	return std::unexpected("ERROR! GLFW WINDOW: window creation failed!");
    }
    if(init_glad){
	auto previous = glfwGetCurrentContext();
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
	    return std::unexpected("Failed to initialize GLAD for current context");
	}
	glfwMakeContextCurrent(previous);
    }

    return Window(
	std::unique_ptr<GLFWwindow, decltype(&free_window)>(window, free_window)
    );
}

