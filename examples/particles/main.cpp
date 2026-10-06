#include "engine.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/quaternion_transform.hpp"
#include "shader_program.h"
#include <iostream>
#include <filesystem>
#include "window.h"
#include "draw_objects/draw_textured_mesh_3d_t.h"
#include <chrono>
#include "particle_generator_cpu.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
}

int main(){
    auto engine_r = EngineBuilder().build();
    if(!engine_r.has_value()){
	std::cout << engine_r.error() << std::endl;
	return 1;
    }
    Engine engine {std::move(engine_r.value())};
    
    auto window_r = Window::opengl_window();
    if(!window_r.has_value()){
	std::cout << window_r.error() << std::endl;
	return -1;
    }
    Window window {std::move(window_r.value())};
    window.make_context_current();

    auto current_context_window = glfwGetCurrentContext();
    glfwSetFramebufferSizeCallback(current_context_window, framebuffer_size_callback);

    std::vector<Shader> shaders;
    auto vertex_shader_r = Shader::try_from(
	std::filesystem::current_path().append("res/shaders/particle.vert"),
	GL_VERTEX_SHADER);
    if(!vertex_shader_r.has_value()){
	std::cout << vertex_shader_r.error() << std::endl;
	return 1;
    }

    auto fragment_shader_r = Shader::try_from(
	std::filesystem::current_path().append("res/shaders/particle.frag"),
	GL_FRAGMENT_SHADER);
    if(!fragment_shader_r.has_value()){
	std::cout << fragment_shader_r.error() << std::endl;
	return 1;
    }

    shaders.emplace_back(std::move(vertex_shader_r.value()));
    shaders.emplace_back(std::move(fragment_shader_r.value()));

    auto program_r = ShaderProgram::try_from(shaders);
    if(!program_r.has_value()){
	std::cout << program_r.error() << std::endl;
	return 1;
    }
    ShaderProgram program {std::move(program_r.value())};
    glClearColor(0.4f, 0.4f, 0.4f, 1.0f);

    glm::mat4 proj = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);

    program.use_program();
    auto projection_r = program.get_uniform_id("projection");
    if(!projection_r.has_value()){
	std::cout << projection_r.error() << std::endl;
	return 1;
    }
    auto projection_id = std::move(projection_r.value());

    auto view_r = program.get_uniform_id("view");
    if(!view_r.has_value()){
	std::cout << view_r.error() << std::endl;
	return 1;
    }
    auto view_id = std::move(view_r.value());

    if(!program.set_uniformv(projection_id, &proj, 1, GL_FALSE).has_value()){
	std::cout << "something went wrong with uniform projection!!!! VERY BAD ERROR!" << std::endl;
	return 1;
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

    auto image_path = "res/images/image.png";
    auto image_r = Image::from_file(image_path, 3);
    if(!image_r.has_value()){
	std::cout << image_r.error() << std::endl;
	return -1;
    }
    Image image {std::move(image_r.value())};
    
    auto texture_r = TextureBuilder().from_image(image).build();
    if(!texture_r.has_value()){
	std::cout << texture_r.error() << std::endl;
	return -1;
    }
    Texture texture = std::move(texture_r.value());

    auto model_r = program.get_uniform_id("model");
    if(!model_r.has_value()){
	std::cout << model_r.error() << std::endl;
	return -1;
    }
    auto model_id = std::move(model_r.value());

    ParticleGeneratorCPU particle_generator_cpu(DrawTexturedMesh3DT(
	&mesh,
	&program,
	std::vector<std::pair<const Texture*, GLenum>> {std::pair(&texture, GL_TEXTURE0)},
	model_id,
	glm::mat4(1.0f)));
    particle_generator_cpu.particle_spawn_rate = 0.001f;
    particle_generator_cpu.particle_lifetime = 1000.0f;
    particle_generator_cpu.particle_start_velocity = glm::vec3(0.0f, 0.0f, 0.0f);
    particle_generator_cpu.particle_start_spin = glm::vec3(0.0f, 10000.0f, 0.0f);
    particle_generator_cpu.particle_start_scale = glm::vec3(2.0f, 2.0f, 2.0f);
    particle_generator_cpu.particle_spawn_queue_max_size = 1000;
    auto timer_start = std::chrono::steady_clock::now();
    particle_generator_cpu.set_particle_count_max(10000);
    auto timer_end = std::chrono::steady_clock::now();
    std::cout << std::chrono::duration<double, std::milli>(timer_end - timer_start).count() << "ms\n";

    float camera_rotation_speed {10.0f};
    glm::vec3 camera_position {0.0f, 7.0f, 7.0};
    float camera_rotation_y {0.0f};
    glm::vec3 camera_target {0.0f, 0.0f, 0.0f};

    glEnable(GL_DEPTH_TEST);

    float last_time = glfwGetTime();
    float delta_time {0.0f};
    unsigned int frames_passed {0};
    while(!window.should_close()){
	glfwPollEvents();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//camera
	glm::mat4 view = glm::lookAt(camera_position, camera_target, glm::vec3(0.0f, 1.0f, 0.0f));
	camera_rotation_y += camera_rotation_speed * delta_time;
	view = glm::rotate(view, glm::radians(camera_rotation_y), glm::vec3(0.0f, 1.0f, 0.0f));
	if(!program.set_uniformv(view_id, &view, 1, GL_FALSE).has_value()){
	    std::cout << "something went wrong with uniform view!!!! VERY BAD ERROR!" << std::endl;
	    return 1;
	}
	//DRAW BLOCK
	auto particle_generator_cpu_update_r = particle_generator_cpu.update(glfwGetTime(), delta_time);
	if(!particle_generator_cpu_update_r.has_value()){
	    std::cout << particle_generator_cpu_update_r.error() << '\n';
	}
	//END DRAW BLOCK
	window.swap_buffers();

	//time
	float current_time = glfwGetTime();
	delta_time = current_time - last_time;
	last_time = current_time;
	if(frames_passed >= 100){
	    frames_passed = 0;
	    std::cout << std::format("fps: {}\n", 1.0f / delta_time);
	    std::cout << std::format("currently_alive: {}\n", particle_generator_cpu.get_particle_avile_count());
	}else{
	    frames_passed++;
	}
    }
    return 0;
}
