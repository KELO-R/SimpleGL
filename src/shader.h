#pragma once
#include <expected>
#include <filesystem>
#include <glad/glad.h>
#include <memory>
#include <string>

class Shader {
  private:
	// This pattern is easy solution to manage Resources.
	// It is not the best performance solution, because it allocates on heap.
	// Even if I can write template wrapper that resembles std::unique_ptr but
	// for values, I WILL NOT.
	static void free_shader(GLuint *shader) {
		glDeleteShader(*shader);
		delete shader;
	}
	std::unique_ptr<GLuint, decltype(&free_shader)> shader;
	GLenum type;

	Shader(std::unique_ptr<GLuint, decltype(&free_shader)> &&shader_name,
	       GLenum type)
	    : shader(std::move(shader_name)), type(type) {}

  public:
	friend class ShaderProgram;
	static std::expected<Shader, std::string> try_from(const char *const source,
	                                                   GLenum gl_shader_type);
	static std::expected<Shader, std::string>
	try_from(const std::filesystem::path &path, GLenum gl_shader_type);
};
