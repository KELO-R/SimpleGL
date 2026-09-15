#pragma once
#include <glad/glad.h>
#include <vector>
#include "shader.h"

class ShaderProgram{
    GLuint id;
    ShaderProgram(GLuint id)
	:id(id){}
public:
    ~ShaderProgram(){
	glDeleteProgram(id);
    }
    ShaderProgram(const ShaderProgram&) = delete;
    ShaderProgram& operator=(const ShaderProgram&) = delete;
    static std::expected<ShaderProgram, std::string> try_from(const std::vector<const Shader*>& shaders);
    void use_program() const;
};
