#pragma once
#include "glad/glad.h"
#include <expected>
#include <string>
#include <memory>
#include "image.h"


struct Texture{
private:
    static void free_texture(GLuint* texture){
	glDeleteTextures(1, texture);
	delete texture;
    }
    std::unique_ptr<GLuint, decltype(&free_texture)> texture;
    GLenum target;
    Texture(std::unique_ptr<GLuint, decltype(&free_texture)>&& texture, GLenum target)
	:texture(std::move(texture)), target(target){}
public:
    static std::expected<Texture, std::string> texture2d(
	GLsizei width,
	GLsizei height,
	const void* data,
	GLenum target = GL_TEXTURE_2D,
	GLenum source_format = GL_RGB,
	GLint internal_format = GL_RGB,
	GLenum source_type = GL_UNSIGNED_BYTE,
	bool generate_mipmap = true,
	GLint level = 0,
	GLenum wrap_s = GL_REPEAT,
	GLenum wrap_t = GL_REPEAT,
	GLenum min_filter = GL_LINEAR_MIPMAP_LINEAR,
	GLenum mag_filter = GL_LINEAR
    );

    inline void bind_texture(GLenum texture_unit_slot) const{
	glActiveTexture(texture_unit_slot);
	glBindTexture(GL_TEXTURE_2D, *texture);
    }
};

struct TextureBuilder{
private:
public:
    GLsizei width {}, height{};
    const void* data{};
    GLenum target {GL_TEXTURE_2D};
    GLenum source_format {GL_UNSIGNED_BYTE};
    GLint internal_format {GL_RGB};
    GLenum source_type {GL_RGB};
    bool generate_mipmap {true};
    GLint level{0};

    TextureBuilder(){};

    inline TextureBuilder& set_width(GLsizei width){
	this->width = width;
	return *this;
    }
    inline TextureBuilder& set_height(GLsizei height){
	this->height = height;
	return *this;
    }
    inline TextureBuilder& set_data(const void* data){
	this->data = data;
	return *this;
    }
    inline TextureBuilder& set_target(GLenum target){
	this->target = target;
	return *this;
    }
    inline TextureBuilder& set_source_type(GLenum source_type){
	this->source_type = source_type;
	return *this;
    }
    inline TextureBuilder& set_source_format(GLenum source_format){
	this->source_format = source_format;
	return *this;
    }
    inline TextureBuilder& set_level(GLint level){
	this->level = level;
	return *this;
    }
    inline TextureBuilder& set_generate_mipmap(bool generate_mipmap){
	this->generate_mipmap = generate_mipmap;
	return *this;
    }
    inline TextureBuilder& set_internal_format(GLint internal_format){
	this->internal_format = internal_format;
	return *this;
    }
    inline TextureBuilder& set_size(GLsizei width, GLsizei height){
	this->width = width;
	this->height = height;
	return *this;
    }
    inline TextureBuilder& set_source(GLenum source_type, GLenum source_format){
	this->source_type = source_type;
	this->source_format = source_format;
	return *this;
    }

    #ifdef STBI_INCLUDE_STB_IMAGE_H
    inline TextureBuilder& from_image(const Image& image){
	this->width = image.get_width();
	this->height = image.get_height();
	this->data = image.get_data();
	switch(image.get_channels_count()){
	    case 1:
		this->source_format = GL_RED; 
	    case 2:
		this->source_format = GL_RG; 
	    case 3:
		this->source_format = GL_RGB; 
	    case 4:
		this->source_format = GL_RGBA; 
	    default:
		this->source_format = GL_RGB;
	}
	this->source_type = GL_UNSIGNED_BYTE;
	return *this;
    }
    #endif
    inline std::expected<Texture, std::string> build(){
	return Texture::texture2d(
	    width,
	    height,
	    data,
	    target,
	    source_format,
	    internal_format,
	    source_type,
	    generate_mipmap,
	    level
	);
    }
};
