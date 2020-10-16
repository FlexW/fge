#version 460

layout (location = 0) in vec3 in_position;

uniform mat4 projection_mat;
uniform mat4 view_mat;

uniform mat4 world_mat;

void main()
{
  gl_Position = projection_mat * view_mat * world_mat *  vec4(in_position, 1.0);
}
