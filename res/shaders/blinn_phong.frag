#version 460 core

#define MAX_POINT_LIGHTS_COUNT 5

in VS_OUT
{
  vec3 position;
  vec3 normal;
  vec2 tex_coord;
} fs_in;

layout (location = 0) out vec4 out_color;

struct PointLight
{
  vec3 position;
};

#ifdef DIFFUSE_TEX
uniform sampler2D in_diffuse_tex;
#else // DIFFUSE_TEX
uniform vec3 in_diffuse_color = vec3(0.6);
#endif // DIFFUSE_TEX

uniform float specular_power = 200.0f;

uniform int point_light_count;
uniform PointLight point_lights[MAX_POINT_LIGHTS_COUNT];

void main()
{
  vec3 diffuse_color;
  #ifdef DIFFUSE_TEX
  diffuse_color = texture(in_diffuse_tex, fs_in.tex_coord).rgb;
  #else // DIFFUSE_TEX
  diffuse_color = in_diffuse_color;
  #endif // DIFFUSE_TEX

  vec3 specular_color = diffuse_color;

  vec3 N = normalize(fs_in.normal);
  vec3 V = normalize(-fs_in.position);

  vec3 color = vec3(0.0f);

  for (int i = 0; i < point_light_count; ++i)
  {
    vec3 L = normalize(point_lights[i].position - fs_in.position);
    vec3 H = normalize(L + V);

    // Compute lightning
    vec3 diffuse = max(dot(N, L), 0.0) * diffuse_color;
    vec3 specular = pow(max(dot(N, H), 0.0), specular_power) * specular_color;

    color += diffuse + specular;
  }

  out_color = vec4(color, 1.0);
}
