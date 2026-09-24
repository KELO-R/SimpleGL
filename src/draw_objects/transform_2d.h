#pragma once
#include "uniform_value.h"
#include "glm/glm.hpp"

struct Transform2D : public IUniformValue{
    GLsizei mat_program_id;
    glm::mat3 mat;

    Transform2D(GLsizei mat_program_id, glm::mat3 mat = glm::mat3())
	:mat_program_id(mat_program_id), mat(mat){}
    virtual std::expected<void, std::string> set_uniforms(const ShaderProgram& program) override{
	return program.set_uniformv(mat_program_id, &mat, 1, GL_FALSE);
    };
};
