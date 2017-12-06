#version 330

uniform mat4 projection_xform;
uniform mat4 view_xform;
uniform mat4 model_xform;


in vec3 vertex_position;
in vec3 vertex_normal;
in vec3 vertex_material;

out vec3 P;
out vec3 N;

void main(void)
{
	N = normalize(mat3(model_xform) * vertex_normal);
	P = (mat4x3(model_xform) * vec4(vertex_position, 1.0)).xyz;

	gl_Position = projection_xform * view_xform * model_xform * vec4(vertex_position, 1.0);
}