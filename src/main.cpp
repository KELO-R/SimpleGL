#include <iostream>
#include "renderer.h"

#define PROCESS_EXCEPTION(x) do{\
    auto r = x;\
    if (!r.has_value()){\
	std::cout << r.error() << std::endl;\
	return 1;\
    }\
}while(false)

int main()
{
    const char* vertex_shader_src = R"(
	#version 330 core
	layout (location = 0) in vec3 aPos;

	void main()
	{
	    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
	}
    )";
    const char* fragment_shader_src = R"(
	#version 330 core
	out vec4 FragColor;

	void main()
	{
	    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
	} 
    )";
    if (!glfwInit()){
	std::cerr << "glfw failed to initialize!" << std::endl;
	return 1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(500, 500, "GLFW window title", NULL, NULL);
    if (window == NULL){
	std::cerr << "glfw window initialization failed" << std::endl;
	glfwTerminate();
	return 2;
    }
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
	std::cout << "glad failed to initialize!" << std::endl;
	return 3;
    }
    glViewport(0, 0, 500, 500);
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

    check_gl("before renderer");
    Renderer renderer{};
    check_gl("after renderer");

    //std::cout << "compiling vertex_shader_src\n";
    PROCESS_EXCEPTION(renderer.shaders.add_res(vertex_shader_src, GL_VERTEX_SHADER));

    check_gl("after add_res vertex_shader");
    renderer.shaders.resources.at(0).handle().get().has_value();
    //std::cout << "compiling fragment_shader_src\n";
    PROCESS_EXCEPTION(renderer.shaders.add_res(fragment_shader_src, GL_FRAGMENT_SHADER));
    check_gl("after add_res fragment_shader");

    std::vector<const Shader*> shaders{};
    for(auto& it : renderer.shaders.resources){
	auto r = it.handle().get();
	if(!r.has_value()){
	    std::cout << "SHIT!\n";
	    return 1;
	}
	shaders.push_back(r.value());
    }
    check_gl("after shaders vector creation");

    PROCESS_EXCEPTION(renderer.shader_programs.add_res(shaders));

    check_gl("after add_res shader_program");
    renderer.shaders.resources.clear();

    check_gl("after clear shaders.resources");
    float vertices[] = {
	 0.5f,  0.5f, 0.0f,  // top right
	 0.5f, -0.5f, 0.0f,  // bottom right
	-0.5f, -0.5f, 0.0f,  // bottom left
	-0.5f,  0.5f, 0.0f   // top left 
    };
    unsigned int indices[] = {  // note that we start from 0!
	0, 1, 3,   // first triangle
	1, 2, 3    // second triangle
    }; 
    std::vector<AttributeLayout> layout = {AttributeLayout(3, GLenumWrapper::FLOAT, GL_FALSE)};

    PROCESS_EXCEPTION(
	renderer.meshes.add_res( layout, 
	    vertices, sizeof(vertices), 
	    indices, sizeof(indices), 
	    sizeof(indices) / sizeof(unsigned int)
	)
    );
    check_gl("after add_res meshes");

    const ShaderProgram* shader;
    auto r_shader = renderer.shader_programs.resources.at(0).handle().get();
    if(r_shader.has_value()){
	shader = r_shader.value();
    }else{
	return 1;
    }
    const Mesh* mesh;
    auto r_mesh = renderer.meshes.resources.at(0).handle().get();
    if(r_mesh.has_value()){
	 mesh = r_mesh.value();
    }else{
	std::cout << "Mesh handle is empty!\n";
	return 1;
    }
    check_gl("before while");
    while(!glfwWindowShouldClose(window)){
	glClear(GL_COLOR_BUFFER_BIT);
	check_gl("before shader");
	shader->use_program();
	check_gl("before mesh");
	mesh->draw_elements();
	check_gl("after mesh");
	glfwSwapBuffers(window);
	check_gl("after swap");
	glfwPollEvents();
    }
    

    return 0;
}
