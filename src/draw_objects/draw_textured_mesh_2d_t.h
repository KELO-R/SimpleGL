#pragma once
#include "draw_objects/transform_2d.h"
#include "draw_textured_mesh.h"
#include "texture.h"
#include <utility>

class DrawTexturedMesh2DT : public DrawTexturedMesh, public Transform2D{
private:
public:
    DrawTexturedMesh2DT(
	const Mesh* mesh,
	const ShaderProgram* program,
	std::vector<std::pair<const Texture*, GLenum>>&& texture_slot_pairs,
	GLsizei transform_mat_program_id,
	glm::mat3 transform_mat)
	    : DrawTexturedMesh(mesh, program, std::move(texture_slot_pairs)),
	    Transform2D(transform_mat_program_id, std::move(transform_mat)){}

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
