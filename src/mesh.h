#pragma once
#include "attribute_layout.h"
#include <expected>
#include <glad/glad.h>
#include <memory>
#include <string>
#include <vector>

class Mesh {
	static void free_vao(GLuint *vao) {
		glDeleteVertexArrays(1, vao);
		delete vao;
	}
	static void free_bo(GLuint *bo) { delete bo; }

	using Vao = std::unique_ptr<GLuint, decltype(&free_vao)>;
	using Buffer = std::unique_ptr<GLuint, decltype(&free_bo)>;

	Vao vao;
	Buffer vbo;
	Buffer ebo;
	size_t indices_count;

	Mesh(Vao &&vao, Buffer &&vbo, Buffer &&ebo, GLsizei indices_count)
	    : vao(std::move(vao)), vbo(std::move(vbo)), ebo(std::move(ebo)),
	      indices_count(indices_count) {}

  public:
	static std::expected<Mesh, std::string>
	try_from(const std::vector<AttributeLayout> &vbo_layout,
	         const void *vertices, size_t vertices_byte_size,
	         const void *indices, size_t indices_byte_size,
	         size_t indices_count);
	void draw_elements() const;
};
