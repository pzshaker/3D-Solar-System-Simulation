#version 330 core

in vec3 vertexPosition, vertexNormal;
in vec2 vertexTexture;

uniform mat4 model, view, projection;

out vec3 normal, position;
out vec2 texCoord;

void main()
{
	normal = mat3(transpose(inverse(model))) * vertexNormal;
	position = vec3(model * vec4(vertexPosition, 1.0));
	texCoord = vertexTexture;
	gl_Position = projection * view * vec4(position, 1.0);
}