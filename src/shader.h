#pragma once
#include <glad/glad.h>
#include <expected>
#include <string>

class Shader{
    GLuint id;
    int type;

    Shader(GLuint id, int type)
	: id(id), type(type){}
public:
    friend class ShaderProgram;
    ~Shader(){
	glDeleteShader(id);
    }
    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;
    Shader(const Shader&& other) = delete;
    Shader& operator=(const Shader&&) = delete;

    static std::expected<Shader, std::string> try_from(const char* const source, GLuint gl_shader_type);
};
