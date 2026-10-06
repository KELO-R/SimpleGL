#include "texture.h"
#include <iostream>

std::expected<Texture, std::string> Texture::texture2d(
	GLsizei width,
	GLsizei height,
	const void* data,
	GLenum target,
	GLenum source_format,
	GLint internal_format,
	GLenum source_type,
	bool generate_mipmap,
	GLint level,
	GLenum wrap_s,
	GLenum wrap_t,
	GLenum min_filter,
	GLenum mag_filter
    ){
    std::unique_ptr<GLuint, decltype(&free_texture)> texture(new GLuint, free_texture);
    glGenTextures(1, texture.get());
    glBindTexture(target, *texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter);

    glTexImage2D(target, level, internal_format, width, height, 0, source_format, source_type, data);
    if(generate_mipmap)
	glGenerateMipmap(target);
    std::cout << "width :" << width <<'\n';
    std::cout << "height :" << width <<'\n';
    std::cout << "data != nullptr :" << (data != nullptr) <<'\n';
    return Texture(std::move(texture), target);
}
