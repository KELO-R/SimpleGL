#include "mesh.h"
#include <format>
#include <iostream>
#include <gl_helper.h>

std::expected<void, std::string>
inline setup_attrib_layout(const std::vector<AttributeLayout> &attributes){
    unsigned int stride_size{};
    for(size_t i{1}; i < attributes.size(); i++){
	stride_size += attributes[i].count * glenum_size(attributes[i].type);
    }
    std::cout << std::format("stride_size:{}", stride_size) <<'\n';

    const char* offset = (char*)(void*)(0);
    for(size_t i{}; i < attributes.size(); i++){
	const AttributeLayout &attrib = attributes[i];
	glVertexAttribPointer(i, attrib.count, unwrap(attrib.type), attrib.normalize, stride_size, (void*)offset);
	offset += attrib.count + glenum_size(attrib.type);
	glEnableVertexAttribArray(i);
    }
    return {};
}

//TODO address the ambiguity type used for indices
std::expected<Mesh, std::string> Mesh::try_from(
	const std::vector<AttributeLayout>& layout,
	const void* vertices, size_t vertices_byte_size,
	const void* indices, size_t indices_byte_size, size_t indices_count){
    unsigned int vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    unsigned int vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices_byte_size, vertices, GL_STATIC_DRAW);
    check_gl("after vbo");

    unsigned int ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_byte_size, indices, GL_STATIC_DRAW);
    check_gl("after ebo");

    auto r = setup_attrib_layout(layout);
    check_gl("after setup attrib layout");
    if(!r.has_value()){
	return std::unexpected(std::format("Mesh creation failed! {}", r.error()));
    }

    glBindVertexArray(0);
    return Mesh(vao, indices_count);
}

void Mesh::draw_elements() const{
    std::cout <<std::format("glIsVertexArray:{}", glIsVertexArray(vao)) << '\n';
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    //glDrawElements(GL_TRIANGLES, indices_count, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
