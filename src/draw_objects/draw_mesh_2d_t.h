#pragma once
#include "draw_mesh_2d.h"
#include "transform_2d.h"

class DrawMesh2DT : public DrawMesh2D, public Transform2D{
public:
    DrawMesh2DT(
	GLsizei transform_mat_program_id, const Mesh* mesh, const ShaderProgram* program, glm::mat3 transform_mat)
	    :DrawMesh2D(mesh, program), Transform2D(transform_mat_program_id, std::move(transform_mat)){};
    std::expected<void, std::string> draw() override{
	if(program){
	    program->use_program();
	    auto r = set_uniforms(*program);
	    if(!r.has_value())
		return std::unexpected(r.error());
	}
	if(mesh)
	    mesh->draw_elements();
	return {};
    }
};
