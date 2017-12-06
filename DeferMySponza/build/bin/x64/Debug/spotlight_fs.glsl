#version 330

uniform sampler2DRect sampler_world_position;
uniform sampler2DRect sampler_world_normal;
uniform sampler2DRect sampler_world_material;

uniform vec3 spotlight_position;
uniform float spotlight_range;
uniform vec3 spotlight_intensity;
uniform vec3 spotlight_direction;
uniform float spotlight_cone_angle;
uniform bool spotlight_castshadow;

vec3 position;
vec3 normal;
vec3 material;

out vec4 fragment_colour;

vec3 SpotLightCalc(vec3 colour);
vec3 DiffuseLightCalc(vec3 lightPos, vec3 lightInt, float attenuation);

void main()
{
	ivec2 fragCoord = ivec2(gl_FragCoord.xy);

	position = texelFetch(sampler_world_position, fragCoord).xyz;
	normal = texelFetch(sampler_world_normal, fragCoord).xyz;
	material = texelFetch(sampler_world_material, fragCoord).xyz;

	normal = normalize(normal);

	vec3 final_colour = vec3(0, 0, 0);
	SpotLightCalc(final_colour);

	fragment_colour = vec4(final_colour, 1);
}

vec3 DiffuseLightCalc(vec3 lightPos, vec3 lightInt, float attenuation)
{
	vec3 L = normalize(lightPos - position);
	float scaler = max(0, dot(L, normal))*attenuation;

	vec3 diffuse_intensity = lightInt * scaler * material;

	return diffuse_intensity;
}

vec3 SpotLightCalc(vec3 colour)
{
	float direction = dot(normalize(spotlight_position - position), -spotlight_direction);
	float effect = smoothstep(cos(spotlight_cone_angle), cos(spotlight_cone_angle / 2), direction);

	float distance = distance(spotlight_position, position);
	float attenuation = 1 - smoothstep(0.0, spotlight_range, distance);

	vec3 diffuse_intensity = DiffuseLightCalc(spotlight_position, spotlight_intensity, attenuation);

	colour += (diffuse_intensity * effect);

	return colour;
}

/*#version 330

uniform sampler2DRect sampler_world_position;
uniform sampler2DRect sampler_world_normal;
uniform sampler2DRect sampler_world_material;

uniform vec3 spotlight_position;
uniform float spotlight_range;
uniform vec3 spotlight_intensity;

vec3 position;
vec3 normal;
vec3 material;

out vec4 fragment_colour;

vec3 SpotLightCalc();
vec3 DiffuseLightCalc(vec3 lightPos, vec3 lightInt, float attenuation);

void main()
{
	ivec2 fragCoord = ivec2(gl_FragCoord.xy);

	position = texelFetch(sampler_world_position, fragCoord).xyz;
	normal = texelFetch(sampler_world_normal, fragCoord).xyz;
	material = texelFetch(sampler_world_material, fragCoord).xyz;

	normal = normalize(normal);

	vec3 colour = SpotLightCalc();

	fragment_colour = vec4(colour, 1);
}

vec3 DiffuseLightCalc(vec3 lightPos, vec3 lightInt, float attenuation)
{
	vec3 L = normalize(lightPos - position);
	float scaler = max(0, dot(L, normal))*attenuation;

	vec3 diffuse_intensity = lightInt * scaler;

	return diffuse_intensity;
}

vec3 SpotLightCalc()
{
	float dist = distance(spotlight_position, position);
	float attenuation = 1 - smoothstep(1.f, spotlight_range, dist);

	vec3 colour = DiffuseLightCalc(spotlight_position, spotlight_intensity, attenuation);

	return colour;
}*/