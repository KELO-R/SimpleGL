#pragma once
#include "draw_objects/draw_object.h"
#include "mesh.h"
#include "shader_program.h"

struct DrawMesh2D : public IDrawObject{
    const Mesh* mesh;
    const ShaderProgram* program;
public:
    DrawMesh2D(const Mesh* mesh, const ShaderProgram* program)
	:mesh(mesh), program(program){}
    std::expected<void, std::string> draw() override{
	if(program)
	    program->use_program();
	if(mesh)
	    mesh->draw_elements();
	return {};
    }
};
