#pragma once
#include "glad/glad.h"
#include "gl_helper.h"

struct AttributeLayout{
    unsigned int count;
    GLenumWrapper type;
    GLboolean normalize;

    AttributeLayout(unsigned int count, GLenumWrapper type, GLboolean normalize)
	: count(count), type(type), normalize(normalize){}
};
