#version 330 core

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

void build_quad(vec4 position){
    float scale = 1.0f;
    gl_Position = position + vec4(-scale, -scale, 0.0, 0.0);
    EmitVertex();   
    gl_Position = position + vec4(scale, -scale, 0.0, 0.0);
    EmitVertex();
    gl_Position = position + vec4(-scale, scale, 0.0, 0.0);
    EmitVertex();
    gl_Position = position + vec4(scale, scale, 0.0, 0.0);
    EmitVertex();
    EndPrimitive();
}

void main(){
    build_quad(gl_in[0].gl_Position);
}
