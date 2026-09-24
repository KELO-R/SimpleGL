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
	u.location = glGetUniformLocation(*program, u.name.c_str());
    }
    return ShaderProgram(std::move(program), std::move(uniforms));
}
void ShaderProgram::use_program() const{
    glUseProgram(*program);
}
GLint ShaderProgram::get_uniform_location(std::string_view uniform_name) const{
    return glGetUniformLocation(*program, uniform_name.cbegin());
}


#define CHECK_ID(id) do{ \
    if(uniforms.size() <= id){ \
	return std::unexpected(std::format("invalid uniform id\n\tid: {}\n\tmax_id: {}\n", id, uniforms.size())); \
    } \
}while(false)

const std::expected<ShaderProgram::Uniform, std::string> ShaderProgram::get_uniform(size_t id) const{
    CHECK_ID(id);
    return uniforms[id];
}

const std::expected<size_t, std::string> ShaderProgram::get_uniform_id(std::string_view name) const{
    for(size_t i {}; i < uniforms.size(); i++){
	if(uniforms[i].name == name)
	    return i;
    }
    return std::unexpected(std::format("invalid uniform name\n\tname: {}\n", name));
}

//set_uniform overloads
#define UNEXPECTED_INVALID_TYPE(provided_type) std::unexpected(std::format( \
"failed to find correct glUniform for value provided!\n\tuniform GLenum: {}\n\tprovided value type: {}\n\t",\
uniforms[id].type, typeid(provided_type).name()))

const std::expected<void, std::string> ShaderProgram::set_uniform(size_t id, GLfloat v0) const{
    CHECK_ID(id);
    if(uniforms[id].type != GL_FLOAT)
	return UNEXPECTED_INVALID_TYPE(GLfloat);
    glUniform1f(uniforms[id].location, v0);
    return {};
}
const std::expected<void, std::string> ShaderProgram::set_uniform(size_t id, glm::vec2 v) const{
    CHECK_ID(id);
    if(uniforms[id].type != GL_FLOAT_VEC2)
	return UNEXPECTED_INVALID_TYPE(glm::vec2);
    glUniform2f(uniforms[id].location, v.x, v.y);
    return {};
}
const std::expected<void, std::string> ShaderProgram::set_uniform(size_t id, glm::vec3 v) const{
    CHECK_ID(id);
    if(uniforms[id].type != GL_FLOAT_VEC3)
	return UNEXPECTED_INVALID_TYPE(glm::vec3);
    glUniform3f(uniforms[id].location, v.x, v.y, v.z);
    return {};
}
const std::expected<void, std::string> ShaderProgram::set_uniform(size_t id, glm::vec4 v) const{
    CHECK_ID(id);
    if(uniforms[id].type != GL_FLOAT_VEC4)
	return UNEXPECTED_INVALID_TYPE(glm::vec4);
    glUniform4f(uniforms[id].location, v.x, v.y, v.z, v.w);
    return {};
}
const std::expected<void, std::string> ShaderProgram::set_uniform(size_t id, GLint v0) const{
    CHECK_ID(id);
    switch(uniforms[id].type){
	case GL_INT:
	case GL_SAMPLER_1D:
	case GL_SAMPLER_2D:
	case GL_SAMPLER_3D:
	case GL_SAMPLER_CUBE:
	case GL_SAMPLER_1D_SHADOW:
	case GL_SAMPLER_2D_SHADOW:
	case GL_SAMPLER_1D_ARRAY:
	case GL_SAMPLER_2D_ARRAY:
	case GL_SAMPLER_1D_ARRAY_SHADOW:
	case GL_SAMPLER_2D_ARRAY_SHADOW:
	case GL_SAMPLER_2D_MULTISAMPLE:
	case GL_SAMPLER_2D_MULTISAMPLE_ARRAY:
	case GL_SAMPLER_CUBE_SHADOW:
	case GL_SAMPLER_BUFFER:
	case GL_SAMPLER_2D_RECT:
	case GL_SAMPLER_2D_RECT_SHADOW:
	case GL_INT_SAMPLER_1D:
	case GL_INT_SAMPLER_2D:
	case GL_INT_SAMPLER_3D:
	case GL_INT_SAMPLER_CUBE:
	case GL_INT_SAMPLER_1D_ARRAY:
	case GL_INT_SAMPLER_2D_ARRAY:
	case GL_INT_SAMPLER_2D_MULTISAMPLE:
	case GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY:
	case GL_INT_SAMPLER_BUFFER:
	case GL_INT_SAMPLER_2D_RECT:
	case GL_UNSIGNED_INT_SAMPLER_1D:
	case GL_UNSIGNED_INT_SAMPLER_2D:
	case GL_UNSIGNED_INT_SAMPLER_3D:
	case GL_UNSIGNED_INT_SAMPLER_CUBE:
	case GL_UNSIGNED_INT_SAMPLER_1D_ARRAY:
	case GL_UNSIGNED_INT_SAMPLER_2D_ARRAY:
	case GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE:
	case GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY:
	case GL_UNSIGNED_INT_SAMPLER_BUFFER:
	case GL_UNSIGNED_INT_SAMPLER_2D_RECT:
	    glUniform1i(uniforms[id].location, v0);
	    return {};
    }
    return UNEXPECTED_INVALID_TYPE(GLint);
}
const std::expected<void, std::string> ShaderProgram::set_uniform(size_t id, glm::ivec2 v) const{
    CHECK_ID(id);
    if(uniforms[id].type != GL_INT_VEC2)
	return UNEXPECTED_INVALID_TYPE(glm::ivec2);
    glUniform2i(uniforms[id].location, v.x, v.y);
    return {};
}
const std::expected<void, std::string> ShaderProgram::set_uniform(size_t id, glm::ivec3 v) const{
    CHECK_ID(id);
    if(uniforms[id].type != GL_INT_VEC3)
	return UNEXPECTED_INVALID_TYPE(glm::ivec3);
    glUniform3i(uniforms[id].location, v.x, v.y, v.z);
    return {};
}
const std::expected<void, std::string> ShaderProgram::set_uniform(size_t id, glm::ivec4 v) const{
    CHECK_ID(id);
    if(uniforms[id].type != GL_INT_VEC4)
	return UNEXPECTED_INVALID_TYPE(glm::ivec4);
    glUniform4i(uniforms[id].location, v.x, v.y, v.z, v.w);
    return {};
}
const std::expected<void, std::string> ShaderProgram::set_uniform(size_t id, GLuint v0) const{
    CHECK_ID(id);
    if(uniforms[id].type != GL_UNSIGNED_INT)
	return UNEXPECTED_INVALID_TYPE(GLuint);
    glUniform1ui(uniforms[id].location, v0);
    return {};
}
const std::expected<void, std::string> ShaderProgram::set_uniform(size_t id, glm::uvec2 v) const{
    CHECK_ID(id);
    if(uniforms[id].type != GL_UNSIGNED_INT_VEC2)
	return UNEXPECTED_INVALID_TYPE(glm::uvec2);
    glUniform2ui(uniforms[id].location, v.x, v.y);
    return {};
}
const std::expected<void, std::string> ShaderProgram::set_uniform(size_t id, glm::uvec3 v) const{
    CHECK_ID(id);
    if(uniforms[id].type != GL_UNSIGNED_INT_VEC3)
	return UNEXPECTED_INVALID_TYPE(glm::uvec3);
    glUniform3ui(uniforms[id].location, v.x, v.y, v.z);
    return {};
}
const std::expected<void, std::string> ShaderProgram::set_uniform(size_t id, glm::uvec4 v) const{
    CHECK_ID(id);
    if(uniforms[id].type != GL_UNSIGNED_INT_VEC4)
	return UNEXPECTED_INVALID_TYPE(glm::uvec4);
    glUniform4ui(uniforms[id].location, v.x, v.y, v.z, v.w);
    return {};
}

