#pragma once
#include "glad/glad.h"
#include "gl_helper.h"

struct AttributeLayout{
    unsigned int count;
    GLenumAttributeWrapper type;
    GLboolean normalize;

    AttributeLayout(unsigned int count, GLenumAttributeWrapper type, GLboolean normalize)
	: count(count), type(type), normalize(normalize){}
};
