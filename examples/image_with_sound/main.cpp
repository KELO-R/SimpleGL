#include "attribute_layout.h"
#include "engine.h"
#include "window.h"
#include "shader_program.h"
#include "mesh.h"
#include "texture.h"
#include "gl_helper.h"
#include "audio/audio.h"
#include "draw_objects/draw_textured_mesh_2d_t.h"
#include <format>
#include <iostream>
#include <vector>

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

int main(){
    auto engine_r = EngineBuilder().build();
    if(!engine_r.has_value()){
	std::cout << engine_r.error() << std::endl;
	return -1;
    }
    Engine engine {std::move(engine_r.value())};

    auto window_r = Window::opengl_window();
    if(!window_r.has_value()){
	std::cout << window_r.error() << std::endl;
	return -1;
    }
    Window window {std::move(window_r.value())};
    window.make_context_current();
    glfwSetFramebufferSizeCallback(glfwGetCurrentContext(), framebuffer_size_callback);


    const char* vertex_shader_source = R"(#version 330 core
	layout (location = 0) in vec3 aPos;
	layout (location = 1) in vec2 aTexCoord;
	out vec2 TexCoord;
	uniform mat3 model;

	void main()
	{
	    gl_Position = vec4(model * aPos, 1.0);
	    TexCoord = aTexCoord;
	}
    )";
    const char* fragment_shader_source = R"(#version 330 core
	uniform sampler2D ourTexture;
	in vec2 TexCoord;
	out vec4 FragColor;

	void main()
	{
	    FragColor = texture(ourTexture, TexCoord);
	    //FragColor = vec4(TexCoord, 0.0, 1.0);
	    //FragColor = vec4(1.0, 0.0, 0.0, 1.0);
	} 
    )";

    auto frag_shader_r = Shader::try_from(fragment_shader_source, GL_FRAGMENT_SHADER);
    if(!frag_shader_r.has_value()){
	std::cout << frag_shader_r.error() << std::endl;
	return -1;
    }
    Shader frag_shader {std::move(frag_shader_r.value())};

    auto vert_shader_r = Shader::try_from(vertex_shader_source, GL_VERTEX_SHADER);
    if(!vert_shader_r.has_value()){
	std::cout << vert_shader_r.error() << std::endl;
	return -1;
    }
    Shader vert_shader {std::move(vert_shader_r.value())};

    std::vector<Shader> shaders;
    shaders.emplace_back(std::move(vert_shader));
    shaders.emplace_back(std::move(frag_shader));

    auto program_r = ShaderProgram::try_from(shaders);
    if(!program_r.has_value()){
	std::cout << program_r.error() << std::endl;
	return -1;
    }
    ShaderProgram program {std::move(program_r.value())};
    auto uniforms = program.get_uniforms();
    for(size_t i{}; i < uniforms.size(); i++){
	std::cout << std::format("uniform_name: {}\n\tid: {}\n\tsize: {}\n\ttype: {}\n\n",
	    uniforms[i].name, i, uniforms[i].size, uniforms[i].type);
    }

    float vertices[] = {
	// positions          // texture coords
	 0.5f,  0.5f, 0.0f,   1.0f, 1.0f,   // top right
	 0.5f, -0.5f, 0.0f,   1.0f, 0.0f,   // bottom right
	-0.5f, -0.5f, 0.0f,   0.0f, 0.0f,   // bottom left
	-0.5f,  0.5f, 0.0f,   0.0f, 1.0f    // top left 
    };
    unsigned int indices[] = {  // note that we start from 0!
	0, 1, 3,   // first triangle
	1, 2, 3    // second triangle
    }; 

    std::vector<AttributeLayout> layout {
	AttributeLayout(3, GLenumWrapper::FLOAT, GL_FALSE),
	AttributeLayout(2, GLenumWrapper::FLOAT, GL_FALSE)
    }; 

    auto mesh_r = Mesh::try_from(layout, 
	    vertices, sizeof(vertices), 
	    indices, sizeof(indices), sizeof(indices) / sizeof(unsigned int));
    if(!mesh_r.has_value()){
	std::cout << mesh_r.error() << std::endl;
	return -1;
    }
    Mesh mesh {std::move(mesh_r.value())};

    auto image_path = "image.png";
    auto image_r = Image::from_file(image_path, 3);
    if(!image_r.has_value()){
	std::cout << image_r.error() << std::endl;
	return -1;
    }
    Image image {std::move(image_r.value())};
    
    auto texture_r = TextureBuilder().from_image(image).build();
    if(!texture_r){
	std::cout << texture_r.error() << std::endl;
	return -1;
    }
    Texture texture = std::move(texture_r.value());

    auto audio_r = Audio::create();
    if(!audio_r.has_value()){
	std::cout << audio_r.error() << std::endl;
	return -1;
    }
    Audio audio {std::move(audio_r.value())};
    audio.play("music.wav");

    glClearColor(0.3f, 0.4f, 0.1f, 1.0f);

    auto transform_uniform_id_r = program.get_uniform_id("model");
    if(!transform_uniform_id_r.has_value()){
	std::cout << transform_uniform_id_r.error();
	return -1;
    }
    DrawTexturedMesh2DT draw_obj(
	transform_uniform_id_r.value(),
	&mesh,
	&program,
	glm::mat3(2.0f),
	std::vector<std::pair<const Texture*, GLenum>> {std::pair(&texture, GL_TEXTURE0)});

    while(!window.should_close()){
	glfwPollEvents();
	glClear(GL_COLOR_BUFFER_BIT);
	auto d_r = draw_obj.draw();
	if(!d_r.has_value()){
	    std::cout << d_r.error() << '\n';
	    return -1;
	}
	window.swap_buffers();
    }
}
