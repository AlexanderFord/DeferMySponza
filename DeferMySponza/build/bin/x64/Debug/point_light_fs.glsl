#version 330

uniform sampler2DRect sampler_world_position;
uniform sampler2DRect sampler_world_normal;
uniform sampler2DRect sampler_world_material;

uniform vec3 point_light_position;
uniform float point_light_range;
uniform vec3 point_light_intensity;

vec3 position;
vec3 normal;
vec3 material;

out vec4 fragment_colour;

vec3 PointLightCalc();
vec3 DiffuseLightCalc(vec3 lightPos, vec3 lightInt, float attenuation);

void main()
{
	ivec2 fragCoord = ivec2(gl_FragCoord.xy);

	position = texelFetch(sampler_world_position, fragCoord).xyz;
	normal = texelFetch(sampler_world_normal, fragCoord).xyz;
	material = texelFetch(sampler_world_material, fragCoord).xyz;

	normal = normalize(normal);

	vec3 colour = PointLightCalc();

	fragment_colour = vec4(colour, 1);
}

vec3 DiffuseLightCalc(vec3 lightPos, vec3 lightInt, float attenuation)
{
	vec3 L = normalize(lightPos - position);
	float scaler = max(0, dot(L, normal))*attenuation;

	vec3 diffuse_intensity = lightInt * scaler;

	return diffuse_intensity;
}

vec3 PointLightCalc()
{
	float dist = distance(point_light_position, position);
	float attenuation = 1 - smoothstep(1.f, point_light_range, dist);

	vec3 colour = DiffuseLightCalc(point_light_position, point_light_intensity, attenuation);

	return colour;
}
