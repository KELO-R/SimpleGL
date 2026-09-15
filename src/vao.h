#pragma once

#include <glad/glad.h>
#include <expected>
#include <string>
#include <vector>
#include "gl_helper.h"

class Vao{
    GLuint id;
    Vao(GLuint id)
	:id(id){};
public:
    struct AttributeLayout{
	unsigned int count;
	const GLenumWrapper type;
	GLboolean normalize;

	AttributeLayout(unsigned int count, GLenumWrapper type, GLboolean normalize)
	    : count(count), type(type), normalize(normalize){}
    };
    static std::expected<Vao, std::string>
    try_from(const std::vector<AttributeLayout> &attributes);
};
