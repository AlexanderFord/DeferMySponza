#version 330

uniform sampler2DRect sampler_world_position;
uniform sampler2DRect sampler_world_normal;
uniform sampler2DRect sampler_world_material;

uniform vec3 directional_light_direction;
uniform vec3 directional_light_intensity;

out vec4 fragment_colour;

vec3 DirectionalLight(vec3 normal, vec3 direction);

void main(void)
{
	ivec2 fragCoord = ivec2(gl_FragCoord.xy);

	vec3 position = texelFetch(sampler_world_position, fragCoord).xyz;
	vec3 normal = texelFetch(sampler_world_normal, fragCoord).xyz;
	vec3 material = texelFetch(sampler_world_material, fragCoord).xyz;

	vec3 ambient_light = vec3(0.1f, 0.1f, 0.1f)*material;

	normal = normalize(normal);

	vec3 colour = ambient_light;

	colour += DirectionalLight(normal, directional_light_direction);

	fragment_colour = vec4(colour,1);
}

vec3 DirectionalLight(vec3 normal, vec3 direction)
{
	float scale = max(0, dot(normalize(normal), direction));

	vec3 intensity = vec3(directional_light_intensity.x, directional_light_intensity.y, directional_light_intensity.z)*scale;

	return intensity;
}


