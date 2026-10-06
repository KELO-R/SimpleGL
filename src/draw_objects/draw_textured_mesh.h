#pragma once
#include "draw_mesh.h"
#include "texture.h"
#include <utility>

class DrawTexturedMesh : public DrawMesh{
private:
    std::vector<std::pair<const Texture*, GLenum>> texture_slot_pairs;
public:
    DrawTexturedMesh(
	const Mesh* mesh, 
	const ShaderProgram* program, 
	std::vector<std::pair<const Texture*, GLenum>>&& texture_slot_pairs)
	    : DrawMesh(mesh, program), texture_slot_pairs(std::move(texture_slot_pairs)){};

    std::expected<void, std::string> draw() override{
	for(auto& it : texture_slot_pairs){
	    it.first->bind_texture(it.second);
	}
	return DrawMesh::draw();
    }
};
