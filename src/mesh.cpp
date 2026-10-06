#include "mesh.h"
#include <format>
//#include <iostream>
#include "gl_helper.h"

std::expected<void, std::string>
inline setup_attrib_layout(const std::vector<AttributeLayout> &attributes){
    unsigned int stride_size{};
    if (attributes.size() > 1){
	for(size_t i{}; i < attributes.size(); i++){
	    stride_size += attributes[i].count * glenum_size(attributes[i].type);
	}
    }
    const char* offset = (char*)(0);
    for(size_t i{}; i < attributes.size(); i++){
	const AttributeLayout &attrib = attributes[i];
	glVertexAttribPointer(i, attrib.count, unwrap(attrib.type), attrib.normalize, stride_size, (void*)offset);
	offset += attrib.count * glenum_size(attrib.type);
	glEnableVertexAttribArray(i);
    }
    return {};
}

//TODO address the ambiguity type used for indices
std::expected<Mesh, std::string> Mesh::try_from(
	const std::vector<AttributeLayout>& layout,
	const void* vertices, size_t vertices_byte_size,
	const void* indices, size_t indices_byte_size, size_t indices_count){
    Vao vao {new GLuint, free_vao};
    glGenVertexArrays(1, vao.get());
    glBindVertexArray(*vao);

    Vao vbo {new GLuint, free_bo};
    glGenBuffers(1, vbo.get());
    glBindBuffer(GL_ARRAY_BUFFER, *vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices_byte_size, vertices, GL_STATIC_DRAW);

    Vao ebo {new GLuint, free_bo};
    glGenBuffers(1, ebo.get());
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_byte_size, indices, GL_STATIC_DRAW);

    auto r = setup_attrib_layout(layout);
    if(!r.has_value()){
	return std::unexpected(std::format("Mesh creation failed! {}", r.error()));
    }

    glBindVertexArray(0);
    return Mesh(std::move(vao), std::move(vbo), std::move(ebo), indices_count);
}

void Mesh::draw_elements() const{
    glBindVertexArray(*vao);
    glDrawElements(GL_TRIANGLES, indices_count, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
