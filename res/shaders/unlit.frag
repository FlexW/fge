#version 460

out vec4 out_color;

uniform vec3 color = vec3(1.0f);

void main()
{
  out_color = vec4(color, 1.0f);
}
