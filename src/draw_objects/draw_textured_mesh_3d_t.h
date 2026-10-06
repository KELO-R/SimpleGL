#pragma once
#include "draw_objects/transform_3d.h"
#include "draw_textured_mesh.h"
#include "texture.h"
#include <utility>

class DrawTexturedMesh3DT : public DrawTexturedMesh, public Transform3D{
private:
public:
    DrawTexturedMesh3DT(
	const Mesh* mesh,
	const ShaderProgram* program,
	std::vector<std::pair<const Texture*, GLenum>>&& texture_slot_pairs,
	GLsizei transform_mat_program_id,
	glm::mat4 transform_mat)
	    : DrawTexturedMesh(mesh, program, std::move(texture_slot_pairs)),
	    Transform3D(transform_mat_program_id, std::move(transform_mat)){}

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
