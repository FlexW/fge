#version 460

in vec2 frag_tex_coord;

out vec4 frag_color;

#ifdef DIFFUSE_TEX
uniform sampler2D in_diffuse_tex;
#else // DIFFUSE_TEX
uniform vec3 in_diffuse_color = vec3(0.6);
#endif // DIFFUSE_TEX

void main()
{
  vec3 color = vec3(0.0f);

  vec3 diffuse_color;
  #ifdef DIFFUSE_TEX
  diffuse_color = texture(in_diffuse_tex, frag_tex_coord).rgb;
  #else // DIFFUSE_TEX
  diffuse_color = in_diffuse_color;
  #endif // DIFFUSE_TEX

  color = diffuse_color;

  frag_color = vec4(color, 1.0);
}
