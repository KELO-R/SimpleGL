#pragma once

#include <glad/glad.h>
#include <expected>
#include <string>
class Vbo{
    GLuint id;
    Vbo(GLuint id)
	:id(id){}
    ~Vbo(){
	glDeleteBuffers(GL_VERTEX_ARRAY, &id);
    }
public:
    static std::expected<Vbo, std::string> try_from();
};
