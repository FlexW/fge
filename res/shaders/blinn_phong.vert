#version 460 core

#define MAX_BONES 35

#ifdef SKINNED
layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec3 in_tangent;
layout(location = 3) in vec3 in_bitangent;
layout(location = 4) in ivec4 in_skin_bones;
layout(location = 5) in vec4 in_skin_weights;
layout(location = 6) in vec2 in_tex_coord;
#else // SKINNED
layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec3 in_tangent;
layout (location = 3) in vec3 in_bitangent;
layout (location = 4) in vec2 in_tex_coord;
#endif // SKINNED

out VS_OUT
{
  vec3 position;
  vec3 normal;
  vec2 tex_coord;
} vs_out;

uniform mat4 projection_mat;
uniform mat4 view_mat;

uniform mat4 world_mat;
#ifdef SKINNED
uniform mat4 bones[MAX_BONES];
#endif // SKINNED

void main()
{
  #ifdef SKINNED
  mat4 bone_transform = bones[in_skin_bones.x] * in_skin_weights.x;
  bone_transform += bones[in_skin_bones.y] * in_skin_weights.y;
  bone_transform += bones[in_skin_bones.z] * in_skin_weights.z;
  bone_transform += bones[in_skin_bones.w] * in_skin_weights.w;

  vec4 position = bone_transform * vec4(in_position, 1.0);
  vec3 normal = vec3(bone_transform * vec4(in_normal, 0.0));
  #else // SKINNED
  vec4 position = vec4(in_position, 1.0);
  vec3 normal = in_normal;
  #endif // SKINNED

  mat4 view_world_mat = view_mat * world_mat;

  vec4 P = view_world_mat * position;
  vec3 N = mat3(view_world_mat) * normal;

  vs_out.position = P.xyz;
  vs_out.normal = N;
  vs_out.tex_coord = in_tex_coord;

  gl_Position = projection_mat * P;
}
