#pragma once
#include <glad/glad.h>
#include <memory>
#include <vector>
#include "shader.h"

class ShaderProgram{
private:
    struct Uniform{
	Uniform(GLint size = {}, GLenum type = {}, const std::string& name = {})
	    :size(size), type(type), name(name){}
	GLint size;
	GLenum type;
	std::string name;
    };
    static void free_program(GLuint* program){
	glDeleteProgram(*program);
	delete program;
    }

    std::unique_ptr<GLuint, decltype(&free_program)> program;
    std::vector<Uniform> uniforms;

    ShaderProgram(std::unique_ptr<GLuint, decltype(&free_program)>&& program, std::vector<Uniform>&& uniforms)
	:program(std::move(program)), uniforms(std::move(uniforms)){}

    std::vector<Uniform> extract_uniforms();
public:
    static std::expected<ShaderProgram, std::string> try_from(const std::vector<Shader>& shaders);

    inline const std::vector<Uniform>& get_uniforms() const{
	return uniforms;
    }

    void use_program() const;
    GLint get_uniform_location(std::string_view uniform_name) const;
};
