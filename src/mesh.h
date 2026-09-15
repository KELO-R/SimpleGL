#pragma once
#include <glad/glad.h>
#include <expected>
#include <string>
#include <vector>
#include "attribute_layout.h"

class Mesh{
    unsigned int vao;
    size_t indices_count;
    Mesh(unsigned int vao, size_t indices_count)
	: vao(vao), indices_count(indices_count){}
public:
    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;

    static std::expected<Mesh, std::string> try_from(
	const std::vector<AttributeLayout>& vbo_layout,
	const void* vertices, size_t vertices_byte_size,
	const void* indices, size_t indices_byte_size, 
	size_t indices_count);
    void draw_elements() const;
};
