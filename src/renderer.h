#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "resources.h"
#include "shader_program.h"
#include "mesh.h"

class Renderer{
public:
    Renderer(){};
    ResourceManager<Shader> shaders;
    ResourceManager<ShaderProgram> shader_programs;
    ResourceManager<Mesh> meshes;

    static std::expected<Renderer, std::string> create();
private:
};
