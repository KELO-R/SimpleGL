#pragma once
#include "glad/glad.h"
#include "glfw/glfw3.h"
#include <expected>
#include <string>

class Engine{
    friend class EngineBuilder;
private:
    Engine(bool free)
	:free(free){};
public:
    Engine(const Engine&) = delete;
    Engine& operator=(const Engine&) = delete;
    Engine(Engine&& moved)
	:free(moved.free){
	moved.free = false;
    }
    Engine& operator=(Engine&& moved){
	this->free = moved.free;
	moved.free = false;
	return *this;
    }
    ~Engine(){
	if (free){
	    glfwTerminate();
	}
    }
    static std::expected<Engine, std::string> opengl_glfw_engine();
private:
    bool free {false};
};

struct EngineBuilder{
    EngineBuilder() = default;
    std::expected<Engine, std::string> build();
};
