#include "shader_program.h"
#include <format>
#include <iostream>

std::expected<ShaderProgram, std::string> ShaderProgram::try_from(const std::vector<const Shader*>& shaders){
    GLuint id = glCreateProgram();
    for(auto it : shaders){
	glAttachShader(id ,it->id);
	
	if (glIsShader(it->id)){
	    std::cout << "glIsShader: true\n";
	}
	else{
	    std::cout << "glIsShader: false\n";
	}
    }
    glLinkProgram(id);
    int program_link_status;
    glGetProgramiv(id, GL_LINK_STATUS, &program_link_status);
    if(!program_link_status){
	int log_length;
	glGetProgramiv(id, GL_INFO_LOG_LENGTH, &log_length);
	std::string compile_log;
	compile_log.resize(log_length);
	glGetProgramInfoLog(id, log_length, &log_length, compile_log.data());
	return std::unexpected(std::format("Shader Program linking failed!\n {}", compile_log));
    }
    return ShaderProgram(id);
}
void ShaderProgram::use_program() const{
    glUseProgram(id);
}
