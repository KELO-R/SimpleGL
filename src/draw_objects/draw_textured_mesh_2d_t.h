#pragma once
#include "draw_mesh_2d_t.h"
#include "texture.h"
#include <utility>

class DrawTexturedMesh2DT : public DrawMesh2DT{
private:
    std::vector<std::pair<const Texture*, GLenum>> texture_slot_pairs;
public:
    DrawTexturedMesh2DT(
	GLsizei transform_mat_program_id,
	const Mesh* mesh,
	const ShaderProgram* program,
	glm::mat3 transform_mat,
	std::vector<std::pair<const Texture*, GLenum>>&& texture_slot_pairs)
	    : DrawMesh2DT(transform_mat_program_id, mesh, program, transform_mat),
	    texture_slot_pairs(std::move(texture_slot_pairs)){};

    std::expected<void, std::string> draw() override{
	for(auto& it : texture_slot_pairs){
	    it.first->bind_texture(it.second);
	}
	return DrawMesh2DT::draw();
    }
};
