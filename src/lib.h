#pragma once
#include <optional>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <concepts>

#if __cplusplus < 202002L
#error "LSP says: This is NOT compiled with C++20!"
#endif

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void process_input(GLFWwindow* window);
int test();
std::optional<unsigned int> compile_shader(const char* const source, int gl_shader_type);
std::optional<unsigned int> create_program();
unsigned int make_vbo(float* vertices, unsigned int vertices_size);
unsigned int make_ebo(unsigned* indices, unsigned int indices_byte_size);

