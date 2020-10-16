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
  vec3 ambient_color;
  vec3 diffuse_color;
  vec3 specular_color;
};

struct DirectionalLight
{
  vec3 direction;
  vec3 ambient_color;
  vec3 diffuse_color;
  vec3 specular_color;
};


#ifdef DIFFUSE_TEX
uniform sampler2D in_diffuse_tex;
#else // DIFFUSE_TEX
uniform vec3 in_diffuse_color = vec3(0.6);
#endif // DIFFUSE_TEX

uniform float specular_power = 200.0f;

uniform int point_light_count;
uniform PointLight point_lights[MAX_POINT_LIGHTS_COUNT];

uniform bool directional_light_enabled;
uniform DirectionalLight directional_light;

void main()
{
  vec3 diffuse_color;
  #ifdef DIFFUSE_TEX
  diffuse_color = texture(in_diffuse_tex, fs_in.tex_coord).rgb;
  #else // DIFFUSE_TEX
  diffuse_color = in_diffuse_color;
  #endif // DIFFUSE_TEX

  vec3 ambient_color = diffuse_color;
  vec3 specular_color = diffuse_color;

  vec3 N = normalize(fs_in.normal);
  vec3 V = normalize(-fs_in.position);

  vec3 color = vec3(0.0f);

  // Point lights
  for (int i = 0; i < point_light_count; ++i)
  {
    vec3 L = normalize(point_lights[i].position - fs_in.position);
    vec3 H = normalize(L + V);

    // Compute lightning
    vec3 ambient = ambient_color * point_lights[i].ambient_color;
    vec3 diffuse = max(dot(N, L), 0.0) * diffuse_color * point_lights[i].diffuse_color;
    vec3 specular = pow(max(dot(N, H), 0.0), specular_power) * specular_color * point_lights[i].specular_color;

    color += ambient + diffuse + specular;
  }

  // Directional light
  if (directional_light_enabled)
  {
    vec3 L = normalize(-directional_light.direction);
    vec3 H = normalize(L + V);

    // Compute lightning
    vec3 ambient = ambient_color * directional_light.ambient_color;
    vec3 diffuse = max(dot(N, L), 0.0) * diffuse_color * directional_light.diffuse_color;
    vec3 specular = pow(max(dot(N, H), 0.0), specular_power) * specular_color * directional_light.specular_color;

    color += ambient + diffuse + specular;
  }

  out_color = vec4(color, 1.0);
}
