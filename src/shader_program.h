#pragma once
#include <expected>
#include <glad/glad.h>
#include <memory>
#include <vector>
#include "glm/glm.hpp"
#include "glm/matrix.hpp"
#include "shader.h"

class ShaderProgram{
private:
    struct Uniform{
	GLint location;
	GLint size;
	GLenum type;
	std::string name;
    };
    static void free_program(GLuint* program){
	glDeleteProgram(*program);
	delete program;
    }

    std::unique_ptr<GLuint, decltype(&free_program)> program;
    std::vector<Uniform> uniforms;

    ShaderProgram(std::unique_ptr<GLuint, decltype(&free_program)>&& program, std::vector<Uniform>&& uniforms)
	:program(std::move(program)), uniforms(std::move(uniforms)){}

    std::vector<Uniform> extract_uniforms();
public:
    static std::expected<ShaderProgram, std::string> try_from(const std::vector<Shader>& shaders);

    void use_program() const;
    GLint get_uniform_location(std::string_view uniform_name) const;

    //UNIFORMS
    inline const std::vector<Uniform>& get_uniforms() const{
	return uniforms;
    }
    const std::expected<Uniform, std::string> get_uniform(size_t id) const;
    const std::expected<size_t, std::string> get_uniform_id(std::string_view name) const;

    const std::expected<void, std::string> set_uniform(size_t id, GLfloat v0) const;
    const std::expected<void, std::string> set_uniform(size_t id, glm::vec2 v) const;
    const std::expected<void, std::string> set_uniform(size_t id, glm::vec3 v) const;
    const std::expected<void, std::string> set_uniform(size_t id, glm::vec4 v) const;
    const std::expected<void, std::string> set_uniform(size_t id, GLint v0) const;
    const std::expected<void, std::string> set_uniform(size_t id, glm::ivec2 v) const;
    const std::expected<void, std::string> set_uniform(size_t id, glm::ivec3 v) const;
    const std::expected<void, std::string> set_uniform(size_t id, glm::ivec4 v) const;
    const std::expected<void, std::string> set_uniform(size_t id, GLuint v0) const;
    const std::expected<void, std::string> set_uniform(size_t id, glm::uvec2 v) const;
    const std::expected<void, std::string> set_uniform(size_t id, glm::uvec3 v) const;
    const std::expected<void, std::string> set_uniform(size_t id, glm::uvec4 v) const;

    const std::expected<void, std::string> set_uniformv(size_t id, const GLfloat* ptr, GLsizei count) const;
    const std::expected<void, std::string> set_uniformv(size_t id, const glm::vec2* ptr, GLsizei count) const;
    const std::expected<void, std::string> set_uniformv(size_t id, const glm::vec3* ptr, GLsizei count) const;
    const std::expected<void, std::string> set_uniformv(size_t id, const glm::vec4* ptr, GLsizei count) const;
    const std::expected<void, std::string> set_uniformv(size_t id, const GLint* ptr, GLsizei count) const;
    const std::expected<void, std::string> set_uniformv(size_t id, const glm::ivec2* ptr, GLsizei count) const;
    const std::expected<void, std::string> set_uniformv(size_t id, const glm::ivec3* ptr, GLsizei count) const;
    const std::expected<void, std::string> set_uniformv(size_t id, const glm::ivec4* ptr, GLsizei count) const;
    const std::expected<void, std::string> set_uniformv(size_t id, const GLuint* ptr, GLsizei count) const;
    const std::expected<void, std::string> set_uniformv(size_t id, const glm::uvec2* ptr, GLsizei count) const;
    const std::expected<void, std::string> set_uniformv(size_t id, const glm::uvec3* ptr, GLsizei count) const;
    const std::expected<void, std::string> set_uniformv(size_t id, const glm::uvec4* ptr, GLsizei count) const;
    const std::expected<void, std::string> set_uniformv(size_t id, const glm::mat2* ptr, GLsizei count, GLboolean transpose) const;
    const std::expected<void, std::string> set_uniformv(size_t id, const glm::mat3* ptr, GLsizei count, GLboolean transpose) const;
    const std::expected<void, std::string> set_uniformv(size_t id, const glm::mat4* ptr, GLsizei count, GLboolean transpose) const;
    const std::expected<void, std::string> set_uniformv(size_t id, const glm::mat2x3* ptr, GLsizei count, GLboolean transpose) const;
    const std::expected<void, std::string> set_uniformv(size_t id, const glm::mat3x2* ptr, GLsizei count, GLboolean transpose) const;
    const std::expected<void, std::string> set_uniformv(size_t id, const glm::mat2x4* ptr, GLsizei count, GLboolean transpose) const;
    const std::expected<void, std::string> set_uniformv(size_t id, const glm::mat4x2* ptr, GLsizei count, GLboolean transpose) const;
    const std::expected<void, std::string> set_uniformv(size_t id, const glm::mat3x4* ptr, GLsizei count, GLboolean transpose) const;
    const std::expected<void, std::string> set_uniformv(size_t id, const glm::mat4x3* ptr, GLsizei count, GLboolean transpose) const;
};
