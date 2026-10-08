#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "attribute_layout.h"
#include "draw_objects/draw_textured_mesh_2d_t.h"
#include "glad/glad.h"
#include "image.h"
#include "mesh.h"
#include "shader_program.h"
#include "texture.h"
#include <filesystem>
#include <format>
#include <iostream>
#include <vector>

using namespace SimpleGL;

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
	glViewport(0, 0, width, height);
}

int main() {
	if (!glfwInit()) {
		std::cout << "glfw initalization failed!" << std::endl;
		return -1;
	}
	GLFWwindow *window = glfwCreateWindow(
	    500, 500, "simple_gl, example: image_window", NULL, NULL);
	if (!window) {
		std::cout << "glfw window creation failed!" << std::endl;
		return -1;
	}
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	glfwSetFramebufferSizeCallback(glfwGetCurrentContext(), framebuffer_size_callback);

	auto shaders_dir = std::filesystem::current_path().append("res").append("shaders");
	auto frag_shader_r = Shader::try_from(
	    std::filesystem::path(shaders_dir).append("basic.frag"),
	    GL_FRAGMENT_SHADER);
	if (!frag_shader_r.has_value()) {
		std::cout << frag_shader_r.error() << std::endl;
		return -1;
	}
	Shader frag_shader{std::move(frag_shader_r.value())};

	auto vert_shader_r = Shader::try_from(
		std::filesystem::path(shaders_dir).append("basic.vert"),
		GL_VERTEX_SHADER);
	if (!vert_shader_r.has_value()) {
		std::cout << vert_shader_r.error() << std::endl;
		return -1;
	}
	Shader vert_shader{std::move(vert_shader_r.value())};

	std::vector<Shader> shaders;
	shaders.emplace_back(std::move(vert_shader));
	shaders.emplace_back(std::move(frag_shader));

	auto program_r = ShaderProgram::try_from(shaders);
	if (!program_r.has_value()) {
		std::cout << program_r.error() << std::endl;
		return -1;
	}
	ShaderProgram program{std::move(program_r.value())};
	auto uniforms = program.get_uniforms();
	for (size_t i{}; i < uniforms.size(); i++) {
		std::cout << std::format(
		    "uniform_name: {}\n\tid: {}\n\tsize: {}\n\ttype: {}\n\n",
		    uniforms[i].name, i, uniforms[i].size, uniforms[i].type);
	}

	float vertices[] = {
		 0.5f,  0.5f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f, -0.5f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.0f,  0.0f,  1.0f};
	unsigned int indices[] = {0, 1, 3, 1, 2, 3};

	std::vector<AttributeLayout> layout{
		AttributeLayout(3, GLenumAttributeWrapper::FLOAT, GL_FALSE),
		AttributeLayout(2, GLenumAttributeWrapper::FLOAT, GL_FALSE)};

	auto mesh_r = Mesh::try_from(layout, vertices, sizeof(vertices), indices,
			sizeof(indices), sizeof(indices) / sizeof(unsigned int));
	if (!mesh_r.has_value()) {
		std::cout << mesh_r.error() << std::endl;
		return -1;
	}
	Mesh mesh{std::move(mesh_r.value())};

	auto image_path = "res/images/image.png";
	auto image_r = Image::from_file(image_path, 3);
	if (!image_r.has_value()) {
		std::cout << image_r.error() << std::endl;
		return -1;
	}
	Image image{std::move(image_r.value())};

	auto texture_r = TextureBuilder().from_image(image).build();
	if (!texture_r) {
		std::cout << texture_r.error() << std::endl;
		return -1;
	}
	Texture texture = std::move(texture_r.value());
	texture.set_tex_parameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	texture.set_tex_parameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	auto transform_uniform_id_r = program.get_uniform_id("model");
	if (!transform_uniform_id_r.has_value()) {
		std::cout << transform_uniform_id_r.error();
		return -1;
	}
	DrawTexturedMesh2DT draw_obj(
	    &mesh, &program,
	    std::vector<std::pair<const Texture *, GLenum>>{
	        std::pair(&texture, GL_TEXTURE0)},
	    transform_uniform_id_r.value(), glm::mat3(2.0f));

	glClearColor(0.3f, 0.4f, 0.1f, 1.0f);

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT);
		auto d_r = draw_obj.draw();
		if (!d_r.has_value()) {
			std::cout << d_r.error() << '\n';
			return -1;
		}
		glfwSwapBuffers(window);
	}
	glfwTerminate();
}
