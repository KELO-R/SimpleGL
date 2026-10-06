#version 330 core
uniform sampler2D ourTexture;
in vec2 texCoord;
out vec4 FragColor;

void main()
{
    FragColor = texture(ourTexture, texCoord);
    //FragColor = vec4(TexCoord, 0.0, 1.0);
    //FragColor = vec4(1.0, 0.0, 0.0, 1.0);
} 
