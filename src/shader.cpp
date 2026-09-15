#include "shader.h"
#include <expected>
#include <format>
#include <string>
#include <iostream>

std::expected<Shader, std::string> Shader::try_from(const char* const source, GLuint gl_shader_type){
    unsigned int shader_id = glCreateShader(gl_shader_type);
    if(glIsShader(shader_id)){
	std::cout << "0 glIsShader: true\n";
    }else{
	std::cout << "0 glIsShader: false\n";
    }

    glShaderSource(shader_id, 1, &source, NULL);
    glCompileShader(shader_id);

    if(glIsShader(shader_id)){
	std::cout << "1 glIsShader: true\n";
    }else{
	std::cout << "1 glIsShader: false\n";
    }
    
    int compiled;
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &compiled);
    if(!compiled){
	int log_length;
	glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &log_length);
	std::string compile_log;
	compile_log.resize(log_length);
	glGetShaderInfoLog(shader_id, log_length, &log_length, compile_log.data());
	std::cout <<compile_log << std::endl;
	return std::unexpected(std::format("Shader compilation failed!\n{}", compile_log));
    }
    if(glIsShader(shader_id)){
	std::cout << "2 glIsShader: true\n";
    }else{
	std::cout << "2 glIsShader: false\n";
    }
    return Shader(shader_id, gl_shader_type);
}
