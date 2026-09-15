#include "vao.h"
#include "gl_helper.h"
#include <format>

std::expected<Vao, std::string>
Vao::try_from(const std::vector<AttributeLayout> &attributes){
    unsigned int id;
    glGenVertexArrays(1, &id);
    glBindVertexArray(id);

    unsigned int stride_size{};
    for(auto& it : attributes){
	stride_size += it.count * glenum_size(it.type);
    }

    char* offset = (char*)(0);
    for(size_t i{}; i < attributes.size(); i++){
	const AttributeLayout &attrib = attributes[i];
	glVertexAttribPointer(i, attrib.count, attrib.type, attrib.normalize, stride_size, (void*)offset);
	offset += attrib.count + glenum_size(attrib.type);
	glEnableVertexAttribArray(i);
    }
    glBindVertexArray(id);
    return Vao(id);
}
