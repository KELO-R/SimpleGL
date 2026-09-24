#pragma once
#include "draw_mesh_2d.h"
#include "texture.h"
#include <utility>

class DrawTexturedMesh2D : public DrawMesh2D{
private:
    std::vector<std::pair<const Texture*, GLenum>> texture_slot_pairs;
public:
    DrawTexturedMesh2D(
	const Mesh* mesh, 
	const ShaderProgram* program, 
	std::vector<std::pair<const Texture*, GLenum>>&& texture_slot_pairs)
	    : DrawMesh2D(mesh, program), texture_slot_pairs(std::move(texture_slot_pairs)){};

    std::expected<void, std::string> draw() override{
	for(auto& it : texture_slot_pairs){
	    it.first->bind_texture(it.second);
	}
	return DrawMesh2D::draw();
    }
};
