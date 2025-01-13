#version 330 core
in vec3 aPos;

out vec3 TexCoords;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;

void main()
{
    TexCoords = aPos;
    vec4 pos = model * vec4(aPos, 1.0);
    gl_Position = projection * view * pos;
}
