#pragma once
#include "draw_mesh.h"
#include "transform_3d.h"

namespace SimpleGL{
	class DrawMesh3DT : public DrawMesh, public Transform3D{
	public:
		DrawMesh3DT(
		GLsizei transform_mat_program_id, const Mesh* mesh, const ShaderProgram* program, glm::mat4 transform_mat)
			:DrawMesh(mesh, program), Transform3D(transform_mat_program_id, std::move(transform_mat)){};
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
}
