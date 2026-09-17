#include "shader.h"
#include <expected>
#include <format>
#include <string>
#include <iostream>

std::expected<Shader, std::string> Shader::try_from(const char* const source, GLenum gl_shader_type){
    std::unique_ptr<GLuint, decltype(&free_shader)> shader(
	    new GLuint(glCreateShader(gl_shader_type)), free_shader);
    glShaderSource(*shader, 1, &source, NULL);
    glCompileShader(*shader);

    int compiled;
    glGetShaderiv(*shader, GL_COMPILE_STATUS, &compiled);
    if(!compiled){
	int log_length;
	glGetShaderiv(*shader, GL_INFO_LOG_LENGTH, &log_length);
	std::string compile_log;
	compile_log.resize(log_length);
	glGetShaderInfoLog(*shader, log_length, &log_length, compile_log.data());
	std::cout <<compile_log << std::endl;
	return std::unexpected(std::format("Shader compilation failed!\n{}", compile_log));
    }
    return Shader(std::move(shader), gl_shader_type);
}
