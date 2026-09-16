#pragma once
#include <glad/glad.h>
#include <memory>
#include <vector>
#include "shader.h"

class ShaderProgram{
    static void free_program(GLuint* program){
	glDeleteProgram(*program);
	delete program;
    }

    std::unique_ptr<GLuint, decltype(&free_program)> program;

    ShaderProgram(std::unique_ptr<GLuint, decltype(&free_program)>&& program)
	:program(std::move(program)){}
public:
    static std::expected<ShaderProgram, std::string> try_from(const std::vector<Shader>& shaders);

    void use_program() const;
};
