#version 330

uniform vec3 diffuse_colour;

in vec3 P;
in vec3 N;

out vec4 vertex_position;
out vec4 vertex_normal;
out vec4 vertex_material;
out vec4 fragment_colour;

void main(void)
{
	vertex_position = vec4(P, 1.0);
	vertex_normal = vec4(N, 1.0);
	vertex_material = vec4(diffuse_colour, 1);
}

