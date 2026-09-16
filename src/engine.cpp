#include "engine.h"

std::expected<Engine, std::string> Engine::opengl_glfw_engine(){
    if(!glfwInit()){
	return std::unexpected("Failed to initialize GLFW");
    }
    return Engine(true);
}

std::expected<Engine, std::string> EngineBuilder::build(){
    return Engine::opengl_glfw_engine();
};
