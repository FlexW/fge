#version 460 core

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec3 in_tangent;
layout (location = 3) in vec3 in_bitangent;
layout (location = 4) in vec2 in_tex_coord;

out VS_OUT
{
  vec3 position;
  vec3 normal;
  vec2 tex_coord;
} vs_out;

uniform mat4 projection_mat;
uniform mat4 view_mat;

uniform mat4 world_mat;

void main()
{
  mat4 view_world_mat = view_mat * world_mat;

  vec4 P = view_world_mat * vec4(in_position, 1.0);
  vec3 N = mat3(view_world_mat) * in_normal;

  vs_out.position = P.xyz;
  vs_out.normal = N;
  vs_out.tex_coord = in_tex_coord;

  gl_Position = projection_mat * P;
}
