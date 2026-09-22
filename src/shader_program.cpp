#include "shader_program.h"
#include <format>

std::vector<ShaderProgram::Uniform> ShaderProgram::extract_uniforms(){
    return uniforms;
}

std::expected<ShaderProgram, std::string> 
ShaderProgram::try_from(const std::vector<Shader>& shaders){
    std::unique_ptr<GLuint, decltype(&free_program)> program(
	    new GLuint(glCreateProgram()), free_program);

    for(auto& it : shaders){
	glAttachShader(*program, *it.shader);
    }
    glLinkProgram(*program);
    int program_link_status;
    glGetProgramiv(*program, GL_LINK_STATUS, &program_link_status);
    if(!program_link_status){
	int log_length;
	glGetProgramiv(*program, GL_INFO_LOG_LENGTH, &log_length);
	std::string compile_log;
	compile_log.resize(log_length);
	glGetProgramInfoLog(*program, log_length, &log_length, compile_log.data());
	return std::unexpected(std::format("Shader Program linking failed!\n {}", compile_log));
    }

    std::vector<Uniform> uniforms;
    GLint active_uniforms_count;
    glGetProgramiv(*program, GL_ACTIVE_UNIFORMS, &active_uniforms_count);
    uniforms.resize(active_uniforms_count);
    GLint active_uniform_name_max_length;
    glGetProgramiv(*program, GL_ACTIVE_UNIFORM_MAX_LENGTH, &active_uniform_name_max_length);
    for(GLuint i{}; i < active_uniforms_count; i++){
	Uniform& u = uniforms[i];
	u.name.resize(active_uniform_name_max_length);
	GLsizei actual_name_size;
	glGetActiveUniform(*program, i, active_uniform_name_max_length, &actual_name_size, 
	    &u.size, &u.type, u.name.data());
	u.name.resize(actual_name_size);
    }
    return ShaderProgram(std::move(program), std::move(uniforms));
}
void ShaderProgram::use_program() const{
    glUseProgram(*program);
}
GLint ShaderProgram::get_uniform_location(std::string_view uniform_name) const{
    return glGetUniformLocation(*program, uniform_name.cbegin());
}
