#pragma once
#include "uniform_value.h"
#include "glm/glm.hpp"

namespace SimpleGL{
	struct Transform3D : public IUniformValue{
		GLsizei mat_program_id;
		glm::mat4 mat;

		Transform3D(GLsizei mat_program_id, glm::mat4 mat = glm::mat4())
		:mat_program_id(mat_program_id), mat(mat){}
		virtual std::expected<void, std::string> set_uniforms(const ShaderProgram& program) override{
		return program.set_uniformv(mat_program_id, &mat, 1, GL_FALSE);
		};
	};
}
