#version 460

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec3 in_tangent;
layout (location = 3) in vec3 in_bitangent;
layout (location = 4) in vec2 in_tex_coord;

out vec2 frag_tex_coord;


uniform mat4 projection_mat;
uniform mat4 view_mat;

uniform mat4 world_mat;

void main()
{
  frag_tex_coord = in_tex_coord;

  gl_Position = projection_mat * view_mat * world_mat *  vec4(in_position, 1.0);
}
