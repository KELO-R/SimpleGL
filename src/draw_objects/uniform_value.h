#pragma once
#include "shader_program.h"
#include <expected>
#include <string>

class IUniformValue{
protected:
    IUniformValue(){};
public:
    virtual std::expected<void, std::string> set_uniforms(const ShaderProgram& program) = 0;
};