//NOTE: glm::value_ptr is NOT used because if underlying memory layout is not compact raw data,
//then there would be no compatibility with assigning arrays of elements at all (count > 1)
const std::expected<void, std::string>
ShaderProgram::set_uniformv(size_t id, const GLfloat* ptr, GLsizei count) const{
    CHECK_ID(id);
    if(uniforms[id].type != GL_FLOAT)
	return UNEXPECTED_INVALID_TYPE(GLfloat*);
    glUniform1fv(uniforms[id].location, count, ptr);
    return {};
}
const std::expected<void, std::string>
ShaderProgram::set_uniformv(size_t id, const glm::vec2* ptr, GLsizei count) const{
    CHECK_ID(id);
    if(uniforms[id].type != GL_FLOAT_VEC2)
	return UNEXPECTED_INVALID_TYPE(glm::vec2*);
    glUniform2fv(uniforms[id].location, count, (GLfloat*)ptr);
    return {};
}
const std::expected<void, std::string>
ShaderProgram::set_uniformv(size_t id, const glm::vec3* ptr, GLsizei count) const{
    CHECK_ID(id);
    if(uniforms[id].type != GL_FLOAT_VEC3)
	return UNEXPECTED_INVALID_TYPE(glm::vec3*);
    glUniform3fv(uniforms[id].location, count, (GLfloat*)ptr);
    return {};
}
const std::expected<void, std::string>
ShaderProgram::set_uniformv(size_t id, const glm::vec4* ptr, GLsizei count) const{
    CHECK_ID(id);
    if(uniforms[id].type != GL_FLOAT_VEC4)
	return UNEXPECTED_INVALID_TYPE(glm::vec4*);
    glUniform4fv(uniforms[id].location, count, (GLfloat*)ptr);
    return {};
}
const std::expected<void, std::string>
ShaderProgram::set_uniformv(size_t id, const GLint* ptr, GLsizei count) const{
    CHECK_ID(id);
    switch(uniforms[id].type){
	case GL_INT:
	case GL_SAMPLER_1D:
	case GL_SAMPLER_2D:
	case GL_SAMPLER_3D:
	case GL_SAMPLER_CUBE:
	case GL_SAMPLER_1D_SHADOW:
	case GL_SAMPLER_2D_SHADOW:
	case GL_SAMPLER_1D_ARRAY:
	case GL_SAMPLER_2D_ARRAY:
	case GL_SAMPLER_1D_ARRAY_SHADOW:
	case GL_SAMPLER_2D_ARRAY_SHADOW:
	case GL_SAMPLER_2D_MULTISAMPLE:
	case GL_SAMPLER_2D_MULTISAMPLE_ARRAY:
	case GL_SAMPLER_CUBE_SHADOW:
	case GL_SAMPLER_BUFFER:
	case GL_SAMPLER_2D_RECT:
	case GL_SAMPLER_2D_RECT_SHADOW:
	case GL_INT_SAMPLER_1D:
	case GL_INT_SAMPLER_2D:
	case GL_INT_SAMPLER_3D:
	case GL_INT_SAMPLER_CUBE:
	case GL_INT_SAMPLER_1D_ARRAY:
	case GL_INT_SAMPLER_2D_ARRAY:
	case GL_INT_SAMPLER_2D_MULTISAMPLE:
	case GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY:
	case GL_INT_SAMPLER_BUFFER:
	case GL_INT_SAMPLER_2D_RECT:
	case GL_UNSIGNED_INT_SAMPLER_1D:
	case GL_UNSIGNED_INT_SAMPLER_2D:
	case GL_UNSIGNED_INT_SAMPLER_3D:
	case GL_UNSIGNED_INT_SAMPLER_CUBE:
	case GL_UNSIGNED_INT_SAMPLER_1D_ARRAY:
	case GL_UNSIGNED_INT_SAMPLER_2D_ARRAY:
	case GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE:
	case GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY:
	case GL_UNSIGNED_INT_SAMPLER_BUFFER:
	case GL_UNSIGNED_INT_SAMPLER_2D_RECT:
	    glUniform1iv(uniforms[id].location, count, ptr);
	    return {};
    }
    return UNEXPECTED_INVALID_TYPE(GLint*);
}
const std::expected<void, std::string>
ShaderProgram::set_uniformv(size_t id, const glm::ivec2* ptr, GLsizei count) const{
    CHECK_ID(id);
    if(uniforms[id].type != GL_INT_VEC2)
	return UNEXPECTED_INVALID_TYPE(glm::ivec2*);
    glUniform2iv(uniforms[id].location, count, (GLint*)ptr);
    return {};
}
const std::expected<void, std::string>
ShaderProgram::set_uniformv(size_t id, const glm::ivec3* ptr, GLsizei count) const{
    CHECK_ID(id);
    if(uniforms[id].type != GL_INT_VEC3)
	return UNEXPECTED_INVALID_TYPE(glm::ivec3*);
    glUniform3iv(uniforms[id].location, count, (GLint*)ptr);
    return {};
}
const std::expected<void, std::string>
ShaderProgram::set_uniformv(size_t id, const glm::ivec4* ptr, GLsizei count) const{
    CHECK_ID(id);
    if(uniforms[id].type != GL_INT_VEC4)
	return UNEXPECTED_INVALID_TYPE(glm::ivec4*);
    glUniform4iv(uniforms[id].location, count, (GLint*)ptr);
    return {};
}
const std::expected<void, std::string>
ShaderProgram::set_uniformv(size_t id, const GLuint* ptr, GLsizei count) const{
    CHECK_ID(id);
    if(uniforms[id].type != GL_UNSIGNED_INT)
	return UNEXPECTED_INVALID_TYPE(GLuint*);
    glUniform1uiv(uniforms[id].location, count, ptr);
    return {};
}
const std::expected<void, std::string>
ShaderProgram::set_uniformv(size_t id, const glm::uvec2* ptr, GLsizei count) const{
    CHECK_ID(id);
    if(uniforms[id].type != GL_UNSIGNED_INT_VEC2)
	return UNEXPECTED_INVALID_TYPE(glm::uvec2*);
    glUniform2uiv(uniforms[id].location, count, (GLuint*)ptr);
    return {};
}
const std::expected<void, std::string>
ShaderProgram::set_uniformv(size_t id, const glm::uvec3* ptr, GLsizei count) const{
    CHECK_ID(id);
    if(uniforms[id].type == GL_UNSIGNED_INT_VEC3)
        glUniform3uiv(uniforms[id].location, count, (GLuint*)ptr);
    return UNEXPECTED_INVALID_TYPE(glm::uvec3*);
}
const std::expected<void, std::string>
ShaderProgram::set_uniformv(size_t id, const glm::uvec4* ptr, GLsizei count) const{
    CHECK_ID(id);
    if(uniforms[id].type != GL_UNSIGNED_INT_VEC4)
	return UNEXPECTED_INVALID_TYPE(glm::uvec4*);
    glUniform4uiv(uniforms[id].location, count, (GLuint*)ptr);
    return {};
}
const std::expected<void, std::string>
ShaderProgram::set_uniformv(size_t id, const glm::mat2* ptr, GLsizei count, GLboolean transpose) const{
    CHECK_ID(id);
    if(uniforms[id].type != GL_FLOAT_MAT2)
	return UNEXPECTED_INVALID_TYPE(glm::mat2*);
    glUniformMatrix2fv(uniforms[id].location, count, transpose, (GLfloat*)ptr);
    return {};
}
const std::expected<void, std::string>
ShaderProgram::set_uniformv(size_t id, const glm::mat3* ptr, GLsizei count, GLboolean transpose) const{
    CHECK_ID(id);
    if(uniforms[id].type != GL_FLOAT_MAT3)
	return UNEXPECTED_INVALID_TYPE(glm::mat3*);
    glUniformMatrix3fv(uniforms[id].location, count, transpose, (GLfloat*)ptr);
    return {};
}
const std::expected<void, std::string>
ShaderProgram::set_uniformv(size_t id, const glm::mat4* ptr, GLsizei count, GLboolean transpose) const{
    CHECK_ID(id);
    if(uniforms[id].type != GL_FLOAT_MAT4)
	return UNEXPECTED_INVALID_TYPE(glm::mat4*);
    glUniformMatrix4fv(uniforms[id].location, count, transpose, (GLfloat*)ptr);
    return {};
}
const std::expected<void, std::string>
ShaderProgram::set_uniformv(size_t id, const glm::mat2x3* ptr, GLsizei count, GLboolean transpose) const{
    CHECK_ID(id);
    if(uniforms[id].type != GL_FLOAT_MAT2x3)
	return UNEXPECTED_INVALID_TYPE(glm::mat2x3*);
    glUniformMatrix2x3fv(uniforms[id].location, count, transpose, (GLfloat*)ptr);
    return {};
}
const std::expected<void, std::string>
ShaderProgram::set_uniformv(size_t id, const glm::mat3x2* ptr, GLsizei count, GLboolean transpose) const{
    CHECK_ID(id);
    if(uniforms[id].type != GL_FLOAT_MAT3x2)
	return UNEXPECTED_INVALID_TYPE(glm::mat3x2*);
    glUniformMatrix3x2fv(uniforms[id].location, count, transpose, (GLfloat*)ptr);
    return {};
}
const std::expected<void, std::string>
ShaderProgram::set_uniformv(size_t id, const glm::mat2x4* ptr, GLsizei count, GLboolean transpose) const{
    CHECK_ID(id);
    if(uniforms[id].type != GL_FLOAT_MAT2x4)
	return UNEXPECTED_INVALID_TYPE(glm::mat2x4*);
    glUniformMatrix2x4fv(uniforms[id].location, count, transpose, (GLfloat*)ptr);
    return {};
}
const std::expected<void, std::string>
ShaderProgram::set_uniformv(size_t id, const glm::mat4x2* ptr, GLsizei count, GLboolean transpose) const{
    CHECK_ID(id);
    if(uniforms[id].type != GL_FLOAT_MAT4x2)
	return UNEXPECTED_INVALID_TYPE(glm::mat4x2*);
    glUniformMatrix4x2fv(uniforms[id].location, count, transpose, (GLfloat*)ptr);
    return {};
}
const std::expected<void, std::string>
ShaderProgram::set_uniformv(size_t id, const glm::mat3x4* ptr, GLsizei count, GLboolean transpose) const{
    CHECK_ID(id);
    if(uniforms[id].type != GL_FLOAT_MAT3x4)
	return UNEXPECTED_INVALID_TYPE(glm::mat3x4*);
    glUniformMatrix3x4fv(uniforms[id].location, count, transpose, (GLfloat*)ptr);
    return {};
}
const std::expected<void, std::string>
ShaderProgram::set_uniformv(size_t id, const glm::mat4x3* ptr, GLsizei count, GLboolean transpose) const{
    CHECK_ID(id);
    if(uniforms[id].type != GL_FLOAT_MAT4x3)
	return UNEXPECTED_INVALID_TYPE(glm::mat4x3*);
    glUniformMatrix4x3fv(uniforms[id].location, count, transpose, (GLfloat*)ptr);
    return {};
}
